#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#define MEM_SIZE 4096
#define REGISTER_COUNT 16
#define STACK_SIZE 16
#define FONT_SET_START_ADDRESS 0x050

typedef struct
{
    uint8_t ram[MEM_SIZE];
    uint16_t I;
    uint8_t pc;
    uint16_t stack[STACK_SIZE];
    uint8_t sp;
    uint8_t dtimer;
    uint8_t stimer;
    uint8_t V[REGISTER_COUNT];

} chip8;

extern const uint8_t font[80];

void chip8_init(chip8 *cpu);

void chip8_exec(chip8 *cpu,uint16_t opcode);

#endif