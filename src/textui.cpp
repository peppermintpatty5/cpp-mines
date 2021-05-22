#include <curses.h>

#include "textui.hpp"

/**
 * Maps each tile to a character representation
 */
static char tile_char(tile t)
{
    switch (t)
    {
    case tile::PLAIN:
        return '-';
    case tile::ZERO:
        return ' ';
    case tile::ONE:
        return '1';
    case tile::TWO:
        return '2';
    case tile::THREE:
        return '3';
    case tile::FOUR:
        return '4';
    case tile::FIVE:
        return '5';
    case tile::SIX:
        return '6';
    case tile::SEVEN:
        return '7';
    case tile::EIGHT:
        return '8';
    case tile::MINE:
        return '*';
    case tile::DETONATED:
        return '@';
    case tile::FLAG_RIGHT:
        return '#';
    case tile::FLAG_WRONG:
        return 'X';
    default:
        return '\0';
    }
}

/**
 * Maps each tile to an attribute which can be used directly in wattrset.
 */
static auto tile_attr(tile t)
{
    static bool initialized = false;

    if (!initialized)
    {
        init_pair((int)tile::PLAIN + 1, COLOR_WHITE, -1);
        init_pair((int)tile::ZERO + 1, COLOR_WHITE, -1);
        init_pair((int)tile::ONE + 1, COLOR_BLUE, -1);
        init_pair((int)tile::TWO + 1, COLOR_GREEN, -1);
        init_pair((int)tile::THREE + 1, COLOR_RED, -1);
        init_pair((int)tile::FOUR + 1, COLOR_BLUE, -1);
        init_pair((int)tile::FIVE + 1, COLOR_RED, -1);
        init_pair((int)tile::SIX + 1, COLOR_CYAN, -1);
        init_pair((int)tile::SEVEN + 1, COLOR_WHITE, -1);
        init_pair((int)tile::EIGHT + 1, COLOR_BLACK, -1);
        init_pair((int)tile::MINE + 1, COLOR_MAGENTA, -1);
        init_pair((int)tile::DETONATED + 1, COLOR_MAGENTA, -1);
        init_pair((int)tile::FLAG_RIGHT + 1, COLOR_GREEN, -1);
        init_pair((int)tile::FLAG_WRONG + 1, COLOR_MAGENTA, -1);

        initialized = true;
    }

    switch (t)
    {
    case tile::ONE:
    case tile::THREE:
    case tile::SEVEN:
    case tile::EIGHT:
    case tile::MINE:
    case tile::DETONATED:
    case tile::FLAG_RIGHT:
    case tile::FLAG_WRONG:
        return COLOR_PAIR(static_cast<int>(t) + 1) | A_BOLD;
    case tile::PLAIN:
    case tile::ZERO:
    case tile::TWO:
    case tile::FOUR:
    case tile::FIVE:
    case tile::SIX:
        return COLOR_PAIR(static_cast<int>(t) + 1) | A_NORMAL;
    default:
        return COLOR_PAIR(0) | A_NORMAL;
    }
}

/**
 * Prompts the user for yes/no confirmation with the given message. Returns true
 * if the user chose "yes", false if the user chose "no".
 */
static bool confirm_yn(WINDOW *w, char const *msg)
{
    int max_y = getmaxy(w);

    wmove(w, max_y - 1, 0);
    wprintw(w, "%s [y/n]", msg);

    while (true)
    {
        switch (wgetch(w))
        {
        case 'y':
        case 'Y':
            return true;
        case 'n':
        case 'N':
            return false;
        default:
            break;
        }
    }
}

void start_textui(minesweeper &game)
{
    long ax = 0, ay = 0; // anchor point, top left corner
    long cx = 0, cy = 0; // cursor point, relative to anchor

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    start_color();
    use_default_colors();
    wclear(stdscr);

    while (true)
    {
        cell_t cell(ax + cx, ay + cy);
        int max_x = getmaxx(stdscr);
        int max_y = getmaxy(stdscr);

        // print the grid
        for (int y = 0; y < max_y; y++)
        {
            for (int x = 0; x < max_x / 2; x++)
            {
                tile t = game.get_tile({ax + x, ay + y});

                wmove(stdscr, y, x * 2 + 1);
                wattrset(stdscr, tile_attr(t));
                waddch(stdscr, tile_char(t));
            }
        }
        wmove(stdscr, cy, cx * 2 + 1);
        wrefresh(stdscr);

        // take keyboard input
        switch (wgetch(stdscr))
        {
        case 'w':
            ay -= 1;
            break;
        case 's':
            ay += 1;
            break;
        case 'a':
            ax -= 1;
            break;
        case 'd':
            ax += 1;
            break;
        case 'W':
            ay -= 5;
            break;
        case 'S':
            ay += 5;
            break;
        case 'A':
            ax -= 5;
            break;
        case 'D':
            ax += 5;
            break;
        case KEY_UP:
            cy -= 1;
            break;
        case KEY_DOWN:
            cy += 1;
            break;
        case KEY_LEFT:
            cx -= 1;
            break;
        case KEY_RIGHT:
            cx += 1;
            break;
        case KEY_ENTER:
        case '\r':
        case '\n':
            game.reveal(cell);
            break;
        case ' ':
            if (!game.flag(cell))
                game.chord(cell);
            break;
        case 'q':
        case 'Q':
            if (confirm_yn(stdscr, "Quit?"))
            {
                wclear(stdscr);
                wrefresh(stdscr);
                endwin();
                return;
            }
            else
                wclear(stdscr);
            break;
        default:
            break;
        }
    }
}
