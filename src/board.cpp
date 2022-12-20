#include "board.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <SDL2/SDL_image.h>

Board::Board(const std::string &fp)
    : m_fp(fp)
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

    ifs.close();

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

    std::unordered_map<char, SDL_Texture*> textures = {
        { 'p', IMG_LoadTexture(rend, "res/w-pawn.png") },
        { 'r', IMG_LoadTexture(rend, "res/w-rook.png") },
        { 'k', IMG_LoadTexture(rend, "res/w-knight.png") },
        { 'b', IMG_LoadTexture(rend, "res/w-bishop.png") },
        { 'q', IMG_LoadTexture(rend, "res/w-queen.png") },
        { 'g', IMG_LoadTexture(rend, "res/w-king.png") },

        { 'P', IMG_LoadTexture(rend, "res/b-pawn.png") },
        { 'R', IMG_LoadTexture(rend, "res/b-rook.png") },
        { 'K', IMG_LoadTexture(rend, "res/b-knight.png") },
        { 'B', IMG_LoadTexture(rend, "res/b-bishop.png") },
        { 'Q', IMG_LoadTexture(rend, "res/b-queen.png") },
        { 'G', IMG_LoadTexture(rend, "res/b-king.png") }
    };

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

            if (m_grid[y * 8 + x] != ' ')
                SDL_RenderCopy(rend, textures[m_grid[y * 8 + x]], 0, &r);
        }
    }

    for (auto &pair : textures)
        SDL_DestroyTexture(pair.second);

    SDL_SetRenderTarget(rend, nullptr);
    return res;
}

void Board::move(int from, int to)
{
    m_grid[to] = m_grid[from];
    m_grid[from] = ' ';
}

void Board::dump()
{
    std::ofstream ofs(m_fp);
    ofs << (m_turn == Color::WHITE ? 'w' : 'b') << '\n';
    for (size_t i = 0; i < m_grid.size(); ++i)
    {
        ofs << m_grid[i];

        if ((i + 1) % 8 == 0)
            ofs << '\n';
    }

    ofs.close();
}

