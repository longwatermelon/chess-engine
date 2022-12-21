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
    void random_move();

    void dump();

private:
    // raw: ignore moves that would put own king in check and include own pieces if true
    std::vector<int> get_valid_moves(int i, bool raw = false);
    void add_valid_move(std::vector<int> &moves, int from, int to, bool raw);
    void step_in_dir(std::vector<int>& valid, int i, int dx, int dy, bool raw);

    bool detect_check(Color target);
    bool detect_checkmate(Color target);

    char at(int i);
    Color color_at(int i);

private:
    std::string m_fp;
    std::string m_grid;
    Color m_turn{ Color::NONE };

    int m_from{ -1 }, m_to{ -1 };
};

