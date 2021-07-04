#ifndef MINES_HEADER
#define MINES_HEADER

#include <cstddef>
#include <unordered_set>

/**
 * Represents the state of a cell
 */
enum tile
{
    TILE_ZERO,
    TILE_ONE,
    TILE_TWO,
    TILE_THREE,
    TILE_FOUR,
    TILE_FIVE,
    TILE_SIX,
    TILE_SEVEN,
    TILE_EIGHT,
    TILE_PLAIN,
    TILE_MINE,
    TILE_DETONATED,
    TILE_FLAG_RIGHT,
    TILE_FLAG_WRONG,
};

typedef std::pair<long, long> cell_t;

namespace std
{
    template <>
    struct hash<cell_t>
    {
        size_t operator()(cell_t const &cell) const;
    };
}

/**
 * Infinite minesweeper game
 */
struct minesweeper
{
    std::unordered_set<cell_t>
        m,          // the set of mines
        r,          // the set of revealed cells
        f;          // the set of flagged cells
    double density; // the proportion of cells which are mines
    bool xray;      // xray cheats, show everything
};

bool reveal(struct minesweeper *g, long x, long y);

bool flag(struct minesweeper *g, long x, long y);

bool chord(struct minesweeper *g, long x, long y);

enum tile get_tile(struct minesweeper *g, long x, long y);

#endif
