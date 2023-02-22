#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
//#include "spi.h"

int SPI_init()
{
	if(!bcm2835_init())
	{
		printf("bcm2835_init failed. Are you running as root??\n");
		return -1;
	}
		
	if(!bcm2835_spi_begin())
	{
		printf("bcm2835_init failed. Are you running as root??\n");
		return -1;
	}
		
	//Set SPI SETTINGS
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_4096); //1,5625MHz
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	return 0;
}

uint8_t transferData(uint8_t sendData)
{
	uint8_t readData = bcm2835_spi_transfer(sendData);
	return readData;
}

void closeSPI()
{
	bcm2835_spi_end();
	bcm2835_close();
}


int main()
{		

	int spi = SPI_init();
	if(spi < 0)
	{
		perror("SPI_init failed");
	}
	//Send byte to the slave and simultaneously read a byte back from the slave
	//If you tie MISO to MOSI, you should read back what was sent
	//0b00101001
	uint8_t sendData = 0x23; //uint_8 = unsigned integer of size 8, 0x = hexadecimal
	
	while(1)
	{
		uint8_t readData = transferData(sendData);
		printf("Sent to SPI: 0x%02X. Read back from SPI: 0x%02x. \n", sendData, readData);
		usleep(1000);
		
	}
	closeSPI();
	return 0;
}
