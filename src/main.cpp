#include "prog.h"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cerr << "Error: not enough args\n";
        exit(EXIT_FAILURE);
    }

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *w = SDL_CreateWindow("Chess engine",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        0, 0,
        SDL_WINDOW_HIDDEN);
    SDL_Renderer *r = SDL_CreateRenderer(w, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    {
        Prog p(w, r, argv[1], "log");
        p.move(argv[2], argv[3]);
        p.render("out.png");
    }

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);

    IMG_Quit();
    SDL_Quit();
    return 0;
}

