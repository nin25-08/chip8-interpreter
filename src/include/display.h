#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>

#define PWIDTH 64
#define PHEIGHT 32

typedef struct{
    bool pixels[PHEIGHT][PWIDTH];
}display;
#endif