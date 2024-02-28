import socket

SERVER_ADDRESS = '127.0.0.1'
SERVER_PORT = 1234

# Server template under TCP protocol

# initialization of the socket
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# binding the socket to the address:port combinations
server.bind((SERVER_ADDRESS, SERVER_PORT))

# listening on a maximum backlog of 5
server.listen(5)

while True:
    # accepting a client
    client, addr = server.accept()

    # receiving maximum 10 bytes from the clients
    data = client.recv(10)
    data = data.decode()
    print(data)

    # closing the client
    client.close()