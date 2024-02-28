import socket

TCP_IP = "127.0.0.1"
TCP_PORT = 1234

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((TCP_IP, TCP_PORT))

string = str(input("Dati sir: "))
strLen = str(len(string))
client.send(strLen.encode())
client.send(string.encode())

data = client.recv(len(string))
print(data)
client.close()