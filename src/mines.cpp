#include <random>

#include "mines.hpp"

/**
 * Returns the set of pairs adjacent to (x, y). The result will include (x, y)
 * if keep_center is true.
 */
static std::unordered_set<std::pair<long, long>, pair_hash>
adjacent(long x, long y, bool keep_center);

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

std::unordered_set<std::pair<long, long>, pair_hash>
adjacent(long x, long y, bool keep_center)
{
    std::unordered_set<std::pair<long, long>, pair_hash> adj;

    for (long u = x - 1; u <= x + 1; u++)
        for (long v = y - 1; v <= y + 1; v++)
            adj.insert({u, v});

    if (!keep_center)
        adj.erase({x, y});

    return adj;
}

bool reveal(struct minesweeper *g, long x, long y)
{
    if (!g->r.contains({x, y}) && !g->f.contains({x, y}))
    {
        if (!g->r.empty())
            for (auto [u, v] : adjacent(x, y, true) - g->r)
                if ((adjacent(u, v, false) & g->r).empty() && dist(gen) < g->density)
                    g->m.insert({u, v});

        g->r.insert({x, y});

        return true;
    }
    else
        return false;
}

bool flag(struct minesweeper *g, long x, long y)
{
    if (!g->r.contains({x, y}))
    {
        if (!g->f.contains({x, y}))
            g->f.insert({x, y});
        else
            g->f.erase({x, y});

        return true;
    }
    else
        return false;
}

bool chord(struct minesweeper *g, long x, long y)
{
    auto adj = adjacent(x, y, false);

    if (!g->m.contains({x, y}) && g->r.contains({x, y}) &&
        (adj & g->f).size() + (adj & g->m & g->r).size() ==
            (adj & g->m).size())
    {
        for (auto [u, v] : adj - g->f)
            reveal(g, u, v);

        return true;
    }
    else
        return false;
}

enum tile get_tile(struct minesweeper *g, long x, long y)
{
    bool m = g->m.contains({x, y}),
         r = g->r.contains({x, y}),
         f = g->f.contains({x, y});
    enum tile t = m ? (r ? TILE_DETONATED
                         : (f ? TILE_FLAG_RIGHT : TILE_MINE))
                    : (r ? (enum tile)((int)TILE_ZERO + (adjacent(x, y, false) & g->m).size())
                         : (f ? TILE_FLAG_WRONG : TILE_PLAIN));

    if (g->xray)
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
