#include "prog.h"
#include <iostream>

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *w = SDL_CreateWindow("Chess engine",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        0, 0,
        SDL_WINDOW_HIDDEN);
    SDL_Renderer *r = SDL_CreateRenderer(w, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    {
        // argv[1] = source file
        // argv[2] = rendered game png file
        // argv[3] = game log information
        // argv[3], argv[4] = from, to
        Prog p(w, r, argv[1], argv[3]);

        if (argc == 6)
            p.move(argv[4], argv[5]);

        p.render(argv[2]);
    }

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);

    IMG_Quit();
    SDL_Quit();
    return 0;
}

