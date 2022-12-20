#pragma once
#include <string>
#include <array>
#include <SDL2/SDL.h>

enum class Color
{
    WHITE,
    BLACK
};

class Board
{
public:
    Board(const std::string &fp);
    ~Board();

    SDL_Texture *render(SDL_Renderer *rend);

    // from and to are indexes
    void move(int from, int to);

    void dump();

private:
    std::string m_fp;
    std::string m_grid;
    Color m_turn{ Color::WHITE };
};

