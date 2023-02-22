#include "server.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#include <string.h>

using namespace std;


/*
 * The server obeject constructor, initializes the server
 */ 
Server::Server()
{
	// Creating socket file descriptor
    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 
    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 
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

/*
 * Sends data to client
 */ 
void Server::sendData(string dataToClient)
{
	//const char *test = "[List of LIDAR-Data]\n";
    string toClient = dataToClient + "\n";
    const char *test = dataToClient.c_str();
    send(newSocket, test, strlen(test), 0);
}

/*
 * Recieves data from client
 */ 
string Server::recieveData()
{
	char buffer[512] = {0};
    valRead = read(newSocket, buffer, 512);
    
    string msg = "";
    for (int i = 0; i < 512; i++)
    {
		char b = +buffer[i];
		if (VALID_LETTERS.find(b) != string::npos)
		{
			msg.push_back(b);
		}
		
    }
    //printf("\nRecieved msg: {%s}\n", msg.c_str());
    return msg;
}

/*
 * Starts the server and waits for a connection
 */ 
void Server::startServer()
{
	int connected = 1;
    printf("[STARTING] server is starting...\n");
    printf("[LISTENING] Server is listening on {SERVER}\n");
	int addrLen = sizeof(address);
    while (connected)
    {
        if ((newSocket = accept(serverFd, (struct sockaddr *)&address, (socklen_t *)&addrLen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        return;
    }
}

/*
 * Returns the disconnect message
 */ 
const string Server::getDisconnectMessage()
{
    return DISCONNECT_MESSAGE;
}

/*
 * Closes the connection to the client
 */ 
void Server::closeClient()
{
    close(newSocket);
}

/*
 * Shutdowns the server
 */ 
void Server::shutDown()
{
    shutdown(serverFd, SHUT_RDWR);
}

/*
int main()
{	Server server = Server();
    printf("[STARTING] server is starting...\n");
    server.startServer();
    
    while (1)
        {
            server.sendData();
            usleep(100000);
            string msg = server.recieveData();
            if (msg == server.getDisconnectMessage())
            {
                printf("Server shutting down...\n");
                // closing the connected socket
                server.closeClient();
                // closing the listening socket
                server.shutDown();
                return 0;
            }
        }
    return 0;
}*/

