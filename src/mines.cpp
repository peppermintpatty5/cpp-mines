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

std::unordered_set<std::pair<long, long>, pair_hash>
minesweeper::adjacent(long x, long y, bool keep_center)
{
    std::unordered_set<std::pair<long, long>, pair_hash> adj;

    for (long u = x - 1; u <= x + 1; u++)
        for (long v = y - 1; v <= y + 1; v++)
            adj.insert({u, v});

    if (!keep_center)
        adj.erase({x, y});

    return adj;
}

bool minesweeper::reveal(long x, long y)
{
    if (!revealed.contains({x, y}) && !flags.contains({x, y}))
    {
        if (!revealed.empty())
            for (auto [u, v] : adjacent(x, y, true) - revealed)
                if ((adjacent(u, v, false) & revealed).empty() && dist(gen) < density)
                    mines.insert({u, v});

        revealed.insert({x, y});

        return true;
    }
    else
        return false;
}

bool minesweeper::flag(long x, long y)
{
    if (!revealed.contains({x, y}))
    {
        if (!flags.contains({x, y}))
            flags.insert({x, y});
        else
            flags.erase({x, y});

        return true;
    }
    else
        return false;
}

bool minesweeper::chord(long x, long y)
{
    auto adj = adjacent(x, y, false);

    if (!mines.contains({x, y}) && revealed.contains({x, y}) &&
        (adj & flags).size() + (adj & mines & revealed).size() ==
            (adj & mines).size())
    {
        for (auto [u, v] : adj - flags)
            reveal(u, v);

        return true;
    }
    else
        return false;
}

enum tile minesweeper::get_tile(long x, long y)
{
    bool m = mines.contains({x, y}),
         r = revealed.contains({x, y}),
         f = flags.contains({x, y});
    enum tile t = m ? (r ? TILE_DETONATED
                         : (f ? TILE_FLAG_RIGHT : TILE_MINE))
                    : (r ? (enum tile)((int)TILE_ZERO + (adjacent(x, y, false) & mines).size())
                         : (f ? TILE_FLAG_WRONG : TILE_PLAIN));

    if (xray)
        return t;
    else
    {
        switch (t)
        {
        case TILE_FLAG_WRONG:
            return TILE_FLAG_RIGHT;
        case TILE_MINE:
            return TILE_PLAIN;
        default:
            return t;
        }
    }
}
