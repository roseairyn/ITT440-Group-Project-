import socket
def server():
    host = "192.168.56.101"
    port = 8080

    serversocket = socket.socket()
    
    serversocket.bind((host,port))

    serversocket.listen(2)
    conn, address = serversocket.accept()
    print("Connection from: " + str(address))
    while True:
        request = conn.recv(1024)
        respond = request + b" from server xyz"
        conn.sendall(respond)
        print("Server responded")
        break 
if __name__ == '__main__':
    server()
