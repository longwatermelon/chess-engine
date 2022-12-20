#pragma once
#include "board.h"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Prog
{
public:
    Prog(SDL_Window *w, SDL_Renderer *r, const std::string &fp, const std::string &res_fp);
    ~Prog();

    void render(const std::string &out_fp);
    void move(const char *from, const char *to);

private:
    SDL_Window *m_win{ nullptr };
    SDL_Renderer *m_rend{ nullptr };

    Board m_board;
    std::string m_res_fp;
};

