#include <stdint.h>
#include <bcm2835.h>
#include <unistd.h>
#include <stdio.h>
#include "spi.h"
#include "server.h"
#include "instructions.h"

#include <string.h>



int main()
{
    serverInit();
    instruction_init();
    int spi = SPI_init();
    printf( "[STARTING] server is starting...\n");
    int clientFd = startServer();
    while (1)
    {
		sendData(clientFd);
		sleep(0.1);
		char msg[25];
		//char test[25] = "UP";
		recieveData(msg, clientFd);
		uint8_t command = getInterpetation(msg);
		//printBits(sizeof(command), &command)
		uint8_t recieveData = transferData(command);
		printf("Sent to SPI: 0x%02x. Read back from SPI: 0x%02x. \n", command, recieveData);
		//sleep(0.1);
				
		
		if (strcmp(msg, getDisconnectMessage()) == 0)
		{
			closeSPI();
			printf("Server shutting down...\n");
			// closing the connected socket
			closeClient();
			// closing the listening socket
			shutDown();
			return 0;
		}
    }
	return 0;
}
