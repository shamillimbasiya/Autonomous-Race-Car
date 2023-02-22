#include <iostream>
#include <string.h>

#include "navigation/objDetection.h"
#include "navigation/carRegulation.h"
#include "communicationModule/spi.h"
#include "communicationModule/instruction.h"
#include "communicationModule/server.h"

#ifndef mainLoop_h
#define mainLoop_h

using namespace std;

class MainLoop 
{
	public:
		MainLoop(string mode, string prevCone, int lap, string data, int8_t recieveData, bool isPrevConePairSet);
		int run(float lidarData[]);
		
		uint8_t manualDrive(string msg);
		uint8_t autoDrive(float lidarData[]);
		void checkDriveMode(string msg);
		bool isGuiClosed(string msg);
		void dataToClient(vector<Cone> conePair, int speed);
	
	private:
		Server server{};
		Instruction instruction{};
		SPI spi{};
		
		ObjDetection obj{};
		CarRegulation carReg{};
		
		const string MANUAL_DRIVE = "MANUAL";
		const string AUTO_DRIVE = "AUTO";
		string driveMode;
		
		string prevConePair;
		bool prevPairIsSet;
		
		int laps;
		string sendData;
		uint8_t recieveData;
};

#endif
