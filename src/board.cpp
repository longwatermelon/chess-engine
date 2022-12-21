#include "board.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
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
    if (m_grid[from] == ' ')
        throw std::runtime_error("Error: not a piece");

    if (color_at(from) != m_turn)
        throw std::runtime_error("Error: wrong piece color");

    std::vector<int> moves = get_valid_moves(from);
    if (std::find(moves.begin(), moves.end(), to) == moves.end())
        throw std::runtime_error("Error: invalid move");

    m_grid[to] = m_grid[from];
    m_grid[from] = ' ';

    m_turn = m_turn == Color::WHITE ? Color::BLACK : Color::WHITE;
    if (detect_check(m_turn))
    {
        std::string col = m_turn == Color::WHITE ? "White" : "Black";

        if (detect_checkmate(m_turn))
            throw std::runtime_error("Checkmate: " + col + " loses.");
        else
            throw std::runtime_error(col + " is in check.");
    }
}

void Board::random_move()
{
    std::vector<int> pieces;
    for (size_t i = 0; i < m_grid.size(); ++i)
    {
        if (color_at(i) == m_turn && !get_valid_moves(i).empty())
            pieces.emplace_back(i);
    }

    int i = pieces[rand() % pieces.size()];
    std::vector<int> moves = get_valid_moves(i);

    move(i, moves[rand() % moves.size()]);
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

std::vector<int> Board::get_valid_moves(int i, bool raw)
{
    std::vector<int> valid;

    switch (m_grid[i])
    {
    case 'p':
        if (at(i - 8) == ' ') add_valid_move(valid, i, i - 8, raw);
        if (i >= 8 * 6 - 1 && at(i - 16) == ' ') add_valid_move(valid, i, i - 16, raw);

        if (color_at(i - 8 - 1) == Color::BLACK) add_valid_move(valid, i, i - 8 - 1, raw);
        if (color_at(i - 8 + 1) == Color::BLACK) add_valid_move(valid, i, i - 8 + 1, raw);
        break;
    case 'P':
        if (at(i + 8) == ' ') add_valid_move(valid, i, i + 8, raw);
        if (i <= 8 * 2 - 1 && at(i + 16) == ' ') add_valid_move(valid, i, i + 16, raw);

        if (color_at(i + 8 - 1) == Color::WHITE) add_valid_move(valid, i, i + 8 - 1, raw);
        if (color_at(i + 8 + 1) == Color::WHITE) add_valid_move(valid, i, i + 8 + 1, raw);
        break;
    case 'r':
    case 'R':
        step_in_dir(valid, i, 1, 0, raw);
        step_in_dir(valid, i, 0, -1, raw);
        step_in_dir(valid, i, -1, 0, raw);
        step_in_dir(valid, i, 0, 1, raw);
        break;
    case 'k':
    case 'K':
    {
        int x = i % 8;
        int y = i / 8;
        for (int r = std::max(y - 2, 0); r <= std::min(y + 2, 8); ++r)
        {
            for (int c = std::max(x - 2, 0); c <= std::min(x + 2, 8); ++c)
            {
                if (std::abs((r - y) * (c - x)) == 2 && color_at(r * 8 + c) != color_at(i))
                    add_valid_move(valid, i, r * 8 + c, raw);
            }
        }
    } break;
    case 'b':
    case 'B':
        step_in_dir(valid, i, 1, 1, raw);
        step_in_dir(valid, i, -1, 1, raw);
        step_in_dir(valid, i, -1, -1, raw);
        step_in_dir(valid, i, 1, -1, raw);
        break;
    case 'q':
    case 'Q':
        step_in_dir(valid, i, 1, 1, raw);
        step_in_dir(valid, i, -1, 1, raw);
        step_in_dir(valid, i, -1, -1, raw);
        step_in_dir(valid, i, 1, -1, raw);

        step_in_dir(valid, i, 1, 0, raw);
        step_in_dir(valid, i, 0, -1, raw);
        step_in_dir(valid, i, -1, 0, raw);
        step_in_dir(valid, i, 0, 1, raw);
        break;
    case 'g':
    case 'G':
        int x = i % 8;
        int y = i / 8;
        for (int r = std::max(y - 1, 0); r <= std::min(y + 1, 8); ++r)
        {
            for (int c = std::max(x - 1, 0); c <= std::min(x + 1, 8); ++c)
            {
                if (color_at(r * 8 + c) != color_at(i))
                    add_valid_move(valid, i, r * 8 + c, raw);
            }
        }
        break;
    }

    return valid;
}

void Board::add_valid_move(std::vector<int> &moves, int from, int to, bool raw)
{
    if (from < 0 || from >= 64 || to < 0 || to >= 64)
        return;

    if (raw)
    {
        moves.emplace_back(to);
    }
    else
    {
        char tmp = m_grid[to];
        m_grid[to] = m_grid[from];
        m_grid[from] = ' ';

        if (!detect_check(color_at(to)))
            moves.emplace_back(to);

        m_grid[from] = m_grid[to];
        m_grid[to] = tmp;
    }
}

void Board::step_in_dir(std::vector<int>& valid, int i, int dx, int dy, bool raw)
{
    Color col = color_at(i);
    /* valid.emplace_back(i); */
    int x = i % 8 + dx;
    int y = i / 8 + dy;

    while (x >= 0 && x < 8 && y >= 0 && y < 8 && at(y * 8 + x) == ' ')
    {
        add_valid_move(valid, i, y * 8 + x, raw);
        x += dx;
        y += dy;
    }

    if ((raw || col != color_at(y * 8 + x)) && color_at(y * 8 + x) != Color::NONE)
        add_valid_move(valid, i, y * 8 + x, raw);
}

bool Board::detect_check(Color target)
{
    // Find king pos
    size_t king_i;
    for (size_t i = 0; i < m_grid.size(); ++i)
    {
        if (m_grid[i] == (target == Color::WHITE ? 'g' : 'G'))
        {
            king_i = i;
            break;
        }
    }

    for (size_t i = 0; i < m_grid.size(); ++i)
    {
        if (color_at(i) != target)
        {
            std::vector<int> moves = get_valid_moves(i, true);
            if (std::find(moves.begin(), moves.end(), king_i) != moves.end())
                return true;
        }
    }

    return false;
}

bool Board::detect_checkmate(Color target)
{
    for (size_t i = 0; i < m_grid.size(); ++i)
    {
        if (color_at(i) == target)
        {
            if (!get_valid_moves(i).empty())
                return false;
        }
    }

    return true;
}

char Board::at(int i)
{
    if (i < 0 || i >= 64) return ' ';
    return m_grid[i];
}

Color Board::color_at(int i)
{
    if (i < 0 || i >= 64) return Color::NONE;
    return m_grid[i] == ' ' ? Color::NONE : (m_grid[i] - 'a' < 0 ? Color::BLACK : Color::WHITE);
}

