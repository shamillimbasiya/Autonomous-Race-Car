#include <iostream>
#include <string.h>

#ifndef instruction_h
#define instruction_h

using namespace std;

class Instruction
{
	public:
		Instruction();
		uint8_t getInterpetation(string key);
		uint8_t getInstruction(int turnSegment);
		
		const uint8_t getAccelerate();
		const uint8_t getStop();
		
		uint8_t setAccelerate(uint8_t command);
		uint8_t setConstSpeed(uint8_t command);
		uint8_t setBrake(uint8_t command);
	
	private:
		uint8_t instruction;
		const uint8_t STD_INSTRUCTION = 0b01101111;
		const uint8_t ACCELERATE = 0b00101111;
		const uint8_t BRAKE = 0b01001111;
		const uint8_t STOP = 0b00001111;

		const uint8_t MAX_RIGHT = 0b01100000;
		const uint8_t MAX_LEFT = 0b01111111;
		const uint8_t ONE = 0b00000001;
		
		const string UP = "UP";
		const string DOWN = "DOWN";
		const string LEFT = "LEFT";
		const string RIGHT = "RIGHT";
		const string SPACE = "SPACE";
		
};

#endif
