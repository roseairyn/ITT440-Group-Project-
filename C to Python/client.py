import socket

host = "192.168.56.101"
port = 8080

print("Connecting to server...")

#create a TCP/IP socket
clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    #connect the socket to the server
    clientsocket.connect((host, port))
    print("Connected to server!")

    #ask  user to enter any input
    message = input("Enter any input: ")

    #send the request to the server
    clientsocket.sendall(message.encode())

    #receive the respond from the server
    respond = clientsocket.recv(1024)

    #generate the respond
    print("\nRespond from server:", respond.decode())

    #close client socket
    clientsocket.close()
except socket.error as e:
    print("Connection Failed!")
