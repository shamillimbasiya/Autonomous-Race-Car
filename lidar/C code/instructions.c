#include <stdint.h>
#include "instructions.h"
#include <string.h>


void instruction_init()
{
	instruction = STD_INSTRUCTION;
} 

uint8_t getInterpetation(char key[]){

	if(!strcmp(key,"UP"))
	{
		instruction = ACCELERATE;
	}
	
	else if(!strcmp(key,"DOWN"))
	{
		instruction = BRAKE;
	}
	
	else if(!strcmp(key,"LEFT"))
	{
		if (MAX_RIGHT <= instruction && instruction < MAX_LEFT)
		{
			instruction = instruction + ONE;
		}
	}
	
	else if(!strcmp(key,"RIGHT"))
	{
		if (instruction > MAX_RIGHT)
		{
			instruction = instruction - ONE;
		}
	}
	
	else if(!strcmp(key,"SPACE"))
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
void printBits(size_t const size, void const * const ptr)
{
	unsigned char *b = (unsigned char*) ptr;
	unsigned char byte;
	int i,j;
	for(i = size - 1; i >= 0; i--)
	{
		for(j = 7; j >= 0; j--)
		{
			byte = (b[i] >> & 1) & 1;
			printf("%u, byte");
		}
	}
	puts("");
}*/
