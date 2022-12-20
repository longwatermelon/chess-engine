#pragma once
#include "board.h"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Prog
{
public:
    Prog(SDL_Window *w, SDL_Renderer *r, const std::string &fp);
    ~Prog();

    void render(const std::string &out_fp);

private:
    SDL_Window *m_win{ nullptr };
    SDL_Renderer *m_rend{ nullptr };

    Board m_board;
};

