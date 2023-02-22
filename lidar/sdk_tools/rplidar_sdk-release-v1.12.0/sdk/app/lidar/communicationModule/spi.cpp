#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "spi.h"


/*
 * Constructor for the spi object, initializes the spi and sets the spi
 * settings.
 */ 
SPI::SPI()
{
	if(!bcm2835_init())
	{
		printf("bcm2835_init failed. Are you running as root??\n");
	}
		
	if(!bcm2835_spi_begin())
	{
		printf("bcm2835_init failed. Are you running as root??\n");
	}
		
	//Set SPI SETTINGS
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_4096);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
}

/*
 * Transfers data to the AVR
 */ 
uint8_t SPI::transferData(uint8_t sendData)
{
	uint8_t readData = bcm2835_spi_transfer(sendData);
	return readData;
}

/*
 * Closes the spi connection
 */ 
void SPI::closeSPI()
{
	bcm2835_spi_end();
	bcm2835_close();
}

/*
int main()
{		
	
	SPI spi = SPI();

	//Send byte to the slave and simultaneously read a byte back from the slave
	//If you tie MISO to MOSI, you should read back what was sent
	//0b00101001
	uint8_t sendData = 0x23; //uint_8 = unsigned integer of size 8, 0x = hexadecimal
	
	while(1)
	{
		uint8_t readData = spi.transferData(sendData);
		printf("Sent to SPI: 0x%02X. Read back from SPI: 0x%02x. \n", sendData, readData);
		usleep(1000);
		
	}
	spi.closeSPI();
	return 0;
}*/
