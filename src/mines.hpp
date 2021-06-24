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

typedef std::pair<long, long> cell_t;

/**
 * Infinite minesweeper game
 */
class minesweeper
{
private:
    std::unordered_set<cell_t, pair_hash> mines, revealed, flags;
    double density;
    bool xray;

    /**
     * Returns the set of adjacent cells
     */
    static std::unordered_set<cell_t, pair_hash>
    adjacent(cell_t cell, bool keep_center = false);

public:
    minesweeper(double density, bool xray);

    bool reveal(cell_t cell);

    bool flag(cell_t cell);

    bool chord(cell_t cell);

    tile get_tile(cell_t cell);
};

#endif
