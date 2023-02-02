#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int clientsocket;
    struct sockaddr_in server;
    char request[200], respond[100];

    //create socket
    clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientsocket == -1) {
        printf("Connection Failed!");
        return 1;
    }
    printf("Connecting to server...\n");

    //server host port
    server.sin_addr.s_addr = inet_addr("192.168.56.101");
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    //connect server client
    if (connect(clientsocket, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("Connection failed. Error");
        return 1;
    }
    printf("Connected to server\n");

    //ask  user to enter any input
    printf("Enter any input: ");
    fgets(respond, 100, stdin);

    //send the request to the server
    if (send(clientsocket, respond, strlen(respond), 0) < 0) {
        perror("Request Failed!");
        return 1;
    }

    //receive the respond from the server
    int recv_size = recv(clientsocket, request, 200, 0);
    if (recv_size == -1) {
        perror("Respond Failed!");
        return 1;
    }
    request[recv_size] = '\0';

    //generate the respond
    printf("\nRespond from server: %s", request);

    //close socket
    close(clientsocket);
    return 0;
}
