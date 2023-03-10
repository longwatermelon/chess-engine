#include "prog.h"
#include <iostream>
#include <fstream>

Prog::Prog(SDL_Window *w, SDL_Renderer *r, const std::string &fp, const std::string &res_fp)
    : m_win(w), m_rend(r), m_board(fp), m_res_fp(res_fp)
{
    std::fstream fs(res_fp, std::fstream::out | std::fstream::trunc);
}

Prog::~Prog()
{
    m_board.dump();
}

static void save_texture(const char* file_name, SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_Texture* target = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, texture);
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    IMG_SavePNG(surface, file_name);
    SDL_FreeSurface(surface);
    SDL_SetRenderTarget(renderer, target);
}

void Prog::render(const std::string &out_fp)
{
    SDL_Texture *tex = m_board.render(m_rend);
    save_texture(out_fp.c_str(), m_rend, tex);
    SDL_DestroyTexture(tex);
}

void Prog::move(const char *from, const char *to)
{
    if (strlen(from) != 2 || strlen(to) != 2)
    {
        std::cerr << "Error: either from or to is not valid.\n";
        exit(EXIT_FAILURE);
    }

    try
    {
        m_board.move(from[0] - 'a' + (7 - (from[1] - '1')) * 8,
                     to[0] - 'a' + (7 - (to[1] - '1')) * 8);
    }
    catch (const std::runtime_error &err)
    {
        std::ofstream ofs(m_res_fp);
        ofs << err.what();
    }
}

void Prog::random_move()
{
    m_board.random_move();
}

