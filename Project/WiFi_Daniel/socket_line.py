import socket   
import keyboard            

sock = socket.socket()

host = "172.20.10.5" #ESP32 IP in local network
port = 80               #ESP32 Server Port    
#sock.bind((host, port))

sock.connect((host, port))

data = ""       

d = ""

while True:
    d = sock.recv(1)
    data += d.decode()
    if (d.decode() == "\n"):
        print(data)
    if keyboard.is_pressed("q"):
        break

   
"""""
while len(data) < len(message):
    d = sock.recv(1)
    data += d.decode()

print(data)

print(data)
"""

sock.close()
