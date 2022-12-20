#pragma once
#include <string>
#include <array>
#include <vector>
#include <SDL2/SDL.h>

enum class Color
{
    WHITE,
    BLACK,
    NONE
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
    std::vector<int> get_valid_moves(int i);
    void step_in_dir(std::vector<int>& valid, int dx, int dy);

    Color color_at(int i);

private:
    std::string m_fp;
    std::string m_grid;
    Color m_turn{ Color::NONE };
};

