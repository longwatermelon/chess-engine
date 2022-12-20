#include "board.h"
#include <iostream>
#include <fstream>
#include <sstream>

Board::Board(const std::string &fp)
{
    std::ifstream ifs(fp);
    std::stringstream ss;
    std::string buf;

    // Get turn information
    std::getline(ifs, buf);
    if (buf == "w") m_turn = Color::WHITE;
    else m_turn = Color::BLACK;

    // Get board information
    while (std::getline(ifs, buf))
        ss << buf;

    m_grid = ss.str();
    if (m_grid.size() != 64)
    {
        std::cerr << "Error: grid size is not 64 (" << m_grid.size() << ").\n";
        std::cout << m_grid << "\n";
        exit(EXIT_FAILURE);
    }
}

Board::~Board()
{
}

SDL_Texture *Board::render(SDL_Renderer *rend)
{
    SDL_Texture *res = SDL_CreateTexture(rend,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        800, 800);
    SDL_SetRenderTarget(rend, res);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderFillRect(rend, nullptr);

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            if (std::abs(x - y) % 2 == 0)
                SDL_SetRenderDrawColor(rend, 240, 250, 250, 255);
            else
                SDL_SetRenderDrawColor(rend, 83, 170, 173, 255);

            SDL_Rect r = { x * 100, y * 100, 100, 100 };
            SDL_RenderFillRect(rend, &r);
        }
    }

    SDL_SetRenderTarget(rend, nullptr);
    return res;
}

