#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#define MEM_SIZE 4096
#define GPVR_NUMBER 16
#define STACK_SIZE 16

typedef struct
{
    uint8_t ram[MEM_SIZE];
    uint16_t I;
    uint8_t pc;
    uint16_t stack[STACK_SIZE];
    uint8_t sp;
    uint8_t dtimer;
    uint8_t stimer;
    uint8_t gpvr[GPVR_NUMBER];

} chip8;

extern const uint8_t font[80];

void chip8_init(chip8 *cpu);

#endif