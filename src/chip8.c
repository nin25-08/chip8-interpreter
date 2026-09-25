#include "chip8.h"
#include "display.h"
#include <string.h>
#include <stdlib.h>
static void chip8_push(chip8 *cpu, uint16_t address)
{
    if (cpu->sp >= STACK_SIZE)
    {
        printf("STACK OVERFLOW!");
        return;
    }
    cpu->stack[cpu->sp] = address;
    cpu->sp++;
}

static uint16_t chip8_pop(chip8 *cpu)
{
    if (cpu->sp == 0)
    {
        printf("STACK UNDERFLOW!");
        return;
    }
    cpu->sp--;
    return cpu->stack[cpu->sp];
}

const uint8_t font[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip8_init(chip8 *cpu)
{

    memset(cpu, 0, sizeof(*cpu)); // initializes cpumem to 0

    cpu->pc = 0x200; // programs start from 0x200 address

    memcpy(&cpu->ram[FONT_SET_START_ADDRESS], font, sizeof(font)); // loads font into memory
}

void chip8_exec(chip8 *cpu, display *disp, uint16_t opcode)
{
    // bitwise masks
    uint8_t optype = (opcode & 0xF000) >> 12; // first nibble
    uint8_t x = (opcode & 0x0F00) >> 8;       // second nibble
    uint8_t y = (opcode & 0x00F0) >> 4;       // third nibble
    uint8_t z = (opcode & 0x000F);            // last nibble
    uint16_t NNN = (opcode & 0x0FFF);
    uint8_t NN = (opcode & 0x00FF);

    switch (optype)
    {
    case 0x0000:
        // clears display
        if (opcode == 0x00E0)
        {
            memset(disp, 0, sizeof(*disp));
            cpu->pc += 2;
        }
        // returning from a subroutine is done with 00EE
        if (opcode == 0x00EE)
        {
            cpu->pc = chip8_pop(cpu) + 2;
        }

        break;

        // this instruction should simply set PC to NNN
    case 0x1000:
        cpu->pc = NNN;
        break;

        // 2NNN calls the subroutine at memory location NNN
    case 0x2000:
        chip8_push(cpu, cpu->pc);
        cpu->pc = NNN;
        break;

        // 3XNN will skip one instruction if the value in VX is equal to NN
    case 0x3000:
        if (cpu->V[x] == NN)
        {
            cpu->pc += 4;
        }
        else
        {
            cpu->pc += 2;
        }
        break;

        // 4XNN will skip one instruction if the value in VX is not equal to NN.
    case 0x4000:
        if (cpu->V[x] != NN)
        {
            cpu->pc += 4;
        }
        else
        {
            cpu->pc += 2;
        }
        break;

        // 5XY0 skips if the values in VX and VY are equal,
    case 0x5000:
        if (cpu->V[x] == cpu->V[y])
        {
            cpu->pc += 4;
        }
        else
        {
            cpu->pc += 2;
        }
        break;

        // 6XNN set the register VX to the value NN.
    case 0x6000:
        cpu->V[x] = NN;
        cpu->pc += 2;
        break;

        // 7XNN Add the value NN to VX.
    case 0x7000:

        cpu->V[x] += NN;
        cpu->pc += 2;

        break;

    case 0x8000:
        switch (z)
        {
            // 8XY0 VX is set to the value of VY.
        case 0x0:
            cpu->V[x] = cpu->V[y];
            cpu->pc += 2;
            break;
            // 8XY1 VX is set to the bitwise/binary logical disjunction (OR) of VX and VY. VY is not affected.
        case 0x1:
            cpu->V[x] = cpu->V[x] | cpu->V[y];
            cpu->pc += 2;
            break;
            // 8XY2 VX is set to the bitwise/binary logical conjunction (AND) of VX and VY. VY is not affected.
        case 0x2:
            cpu->V[x] = cpu->V[x] & cpu->V[y];
            cpu->pc += 2;
            break;
            // 8XY3 VX is set to the bitwise/binary exclusive OR (XOR) of VX and VY. VY is not affected.
        case 0x3:
            cpu->V[x] = cpu->V[x] ^ cpu->V[y];
            cpu->pc += 2;
            break;
            // 8XY4 VX is set to the value of VX plus the value of VY. VY is not affected.
        case 0x4: {
            uint16_t sum = cpu->V[x] + cpu->V[y];
            cpu->V[0xF] = (sum > 255) ? 1 : 0;
            cpu->V[x] = sum & 0xFF;
            cpu->pc += 2;
            break;
        }
        case 0x5: {
            cpu->V[0xF] = (cpu->V[x] >= cpu->V[y]) ? 1 : 0;
            cpu->V[x] = cpu->V[x] - cpu->V[y];
            cpu->pc += 2;
            break;
        }
        case 0x6: {
            cpu->V[0xF] = cpu->V[y] & 0x1;
            cpu->V[x] = cpu->V[y] >> 1;
            cpu->pc += 2;
            break;
        }
        case 0x7: {
            cpu->V[0xF] = (cpu->V[y] >= cpu->V[x]) ? 1 : 0;
            cpu->V[x] = cpu->V[y] - cpu->V[x];
            cpu->pc += 2;
            break;
        }
        case 0xE: {
            cpu->V[0xF] = (cpu->V[y] >> 7) & 0x1;
            cpu->V[x] = cpu->V[y] << 1;
            cpu->pc += 2;
            break;
        }

        break;
        case 0x9000:
        if (cpu->V[x] != cpu->V[y])
        {
            cpu->pc += 4;
        }
        else
        {
            cpu->pc += 2;
        }
        break;
        // 0XANNN This sets the index register I to the value NNN.
    case 0xA000:
        cpu->I = NNN;
        cpu->pc += 2;
        break;
        // 0xBNNN this instruction jumped to the address NNN plus the value in the register V0
    case 0xB000:
        cpu->pc = NNN + cpu->V[0];
        break;
        // 0xCXNN This instruction generates a random number, binary ANDs it with the value NN, and puts the result in VX.
    case 0xC000:
        cpu->V[x] = (rand() % 256) & NN;
        cpu->pc += 2;
        break;
        /*0xDXYZ It will draw an N pixels tall sprite from the memory location
         that the I index register is holding to the screen, at the horizontal X coordinate in VX
         and the Y coordinate in VY*/
    case 0xD000:;
        uint8_t x_start = cpu->V[x] % 64;
        uint8_t y_start = cpu->V[y] % 32;
        uint8_t height = z;

        cpu->V[0xF] = 0x0;

        for (uint8_t row = 0; row < height; row++)
        {
            uint8_t py = y_start + row;
            if (py >= 32)
                break;

            uint8_t sprite_data = cpu->ram[cpu->I + row];
            uint8_t mask = 0x80;

            for (int sig = 0; sig < 8; sig++)
            {
                uint8_t px = x_start + sig;
                if (px >= 64)
                    break;

                bool toEval = sprite_data & mask;
                if (toEval)
                {
                    if (disp->pixels[py][px])
                    {
                        cpu->V[0xF] = 1;
                    }
                    disp->pixels[py][px] ^= true;
                }
                mask >>= 1;
            }
        }
        cpu->pc += 2;
        break;
        // skip if key
    case 0xE000:
        switch (NN)
        {
            // EX9E will skip one instruction (increment PC by 2)
            // if the key corresponding to the value in VX is pressed.
        case 0x9E:
            if (cpu->keys[cpu->V[x]])
            {
                cpu->pc += 4;
            }
            else
            {
                cpu->pc += 2;
            }
            break;
            // EXA1 skips if the key corresponding to the value in VX is not pressed.
        case 0xA1:
            if (!cpu->keys[cpu->V[x]])
            {
                cpu->pc += 4;
            }
            else
            {
                cpu->pc += 2;
            }
            break;
        }
        break;
    case 0xF000:
        switch (NN)
        {
            // FX07 sets VX to the current value of the delay timer
        case 0x07:
            cpu->V[x] = cpu->dtimer;
            cpu->pc += 2;
            break;
            // FX15 sets the delay timer to the value in VX
        case 0x15:
            cpu->dtimer = cpu->V[x];
            cpu->pc += 2;
            break;
            // FX18 sets the sound timer to the value in VX
        case 0x18:
            cpu->stimer = cpu->V[x];
            cpu->pc += 2;
            break;
            // FX1E The index register I will get the value in VX added to it.
        case 0x1E:
            cpu->I += cpu->V[x];
            cpu->pc += 2;
            break;
            // FX0A: Get key This instruction “blocks”; it stops executing instructions
            // and waits for key input (or loops forever, unless a key is pressed).
        case 0x0A:
            bool isPressed = false;
            for (int i = 0; i < 16; i++)
            {
                if (cpu->keys[i])
                {
                    isPressed = true;
                    cpu->V[x] = i;
                    break;
                }
            }

            if (isPressed)
            {
                cpu->pc += 2;
            }
            break;

        case 0x29:
            uint8_t character = cpu->V[x] & 0x0F;
            cpu->I = character * 5 + FONT_SET_START_ADDRESS;
            cpu->pc += 2;
            break;
        case 0x33:
            break;
        case 0x55:
            break;
        case 0x65:
            break;
        }
        break;
    }
}