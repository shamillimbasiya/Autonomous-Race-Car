#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>
void instruction_init();
uint8_t getInterpetation(char key[]);

uint8_t getInstruction(int turnDegree);

uint8_t instruction;
//void printBits(size_t const size, void const * const ptr);
static const uint8_t STD_INSTRUCTION = 0b01101111;
static const uint8_t ACCELERATE = 0b00101111;
static const uint8_t BRAKE = 0b01001111;
static const uint8_t STOP = 0b00001111;

static const uint8_t MAX_RIGHT = 0b01100000;
static const uint8_t MAX_LEFT = 0b01111111;
static const uint8_t ONE = 0b00000001;

#endif
