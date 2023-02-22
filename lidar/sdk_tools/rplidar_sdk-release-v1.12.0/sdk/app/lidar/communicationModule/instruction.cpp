#include <stdint.h>
#include "instruction.h"
#include <string.h>
#include <iostream>
#include <bitset>


using namespace std;

/*
 * Constructor of the instruction object
 */ 
Instruction::Instruction()
{
	instruction = STD_INSTRUCTION;
} 

/*
 * Returns a instruction based on the key press
 */ 
uint8_t Instruction::getInterpetation(string key){
	if(key == UP)
	{
		instruction = ACCELERATE;
	}
	
	else if(key == DOWN)
	{
		instruction = BRAKE;
	}
	
	else if(key == LEFT)
	{
		if (MAX_RIGHT <= instruction && instruction < MAX_LEFT)
		{
			instruction = instruction + ONE;
		}
	}
	
	else if(key == RIGHT)
	{
		if (instruction > MAX_RIGHT)
		{
			instruction = instruction - ONE;
		}
	}
	
	else if(key == SPACE)
	{
		instruction = STOP;
	}
	
	else
	{
		if (instruction > STD_INSTRUCTION) 
		{
			instruction = instruction - ONE;
		}
		
		else if (MAX_RIGHT <= instruction && instruction < STD_INSTRUCTION)
		{
			instruction = instruction + ONE;
		}
		
		else
		{
			instruction = STD_INSTRUCTION;
		}
	}
	return instruction;
}

/*
 * Builds the instruction based on the turn segment value
 */ 
uint8_t Instruction::getInstruction(int turnSegment)
{
	uint8_t turnSegmentBytes = turnSegment;
	
	//bitset<8> x(turnSegmentBytes);
	//cout << "turnSegmentBytes: " << x << endl;
	
	uint8_t instruction = 0b10000000 | turnSegmentBytes;
	
	//bitset<8> y(instruction);
	//cout << "Instruction: " << y << endl;
	
	return instruction;
}

/*
 * Returns the accelerate instruction
 */ 
const uint8_t Instruction::getAccelerate()
{
	return ACCELERATE;
}

/*
 * Returns the instruction to stop
 */ 
const uint8_t Instruction::getStop()
{
	return STOP;
}

uint8_t Instruction::setAccelerate(uint8_t command)
{
	return 0b10100000 | command;
}

uint8_t Instruction::setConstSpeed(uint8_t command)
{
	return 0b11100000 | command;
}

uint8_t Instruction::setBrake(uint8_t command)
{
	return 0b11000000 | command;
}

