#ifndef MINES_HEADER
#define MINES_HEADER

#include <cstddef>
#include <unordered_set>
#include <utility>

/**
 * Represents the state of a cell
 */
enum class tile
{
    PLAIN,
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    MINE,
    DETONATED,
    FLAG_RIGHT,
    FLAG_WRONG,
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
class minesweeper
{
private:
    std::unordered_set<cell_t> mines, revealed, flags;
    double density;
    bool xray;

    /**
     * Returns the set of adjacent cells
     */
    static std::unordered_set<cell_t>
    adjacent(cell_t cell, bool keep_center = false);

    bool reveal_auto(cell_t cell);

    bool reveal_base(cell_t cell);

public:
    minesweeper(double density, bool xray);

    bool reveal(cell_t cell);

    bool flag(cell_t cell);

    bool chord(cell_t cell);

    tile get_tile(cell_t cell);
};

#endif
