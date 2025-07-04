// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebSrv.h>

const char* ssid     = "ESP8266 LED Blink ";
const char* password = "ledonoff";
int IRSensorL = 16;
int IRSensorR = 5;
#define BUTTON_PIN 2
const int pingPin = 0; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 4; // Echo Pin of Ultrasonic Sensor
int drag = 0;
int lpressed = 0;
int rpressed = 0;
int left_pressed = 0;
int right_pressed = 0;
int ultra_on = 0;
int ultra_values=0;
long duration, cm;
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
unsigned long previousMillis = 0;    // will store last time DHT was updated
const long interval = 10000;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP8266 IOT GAUNLET</h2>
  <p>
    <span class="labels">Left Button</span> 
    <span id="left">%LEFT%</span>
  </p>
  <p>
    <span class="labels">Right Button</span> 
    <span id="right">%RIGHT%</span>
  </p>
  <p>
    <span class="labels">Ultrasonic Sensor</span>
    <span id="ultra_on">%ULTRA_ON%</span>
  </p>
  <div>
    <span class="labels">Ultrasonic Sensor Values:</span>
    <span id="ultra_values">%ULTRA_VALUES%</span>
  </div>
</body>
<script>

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("left").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/left", true);
  xhttp.send();
}, 500 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("right").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/right", true);
  xhttp.send();
}, 500 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ultra_on").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/ultra_on", true);
  xhttp.send();
}, 500 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ultra_values").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/ultra_values", true);
  xhttp.send();
}, 250 ) ;
</script>
</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var) {
  //Serial.println(var);
  if (var == "LEFT") {
    return String(left_pressed);
  }
  else if (var == "RIGHT") {
    return String(right_pressed);
  }
  else if (var == "ULTRA_ON") {
    return String(ultra_on);
  }
  else if (var == "ULTRA_VALUES") {
    return String(cm);
  }
  return String();
}

void setup() {

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.println("Serial Working");
  pinMode(IRSensorL, INPUT);
  pinMode(IRSensorR, INPUT);
  // Serial port for debugging purposes
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/left", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(left_pressed).c_str());
  });
  server.on("/right", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(right_pressed).c_str());
  });
  server.on("/right", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(ultra_on).c_str());
  });
  server.on("/right", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(ultra_values).c_str());
  });
  // Start server
  server.begin();
}

void loop() {
  int leftPress = digitalRead(IRSensorL);
  int rightPress = digitalRead(IRSensorR);
  if ((rpressed == 1) && (rightPress != 1)) {
    rpressed = 0;
    Serial.println("Right Finished");
  }
  if ((rightPress == 1) && (rpressed == 0)) {
    Serial.println("Right Key Pressed");
    rpressed = 1;
    delay(500);
  }
  if ((lpressed == 1) && (leftPress != 1)) {
    Serial.println("Left Finished");
    lpressed = 0;
  }
  if ((leftPress == 1) && (lpressed == 0)) {
    Serial.println("Left Key Pressed");
    lpressed = 1;
    delay(500);
  }
  ultra_on = !digitalRead(BUTTON_PIN);
  left_pressed = lpressed;
  right_pressed = rpressed;
  ultra_values=cm;

  if (ultra_on == 1) {
    pinMode(pingPin, OUTPUT);
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pingPin, LOW);
    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);
    cm = duration / 29 / 2;
    Serial.print(cm);
    Serial.print("cm");
    Serial.println();
  }
  else{
    ultra_values=0;
  }


}
