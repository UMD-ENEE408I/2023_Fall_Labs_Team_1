import socket               

sock = socket.socket()

host = "172.20.10.3" #ESP32 IP in local network
port = 80               #ESP32 Server Port    
#sock.bind((host, port))

sock.connect((host, port))

rState = 0
lPWM = 100
rPWM = 50

message = str(rState) + " " + str(lPWM) + " " + str(rPWM)   # Reference; https://www.shecodes.io/athena/2142-converting-an-integer-to-string-in-python.
sock.send(message.encode())

data = ""       

while len(data) < len(message):
    d = sock.recv(1)
    data += d.decode()

print(data)

sock.close()
