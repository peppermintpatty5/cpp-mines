#include <cstdlib>
#include <iostream>

#include "mines.hpp"

/**
 * Maps each tile to a character representation
 */
static char tile_ch(tile t)
{
    switch (t)
    {
    case tile::PLAIN:
        return '-';
    case tile::ZERO:
        return ' ';
    case tile::ONE:
        return '1';
    case tile::TWO:
        return '2';
    case tile::THREE:
        return '3';
    case tile::FOUR:
        return '4';
    case tile::FIVE:
        return '5';
    case tile::SIX:
        return '6';
    case tile::SEVEN:
        return '7';
    case tile::EIGHT:
        return '8';
    case tile::MINE:
        return '*';
    case tile::DETONATED:
        return '@';
    case tile::FLAG_RIGHT:
        return '#';
    case tile::FLAG_WRONG:
        return 'X';
    default:
        return '\0';
    }
}

int main()
{
    minesweeper game(0.17);

    for (cell_t::first_type x = 0; x < 10; x++)
    {
        for (cell_t::second_type y = 0; y < 10; y++)
            game.reveal({x, y});
    }
    for (cell_t::first_type x = 0; x < 10; x++)
    {
        for (cell_t::second_type y = 0; y < 10; y++)
            std::cout << tile_ch(game.get_tile({x, y})) << ' ';
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
