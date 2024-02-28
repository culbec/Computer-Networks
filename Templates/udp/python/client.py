import socket

SERVER_ADDRESS = '127.0.0.1'
SERVER_PORT = 1234

# initialization of the client socket
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

string = 'hello'
string = string.encode()

# sending data to the server
client.sendto(string, (SERVER_ADDRESS, SERVER_PORT))

# receiving data from the server
data, addr = client.recvfrom(10)
print(data.decode(), " received from: ", addr)