import socket

SERVER_ADDRESS = '127.0.0.1'
SERVER_PORT = 1234

# Client Template under TCP protocol

# initialization of the client socket
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# connecting the client to the server
client.connect((SERVER_ADDRESS, SERVER_PORT))

data = input("mesaj: ")

# encoding data into bytes for sending
data = data.encode()

# sending data to the server
client.send(data)
