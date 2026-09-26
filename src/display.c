#include "include/display.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static void display_draw_pixel(int x, int y, bool flag)
{
    if (flag)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawPoint(renderer, x, y);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawPoint(renderer, x, y);
    }
}
bool display_init(display *disp)
{
    memset(disp,0,sizeof(*disp));

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return false;
    }
    if (SDL_CreateWindowAndRenderer(64 * 4, 32 * 4, 0, &window, &renderer) != 0)
    {
        return false;
    }

    SDL_RenderSetLogicalSize(renderer, 64, 32);
    // floods the blackground with back
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    return true;
}



void display_draw(display *disp)
{
    // 1. Wipe the screen completely black to start the frame
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 2. Switch our paint color to White
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // 3. ONLY draw the pixels that are supposed to be white
    for (int i = 0; i < PWIDTH; i++) 
    {
        for (int j = 0; j < PHEIGHT; j++) 
        {
            if (disp->pixels[j][i]) 
            {
                SDL_RenderDrawPoint(renderer, i, j);
            }
        }
    }
}

void display_clear()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void display_update()
{
    SDL_RenderPresent(renderer);
}


