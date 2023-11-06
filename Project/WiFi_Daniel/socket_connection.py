import socket               

sock = socket.socket()

host = "172.20.10.5" #ESP32 IP in local network
port = 80               #ESP32 Server Port    
#sock.bind((host, port))

sock.connect((host, port))

message = "Hello World"
sock.send(message.encode())

data = ""       

while len(data) < len(message):
    d = sock.recv(1)
    data += d.decode()

print(data)

sock.close()
