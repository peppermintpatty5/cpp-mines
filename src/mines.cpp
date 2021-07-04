#include <queue>
#include <random>
#include <string>

#include "mines.hpp"

/**
 * The reveal_auto function can loop indefinitely, therefore a limit on the
 * number of cells processed is required.
 */
#define REVEAL_AUTO_LIMIT 1024

/**
 * Returns the set of pairs adjacent to (x, y). The result will include (x, y)
 * if keep_center is true.
 */
static std::unordered_set<std::pair<long, long>>
adjacent(long x, long y, bool keep_center);

static bool reveal_auto(struct minesweeper *g, long x, long y);

static bool reveal_base(struct minesweeper *g, long x, long y);

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
    return std::hash<std::string>()(
        std::to_string(cell.first) + "," + std::to_string(cell.second));
}

std::unordered_set<cell_t>
adjacent(long x, long y, bool keep_center)
{
    std::unordered_set<std::pair<long, long>> adj;

    for (long u = x - 1; u <= x + 1; u++)
        for (long v = y - 1; v <= y + 1; v++)
            adj.insert({u, v});

    if (!keep_center)
        adj.erase({x, y});

    return adj;
}

bool reveal(struct minesweeper *g, long x, long y)
{
    return reveal_auto(g, x, y);
}

bool reveal_auto(struct minesweeper *g, long x, long y)
{
    std::size_t count = 0;
    std::queue<cell_t> queue;
    std::unordered_set<cell_t> cache;

    queue.push({x, y});
    cache.insert({x, y});

    while (!queue.empty() && count < REVEAL_AUTO_LIMIT)
    {
        auto pop = queue.front();

        if (reveal_base(g, pop.first, pop.second))
        {
            auto adj = adjacent(pop.first, pop.second, false);

            if (!g->m.contains(pop) && (adj & g->m).empty())
                for (auto a : adj - g->r - g->f - cache)
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

bool reveal_base(struct minesweeper *g, long x, long y)
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
