#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
void serverInit();
void sendData(int socket); //removed static
void recieveData(char* messeage, int socket); //send message with &??
int startServer();
const char* getDisconnectMessage();
int getServerFd();
void closeClient();
void shutDown();

int serverFd;
int newSocket;
int valRead;
static const int opt = 1; //changed to const
static const int PORT = 3030;

//struct sockaddr_in address;
//const int addrLen = sizeof(address); //changed to const

static int AUTO_DRIVE = 0;

static const char *DISCONNECT_MESSAGE = "!DISCONNECT";
static const char *VALID_LETTERS = "[]!_ABCDEFGHIJKLMNOPQRSTUVWQYZÅÄÖ";
static const char *AUTO_MSG = "AUTONOMT";
static const char *MANUAL_MSG = "MANUELL";
static const char *NO_RECIEVED_MSG = "[NO_MESSAGE_RECIEVED]";

#endif
