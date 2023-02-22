#ifndef spi_h
#define spi_h

class SPI
{
	public:
		SPI();
		uint8_t transferData(uint8_t sendData);
		void closeSPI();
};

#endif
