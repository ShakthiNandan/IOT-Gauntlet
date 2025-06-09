from ctypes import cast,POINTER
from comtypes import CLSCTX_ALL
from pycaw.pycaw import AudioUtilities, IAudioEndpointVolume

while True:
    devices = AudioUtilities.GetSpeakers()
    interface = devices.Activate(IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
    volume = cast(interface, POINTER(IAudioEndpointVolume))
    print(list(range(-64,1,6)))
    dict=dict(enumerate(range(-64,1,6)))
    print(dict)
    vol=volume.SetMasterVolumeLevel(0,None) 
    vol_str = f"{vol:.0f}%"
    print("Volume:",vol_str)
    break
