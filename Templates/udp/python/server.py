import socket

SERVER_ADDRESS = "127.0.0.1"
SERVER_PORT = 1234

# initializing a datagram socket
server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# binding the server
server.bind((SERVER_ADDRESS, SERVER_PORT))

# receiving data from a client
data, addr = server.recvfrom(10)
print(data.decode(), " received from ", addr)

# sending data back to the client
string = 'world'
string = string.encode()

# addr = address of the sender
server.sendto(string, addr)