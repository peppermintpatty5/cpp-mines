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
    FLAG_WRONG
};

/**
 * Really lousy hash function
 */
struct pair_hash
{
public:
    template <typename A, typename B>
    std::size_t operator()(std::pair<A, B> const &p)
    {
        return std::hash<A>()(p.first) + std::hash<B>()(p.second);
    }
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

public:
    minesweeper(double density);

    bool reveal(cell_t cell);

    bool flag(cell_t cell);

    bool chord(cell_t cell);

    tile get_tile(cell_t cell);
};

#endif
