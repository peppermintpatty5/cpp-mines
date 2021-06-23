#include <random>

#include "mines.hpp"

static std::mt19937 gen(std::random_device{}());
static std::uniform_real_distribution<double> dist(0.0, 1.0);

/**
 * Overload bitwise AND operator for sets to perform set intersection
 */
template <typename A, typename B>
static std::unordered_set<A, B>
operator&(std::unordered_set<A, B> a, std::unordered_set<A, B> b)
{
    std::unordered_set<A, B> s;

    for (auto x : a)
        if (b.contains(x))
            s.insert(x);

    return s;
}

/**
 * Overload minus operator for sets to perform set subtraction
 */
template <typename A, typename B>
static std::unordered_set<A, B>
operator-(std::unordered_set<A, B> a, std::unordered_set<A, B> b)
{
    std::unordered_set<A, B> s;

    for (auto x : a)
        if (!b.contains(x))
            s.insert(x);

    return s;
}

template <typename A, typename B>
std::size_t pair_hash::operator()(std::pair<A, B> const &p) const
{
    return std::hash<A>()(p.first) + std::hash<B>()(p.second);
}

minesweeper::minesweeper(double density, bool xray)
{
    this->density = density;
    this->xray = xray;
}

std::unordered_set<cell_t, pair_hash>
minesweeper::adjacent(cell_t cell, bool keep_center)
{
    std::unordered_set<cell_t, pair_hash> adj;

    for (auto x = cell.first - 1; x <= cell.first + 1; x++)
        for (auto y = cell.second - 1; y <= cell.second + 1; y++)
            adj.insert({x, y});

    if (!keep_center)
        adj.erase(cell);

    return adj;
}

bool minesweeper::reveal(cell_t cell)
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
    tile t = m ? (r ? tile::DETONATED
                    : (f ? tile::FLAG_RIGHT : tile::MINE))
               : (r ? numeric_tiles[(adjacent(cell) & mines).size()]
                    : (f ? tile::FLAG_WRONG : tile::PLAIN));

    if (xray)
        return t;
    else
    {
        switch (t)
        {
        case tile::FLAG_WRONG:
            return tile::FLAG_RIGHT;
        case tile::MINE:
            return tile::PLAIN;
        default:
            return t;
        }
    }
}
