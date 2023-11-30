# Authors: Daniel McLawhorn and Kent Chesley
# Date: 11/2023
# (Original code by Daniel from Project/Motor_Control_From_Speech/WiFi_Audio was
# adated by Kent to implement the robot's finite state machine.)

import time
import volume
import socket
from sys import base_prefix   
import microphone
import speech
import bandpass

base_pwm = 300
lPWM = base_pwm # Just for testing.
rPWM = base_pwm # Just for testing.

sock = socket.socket()
host = "172.20.10.5"    #ESP32 IP in local network
port = 80               #ESP32 Server Port    
sock.connect((host, port))
print("Connected!")
state = 0
while True:
    microphone.record()
    bandpass
    state = speech.rec(state)
    #Follow me task
    if(state == 2):
        print("started follow me task")
        rState = volume.sound_dir()
        rState = str(rState)
        message = str(rState) + " " + str(lPWM) + " " + str(rPWM)   
        print("rState: ",rState)
        print("State: ",state)
        sock.send(message.encode())
        """"
        time.sleep(0.1)
        rState = 0
        rState = str(rState)
        message = str(rState) + " " + str(lPWM) + " " + str(rPWM)   
        print("rState: ",rState)
        print("State: ",state)
        sock.send(message.encode())
        time.sleep(0.1)
        """
        time.sleep(1)
        
    else:
        rState = 0
        rState = str(rState)
        message = str(rState) + " " + str(lPWM) + " " + str(rPWM)   
        print("rState: ",rState)
        print("State: ",state)
        sock.send(message.encode())