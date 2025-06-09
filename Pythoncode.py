import requests
import time
import urllib.request 
from bs4 import BeautifulSoup
import pyautogui as p
import webbrowser
from ctypes import cast,POINTER
from comtypes import CLSCTX_ALL
from pycaw.pycaw import AudioUtilities, IAudioEndpointVolume

def cut(text):
    text=str(text)
    cut_text=text[text.index(">")+1]
    if cut_text=="1":
        return 0
    else:
        return 1

def cuts(text):
    text=str(text)
    cut_text=text[text.find(">")+1:text.rfind("<")]
    return int(cut_text)
while(True):
    URL = "http://192.168.4.1"
    #webbrowser.open(URL)
    page = requests.get(URL)
    soup = BeautifulSoup(page.content, "html.parser")
    right = soup.find(id="right")
    left = soup.find(id="left")
    ultra_on = soup.find(id="ultra_on")
    ultra_values = soup.find(id="ultra_values")
    #print("Right Button status:",cut(right))
    #print("Left Button status:",cut(left))
    left=cut(left)
    right=cut(right)
    ultra_on=cuts(ultra_on)
    ultra_values=cuts(ultra_values)
    #print("Left:",left,type(left))
    '''if left == 1:
        p.press('left')
        time.sleep(0.5)
    if right == 1:
        p.press('right')
        time.sleep(0.5)'''
    new=0
    start=0
    if ultra_on == 1:
        
        devices = AudioUtilities.GetSpeakers()
        interface = devices.Activate(IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
        volume = cast(interface, POINTER(IAudioEndpointVolume))
        if new == 0:
            curvol=volume.GetMasterVolumeLevel()
            if curvol>=-65 and curvol<=-56:
                curvol=-64
            elif curvol>=-55 and curvol<=-46:
                curvol=-55
            elif curvol>=-45 and curvol<=-36:
                curvol>=-45
            elif curvol>=-35 and curvol<=-26:
                curvol>=-35
            elif curvol>=-25 and curvol<=-16:
                curvol>=-25
            elif curvol>=-15 and curvol<=0:
                curvol>=-15
            else:
                curvol=0
            start=curvol
        print("CUrrent Volume-------------->",curvol)
        print("Starting Volume------------->",start)
        
        print("Ultrasonic reading:",ultra_values)
        ultra_values+=-32
        if ultra_values<=-64:    
            ultra_values=-64
        elif ultra_values>=0:
            ultra_values=0
        print("Ultrasonic reading:",ultra_values)
       # print("started at",start)
        sounds=dict(enumerate(range(-60,1,2)))
        print("this------------>",ultra_values)
        vol_to_set=ultra_values#sounds[ultra_values]
        volume.SetMasterVolumeLevel(float(vol_to_set),None)
    else:
        new=0
    

