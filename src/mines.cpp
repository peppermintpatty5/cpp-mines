#include <queue>
#include <random>

#include "mines.hpp"

/**
 * The reveal_auto function can loop indefinitely, therefore a limit on the
 * number of cells processed is required.
 */
#define REVEAL_AUTO_LIMIT 4096

static std::mt19937 gen(std::random_device{}());
static std::uniform_real_distribution<double> dist(0.0, 1.0);

/**
 * Set intersection operator overload
 */
static std::unordered_set<cell_t>
operator&(std::unordered_set<cell_t> a, std::unordered_set<cell_t> b)
{
    std::unordered_set<cell_t> s;

    for (auto x : a)
        if (b.contains(x))
            s.insert(x);

    return s;
}

/**
 * Set subtraction operator overload
 */
static std::unordered_set<cell_t>
operator-(std::unordered_set<cell_t> a, std::unordered_set<cell_t> b)
{
    std::unordered_set<cell_t> s;

    for (auto x : a)
        if (!b.contains(x))
            s.insert(x);

    return s;
}

/**
 * Really lousy hash function
 */
std::size_t std::hash<cell_t>::operator()(cell_t const &cell) const
{
    return std::hash<cell_t::first_type>()(cell.first << 32 | cell.second);
}

minesweeper::minesweeper(double density)
{
    this->density = density;
}

std::unordered_set<cell_t> minesweeper::adjacent(cell_t cell, bool keep_center)
{
    std::unordered_set<cell_t> adj;

    for (auto x = cell.first - 1; x <= cell.first + 1; x++)
        for (auto y = cell.second - 1; y <= cell.second + 1; y++)
            adj.insert({x, y});

    if (!keep_center)
        adj.erase(cell);

    return adj;
}

bool minesweeper::reveal(cell_t cell)
{
    return reveal_auto(cell);
}

bool minesweeper::reveal_auto(cell_t cell)
{
    std::size_t count = 0;
    std::queue<cell_t> queue;
    std::unordered_set<cell_t> cache;

    queue.push(cell);
    cache.insert(cell);

    while (!queue.empty() && count < REVEAL_AUTO_LIMIT)
    {
        auto f = queue.front();

        if (reveal_base(f))
        {
            auto adj = adjacent(f);

            if (!mines.contains(f) && (adj & mines).empty())
                for (auto a : adj - revealed - flags - cache)
                {
                    queue.push(a);
                    cache.insert(a);
                }
            count++;
        }
        queue.pop();
    }

    return count != 0;
}

bool minesweeper::reveal_base(cell_t cell)
{
    if (!revealed.contains(cell) && !flags.contains(cell))
    {
        if (!revealed.empty())
            for (auto a : adjacent(cell, true) - revealed)
                if ((adjacent(a) & revealed).empty() && dist(gen) < density)
                    mines.insert(a);

        revealed.insert(cell);

        return true;
    }
    else
        return false;
}

bool minesweeper::flag(cell_t cell)
{
    if (!revealed.contains(cell))
    {
        if (!flags.contains(cell))
            flags.insert(cell);
        else
            flags.erase(cell);

        return true;
    }
    else
        return false;
}

bool minesweeper::chord(cell_t cell)
{
    auto adj = adjacent(cell);

    if (!mines.contains(cell) && revealed.contains(cell) &&
        (adj & flags).size() + (adj & mines & revealed).size() ==
            (adj & mines).size())
    {
        for (auto a : adj - flags)
            reveal(a);

        return true;
    }
    else
        return false;
}

tile minesweeper::get_tile(cell_t cell)
{
    static tile const numeric_tiles[] = {
        tile::ZERO,
        tile::ONE,
        tile::TWO,
        tile::THREE,
        tile::FOUR,
        tile::FIVE,
        tile::SIX,
        tile::SEVEN,
        tile::EIGHT,
    };

    bool m = mines.contains(cell),
         r = revealed.contains(cell),
         f = flags.contains(cell);

    return m ? (r ? tile::DETONATED
                  : (f ? tile::FLAG_RIGHT : tile::MINE))
             : (r ? numeric_tiles[(adjacent(cell) & mines).size()]
                  : (f ? tile::FLAG_WRONG : tile::PLAIN));
}
