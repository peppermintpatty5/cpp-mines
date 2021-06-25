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
class minesweeper
{
private:
    std::unordered_set<std::pair<long, long>, pair_hash> mines, revealed, flags;
    double density;
    bool xray;

    /**
     * Returns the set of adjacent cells
     */
    static std::unordered_set<std::pair<long, long>, pair_hash>
    adjacent(long x, long y, bool keep_center);

public:
    minesweeper(double density, bool xray);

    bool reveal(long x, long y);

    bool flag(long x, long y);

    bool chord(long x, long y);

    enum tile get_tile(long x, long y);
};

#endif
