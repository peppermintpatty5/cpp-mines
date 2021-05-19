#include <cstdlib>
#include <iostream>
#include <unordered_map>

#include "mines.hpp"

int main()
{
    static std::unordered_map<tile, char> const tile_ch = {
        {tile::PLAIN, '-'},
        {tile::ZERO, ' '},
        {tile::ONE, '1'},
        {tile::TWO, '2'},
        {tile::THREE, '3'},
        {tile::FOUR, '4'},
        {tile::FIVE, '5'},
        {tile::SIX, '6'},
        {tile::SEVEN, '7'},
        {tile::EIGHT, '8'},
        {tile::MINE, '*'},
        {tile::DETONATED, '@'},
        {tile::FLAG_RIGHT, '#'},
        {tile::FLAG_WRONG, 'X'},
    };

    minesweeper game(0.17);

    for (cell_t::first_type x = 0; x < 10; x++)
    {
        for (cell_t::second_type y = 0; y < 10; y++)
            game.reveal({x, y});
    }
    for (cell_t::first_type x = 0; x < 10; x++)
    {
        for (cell_t::second_type y = 0; y < 10; y++)
            std::cout << tile_ch.at(game.get_tile({x, y})) << ' ';
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
