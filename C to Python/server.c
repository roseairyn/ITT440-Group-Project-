#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>

int serversocket;

void interrupt_handler(int signal) {
    printf("\nServer responded...\n");
    close(serversocket);
    exit(0);
}

void process_connection(int clientsocket) {
    char respond[200], request[100];

    //receive client request
    int recv_size = recv(clientsocket, request, 100, 0);

    if (recv_size == -1) {
        perror("Connect failed. Error");
        return;
    }

    request[recv_size] = '\0';

    //generate server respond
    sprintf(respond, "%s from server xyz", request);

    //send respond 
    if (send(clientsocket, respond, strlen(respond), 0) < 0) {
        perror("Send failed");
        return;
    }

    //close socket
    close(clientsocket);
}

int main(int argc, char *argv[]) {
    int serversocket, clientsocket;
    struct sockaddr_in server, client;

    // Create the server socket
    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serversocket == -1) {
        printf("Connection failed. Error");
        return 1;
    }

    //server host port
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    //connect server client
    if (bind(serversocket, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("Connection failed");
        return 1;
    }

    //connecting
    listen(serversocket, 5);
    printf("Connecting to server....\n");

    //declare interrupt handler
    signal(SIGINT, interrupt_handler);

    //accept connection
    while (1) {
        int c = sizeof(struct sockaddr_in);
        clientsocket = accept(serversocket, (struct sockaddr *) &client, (socklen_t *) &c);
        
        if (clientsocket < 0) {
            perror("Connection failed");
            return 1;
        }

        printf("Connection from: %s\n", inet_ntoa(client.sin_addr));

        //manage connection in separate process with fork
        int pid = fork();

        if (pid == 0) {             //child process
            close(serversocket);
            process_connection(clientsocket);
            exit(0);

        } else {                    //parent process
            close(clientsocket);
        }
    }

    close(serversocket);
    return 0;
}
