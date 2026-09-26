#include <stdio.h>
#include <string.h>
#include "include/chip8.h"
#include "include/display.h"
#include "include/keypad.h"

void loadRom(chip8 *cpu, const char *filedir)
{
    FILE *fp = fopen(filedir, "rb");
    if (fp == NULL)
    {
        printf("cannot open fp!");
        return;
    }
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    if (size > MEM_SIZE - MEM_RESERVED)
    {
        printf("fp too big!");
        fclose(fp);
        return;
    }
    rewind(fp);

    fread(&cpu->ram[0x200], 1, size, fp);
    fclose(fp);
}
int main(int argc, char **argv)
{
    chip8 cpu;
    display disp;
    bool running = true;
    SDL_Event event;

    chip8_init(&cpu);
    display_init(&disp);

    loadRom(&cpu, "../Pong.ch8");
    int keyCode;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
            {
                int keyCode = keyToNum(event.key.keysym.sym);

                if (keyCode != -1)
                {
                    cpu.keys[keyCode] = (event.type == SDL_KEYDOWN);
                }
                break;
            }
            }
        }
        for (int i = 0; i < 10; i++)
        {
            chip8_cycle(&cpu, &disp);
        }
        display_draw(&disp);

        if (cpu.dtimer > 0) cpu.dtimer--;
        if (cpu.stimer > 0) cpu.stimer--;

        display_update();

        SDL_Delay(16); // to have approx. 60fps
    }
}