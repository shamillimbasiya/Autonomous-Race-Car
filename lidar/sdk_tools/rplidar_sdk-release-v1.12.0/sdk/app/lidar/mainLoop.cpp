#include <stdint.h>
#include <bitset>
#include <unistd.h>

#include "mainLoop.h"

using namespace std;



MainLoop::MainLoop(string mode, string prevCone, int lap, string data, int8_t recieveData_, bool isPrevConePairSet)
{
	driveMode = mode;
	prevConePair = prevCone;
	prevPairIsSet = isPrevConePairSet;
	laps = lap;
	sendData = data;
	recieveData = recieveData_;
	
	// ----- INIT Server -----
	server.startServer();
}

/*
 * Format the string of data that is sent to the client
 */
void MainLoop::dataToClient(vector<Cone> conePair, int speed_)
{
	/*
	 * input_string: "deg1, dist1, type1, 
	 * 				  deg2, dist2, type2, 
	 * 				  hastighet, portdistance, avstånd till mitten,
	 *				  vinkel till port, varv"
	*/
	string speed = to_string(speed_);
	string lap = to_string(laps);
	if (conePair.size() == 2) 
	{
		string leftCone = conePair[0].stringToPrint();
		string rightCone = conePair[1].stringToPrint();
		string distToPort = to_string(carReg.distToConePair(conePair));
		string distToCenter = to_string(carReg.distToCenter(conePair));
		string angleToPort = to_string(carReg.calcAngleRegulator(conePair));
		
		sendData = leftCone + ", " + rightCone + ", " + speed + ", " + 
						distToPort + ", " + distToCenter + ", " + angleToPort + 
						", " + lap + ", " + prevConePair;
		//cout << "Data sent to client: " << dataToComputer << endl;
	}
	
	else 
	{
		//sendData = "0, 0, 0, 0, 0, 0, " + speed + ", 0, 0, 0, " + lap + ", " + prevConePair;
		sendData = "0, 0, 0, 0, 0, 0, " + speed + ", 0, 0, 0, 0, 0";
	}

}

/*
 * Returns the right command to the AVR for gas and turn actions
 * when in Manual mode. Based on msg from client that is an ev. keypress.
 */
uint8_t MainLoop::manualDrive(string msg)
{
	uint8_t command = instruction.getInterpetation(msg);
	return command;
}

/*
 * Returns the calculated command to the AVR for gas and turn actions 
 * when in AUTO-DRIVE mode.
 */
uint8_t MainLoop::autoDrive(float lidarData[])
{
	// ----- Find nearest ConePair -----
	vector<Cone> allCones = obj.findCones(lidarData);
	vector<Cone> conePair = obj.nearestConePair(allCones);

	dataToClient(conePair, recieveData);
	
	int turnSegment = 20; 
	if (conePair.size() == 2) {	
		// ----- Car Regulation -----
		turnSegment = carReg.calcTurn(conePair, prevConePair, prevPairIsSet, laps);
		//cout << "prev cone pair    " << prevConePair << endl;
		
		if (prevConePair  == "START" && laps >= 4 && carReg.distToConePair(conePair) < 200)
		{
			sleep(1);
			const uint8_t stop = instruction.getStop();
			recieveData = spi.transferData(stop);
			usleep(100000);
		}
	}
	else if (prevConePair == "LEFT")
	{
		turnSegment = 30;
	}
	else if (prevConePair == "RIGHT")
	{
		turnSegment = 0;
	}
	
	uint8_t command = instruction.getInstruction(turnSegment);
	
	
	// ----- Set Car Speed -----
	/*int speed = recieveData;
	if (speed >= 5 && speed <= 8)
	{
		command = instruction.setConstSpeed(command);
	}
	
	else if (speed < 5)
	{
		command = instruction.setAccelerate(command);
	}
	
	else 
	{
		command = instruction.setBrake(command);
	}*/
	
	return command;
}

/*
 * Check msg from client to see if we have changed the DRIVE MODE. 
 */
void MainLoop::checkDriveMode(string msg)
{	
	if (msg == "MANUAL")
	{
		driveMode = MANUAL_DRIVE;
		
		// Stop the car if changed from AUTO-mode to Manual-mode
		const uint8_t stop = instruction.getStop();
		recieveData = spi.transferData(stop);
		usleep(100000);
		
	}
	
	else if (msg == "AUTO")
	{
		driveMode = AUTO_DRIVE;
		
		// Set const speed for AUTO-Driving
		const uint8_t accelerate = instruction.getAccelerate();
		for (int i = 0; i < 15; i++)
		{
			recieveData = spi.transferData(accelerate);
			usleep(100000);
		}
		
	}
}

/*
 * Check if the we have closed the GUI on the client.
 * If so, close the SPI and server.
 */
bool MainLoop::isGuiClosed(string msg)
{
	if (msg == server.getDisconnectMessage())
	{
		spi.closeSPI();
		printf("Server shutting down...\n");
		// closing the connected socket
		server.closeClient();
		// closing the listening socket
		server.shutDown();
		return true;
	}
	return false;
}

int MainLoop::run(float lidarData[])
{
	// ----- Send Lidar data to Client -----
	server.sendData(sendData);
	usleep(100000);
	
	// ----- Recieve msg from Client -----
	string msg = server.recieveData();
	
	// ----- Update the drive mode -----
	checkDriveMode(msg);
	
	// ----- Check if GUI has been closed. If so, stop the lidar -----
	if (isGuiClosed(msg))
	{
		return -1;
	}

	
	//printf("driveMode: %s\n", driveMode.c_str());
	//printf("Message: %s\n", msg.c_str());
	
	// ----- Fetch either Manual keypress command or AUTO-drive command -----
	uint8_t command;
	if (driveMode == MANUAL_DRIVE)
	{
		command = manualDrive(msg);
	}
	
	else
	{
		command = autoDrive(lidarData);
	}
	
	
	//bitset<8> x(command);
	//cout << "Sending to SPI: " << x << endl;
	
	// ----- Send/Recieve to SPI -----
	recieveData = spi.transferData(command);
		
	return 1;
}

