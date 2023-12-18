# Authors: Daniel McLawhorn and Kent Chesley
# Date: 11/2023
# (Original code by Daniel from Project/Motor_Control_From_Speech/WiFi_Audio was
# adated by Kent to implement the robot's finite state machine.)

import time
import volume
import socket
from sys import base_prefix   
import microphone
import microphone_short
from statistics import mode
import speech
import bandpass
import findCone
import cv2 
import img_dir
import numpy as np

base_pwm = 320
lPWM = base_pwm # Just for testing.
rPWM = base_pwm # Just for testing.

sock = socket.socket()
host = "172.20.10.5"    #ESP32 IP in local network
port = 80               #ESP32 Server Port    
sock.connect((host, port))
print("Connected!")
state = 0
webcam = cv2.VideoCapture(1)
rState = -1
oldrState = -1
not_seen_dir = 1
while True:
    lPWM = 300
    rPWM = 300
    microphone.record()
    bandpass.filter('left.wav', 'right.wav')
    state = speech.rec(state)
    #state = 1
    #Camera task
    if (state == 1):
        lPWM = 400
        rPWM = 400
        time1 = time.time()
        while(time.time()-time1 < 100):
            print("started camera task")
            total = []
            dirs = []
            total_slim = []
            not_seen = 0
            close = 0
            for x in range(11):
                check, frame = webcam.read()
                col = findCone.find(frame)
                if(col >-1):
                    frame[:,col,:]=np.array([0,0,255])
                cv2.imshow("Capturing", frame)
                total.append(col)
                dirs.append(img_dir.dir(col))
                if(img_dir.dir(col) == -5):
                    not_seen = not_seen + 1
                elif(img_dir.dir(col) == -4):
                    close = close + 1
                else:
                    total_slim.append(col)
            total.sort()
            print(total)
            oldrState = rState
            if(not_seen > 5):
                rState = not_seen_dir
                print("No object found")
            elif(close > 5):
                rState = -2

            #elif img_dir.dir(med) == 0:
            #    print("No object found")
            #    rState = -1 
            else:
                not_seen_dir = -1*not_seen_dir
                mid = len(total_slim) // 2
                med = (total_slim[mid] + total_slim[~mid]) / 2
                print(med)
                rState = img_dir.dir(med)
            #if(oldrState != rState):
            #    if(oldrState == 2 and not_seen>5):
            #        rState = 2
            #    elif(oldrState == 2 and rState == 1):
            #        rState = 2
            
            
            rState = str(rState)
            message = str(rState) + " " + str(lPWM) + " " + str(rPWM) 
            print("rState: ",rState)
            print("State: ",state)
            print("Left PWM: ",lPWM)
            print("Right PWM: ",rPWM)
            sock.send(message.encode())

            #time.sleep(2)

            rState = 0
            rState = str(rState)
            message = str(rState) + " " + str(lPWM) + " " + str(rPWM)  
            print("rState: ",rState)
            print("State: ",state)
            sock.send(message.encode())
            time.sleep(3)

    #Follow me task
    elif(state == 2):
        lPWM = 400
        rPWM = 400
        print("started follow me task")
        rStateArray = []
        rStateLoopTime = 0
        initLoopTime = time.time()
        for i in range(3):
                microphone_short.record()
                rStateLoopTime = time.time()-initLoopTime
                #print("rStateLoopTime after recording: ", rStateLoopTime)
                bandpass.filter('left.wav', 'right.wav')
                rStateLoopTime = time.time()-initLoopTime
                #print("rStateLoopTime after bandpass: ", rStateLoopTime)
                rState = volume.sound_dir()
                rStateLoopTime = time.time()-initLoopTime
                #print("rStateLoopTime after sound direction: ", rStateLoopTime)
                rStateArray.append(rState)

        rState = mode(rStateArray)
        rState = str(rState)
        message = str(rState) + " " + str(lPWM) + " " + str(rPWM)
        print("rState: ",rState)
        print("State: ",state)
        sock.send(message.encode())
        
        
        rState = 0
        message = str(rState) + " " + str(lPWM) + " " + str(rPWM)
        print("rState: ",rState)
        print("State: ",state)
        sock.send(message.encode())
        time.sleep(1)
        
        
        
    else:
        rState = 0
        rState = str(rState)
        message = str(rState) + " " + str(lPWM) + " " + str(rPWM)   
        print("rState: ",rState)
        print("State: ",state)
        sock.send(message.encode())