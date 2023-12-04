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
import findCone
import cv2 
import img_dir

base_pwm = 300
lPWM = base_pwm # Just for testing.
rPWM = base_pwm # Just for testing.

sock = socket.socket()
host = "172.20.10.5"    #ESP32 IP in local network
port = 80               #ESP32 Server Port    
sock.connect((host, port))
print("Connected!")
state = 0
webcam = cv2.VideoCapture(1)
while True:
    microphone.record()
    bandpass.filter('left.wav', 'right.wav')
    state = speech.rec(state)
    #Camera task
    if (state == 1):
        time1 = time.time()
        while(time.time()-time1 < 10):
            print("started camera task")
            total = []
            for x in range(10):
                check, frame = webcam.read()
                col = findCone.find(frame)
                total.append(col)
            total.sort()
            mid = len(total) // 2
            med = (total[mid] + total[~mid]) / 2
            print(med)
            t = 100
            if img_dir.dir(med) == 0:
                print("No object found")
                rState = -1 
            else:
                rState = img_dir.dir(med)
            rState = str(rState)
            message = str(rState) + " " + str(lPWM) + " " + str(rPWM) + " " + str(t)  
            print("rState: ",rState)
            print("State: ",state)
            sock.send(message.encode())

            #time.sleep(2)

            # rState = 0
            # rState = str(rState)
            # message = str(rState) + " " + str(lPWM) + " " + str(rPWM) + " " + str(t)  
            # print("rState: ",rState)
            # print("State: ",state)
            # sock.send(message.encode())

    #Follow me task
    elif(state == 2):
        print("started follow me task")
        rState = volume.sound_dir()
        rState = str(rState)
        t = 500
        message = str(rState) + " " + str(lPWM) + " " + str(rPWM) + " " + str(t)
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