#include "server.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string.h>
struct sockaddr_in address;
void serverInit()
{
	// Creating socket file descriptor
    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(serverFd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

void sendData(int socket)
{
	const char *test = "[List of LIDAR-Data]\n";
    send(socket, test, strlen(test), 0);
    //printf("Sending LIDAR-data...\n");
    //sleep(0.1);
}

void recieveData(char *message, int socket)
{
	char buffer[512] = {0};
    valRead = read(socket, buffer, 512);

    char msg[25];
    for (int i = 0; i < 512; i++)
    {
        for (char const *p = VALID_LETTERS; *p; ++p)
        {
            char c = *p;
            if (c == +buffer[i])
            {
                strncat(msg, &c, 1);
            }
        }
    }
    printf("Recieved msg: {%s}\n", msg);

    for (int i = 0; i < 25; ++i)
    {
        message[i] = msg[i];
    }
    memset(msg, 0, 255);
}

int startServer()
{
	int connected = 1;
    printf("[LISTENING] Server is listening on {SERVER}\n");
	int addrLen = sizeof(address);
    while (connected)
    {
		
        if ((newSocket = accept(serverFd, (struct sockaddr *)&address, (socklen_t *)&addrLen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        return newSocket;
    }
    return -1;
}

const char*getDisconnectMessage()
{
    return DISCONNECT_MESSAGE;
}

int getServerFd()
{
    return serverFd;
}

void closeClient()
{
    close(newSocket);
}

void shutDown()
{
    shutdown(serverFd, SHUT_RDWR);
}
/*
int main()
{	serverInit();
    printf("[STARTING] server is starting...\n");
    int clientFd = startServer();
    while (1)
        {
            sendData(clientFd);
            sleep(0.1);
            char msg[25];
            recieveData(msg, clientFd);

            if (strcmp(msg, getDisconnectMessage()) == 0)
            {
                printf("Server shutting down...\n");
                // closing the connected socket
                close(clientFd);
                // closing the listening socket
                shutdown(getServerFd(), SHUT_RDWR);
                return 0;
            }
        }
    return 0;
}*/
