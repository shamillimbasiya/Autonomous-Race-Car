#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>

#ifndef server_h
#define server_h

using namespace std;

class Server
{
	public:
		Server();
		void sendData(string dataToClient);
		string recieveData();
		void startServer();
		const string getDisconnectMessage();
		void closeClient();
		void shutDown();
	
	private:
		int serverFd;
		int newSocket;
		int valRead;
		const int opt = 1; 
		const int PORT = 4040;

		struct sockaddr_in address;
		//const int addrLen = sizeof(address); //changed to const

		const string DISCONNECT_MESSAGE = "!DISCONNECT";
		const string VALID_LETTERS = "[]!_ABCDEFGHIJKLMNOPQRSTUVWQYZÅÄÖ";
		const char *AUTO_MSG = "AUTONOMT";
		const char *MANUAL_MSG = "MANUELL";
		const char *NO_RECIEVED_MSG = "[NO_MESSAGE_RECIEVED]";
};

#endif
