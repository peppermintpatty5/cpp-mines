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

/**
 * Really lousy hash function
 */
struct pair_hash
{
public:
    template <typename A, typename B>
    std::size_t operator()(std::pair<A, B> const &p) const;
};

/**
 * Infinite minesweeper game
 */
struct minesweeper
{
    std::unordered_set<std::pair<long, long>, pair_hash>
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
