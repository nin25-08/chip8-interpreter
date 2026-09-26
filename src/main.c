#include <stdio.h>
#include <string.h>
#include "chip8.h"
#include "display.h"

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

    loadRom(&cpu,"../BC_test.ch8");

    while(running){
      
        chip8_cycle(&cpu,&disp);
        display_draw(&disp);
        display_update();
        SDL_Delay(16);
    } 
}