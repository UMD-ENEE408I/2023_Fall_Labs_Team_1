# Authors: Daniel McLawhorn and Kent Chesley
# Date: 11/2023
# (Original code by Daniel from Project/Motor_Control_From_Speech/WiFi_Audio was
# adated by Kent to implement the robot's finite state machine.)


import socket
from sys import base_prefix   
import microphone
import speech

base_pwm = 550
lPWM = base_pwm # Just for testing.
rPWM = 0.5*base_pwm # Just for testing.

sock = socket.socket()
host = "192.168.1.158"    #ESP32 IP in local network
port = 80               #ESP32 Server Port    
sock.connect((host, port))
print("Connected!")
state = 0
while True:
    microphone.record()

    state = str(speech.rec(state))

    rState = state  # For testing, just set robot state to high-level state.
    message = str(rState) + " " + str(lPWM) + " " + str(rPWM)   

    sock.send(message.encode())
