#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define PWIDTH 64
#define PHEIGHT 32

typedef struct{
    bool pixels[PHEIGHT][PWIDTH];
}display;


bool display_init(display *disp);

void display_draw(display *disp);

void display_clear();

void display_update();

#endif