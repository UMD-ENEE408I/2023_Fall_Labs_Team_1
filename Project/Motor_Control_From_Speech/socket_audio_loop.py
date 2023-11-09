import socket   
import microphone
import speech

sock = socket.socket()
host = "172.20.10.5"    #ESP32 IP in local network
port = 80               #ESP32 Server Port    
sock.connect((host, port))
print("Connected!")
state = 0
while True:
    microphone.record()
    state = str(speech.rec(state))

    sock.send(state.encode())
