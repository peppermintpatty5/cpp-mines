#include <curses.h>

#include "textui.hpp"

/**
 * Maps each tile to a character representation
 */
static char tile_char(enum tile t)
{
    switch (t)
    {
    case TILE_ZERO:
        return ' ';
    case TILE_ONE:
        return '1';
    case TILE_TWO:
        return '2';
    case TILE_THREE:
        return '3';
    case TILE_FOUR:
        return '4';
    case TILE_FIVE:
        return '5';
    case TILE_SIX:
        return '6';
    case TILE_SEVEN:
        return '7';
    case TILE_EIGHT:
        return '8';
    case TILE_PLAIN:
        return '-';
    case TILE_MINE:
        return '*';
    case TILE_DETONATED:
        return '@';
    case TILE_FLAG_RIGHT:
        return '#';
    case TILE_FLAG_WRONG:
        return 'X';
    default:
        return '\0';
    }
}

/**
 * Maps each tile to an attribute which can be used directly in wattrset.
 */
static auto tile_attr(enum tile t)
{
    static bool initialized = false;

    if (!initialized)
    {
        init_pair((int)TILE_ZERO + 1, COLOR_WHITE, -1);
        init_pair((int)TILE_ONE + 1, COLOR_BLUE, -1);
        init_pair((int)TILE_TWO + 1, COLOR_GREEN, -1);
        init_pair((int)TILE_THREE + 1, COLOR_RED, -1);
        init_pair((int)TILE_FOUR + 1, COLOR_BLUE, -1);
        init_pair((int)TILE_FIVE + 1, COLOR_RED, -1);
        init_pair((int)TILE_SIX + 1, COLOR_CYAN, -1);
        init_pair((int)TILE_SEVEN + 1, COLOR_WHITE, -1);
        init_pair((int)TILE_EIGHT + 1, COLOR_BLACK, -1);
        init_pair((int)TILE_PLAIN + 1, COLOR_BLACK, -1);
        init_pair((int)TILE_MINE + 1, COLOR_MAGENTA, -1);
        init_pair((int)TILE_DETONATED + 1, COLOR_MAGENTA, -1);
        init_pair((int)TILE_FLAG_RIGHT + 1, COLOR_GREEN, -1);
        init_pair((int)TILE_FLAG_WRONG + 1, COLOR_MAGENTA, -1);

        initialized = true;
    }

    switch (t)
    {
    case TILE_ONE:
    case TILE_THREE:
    case TILE_SEVEN:
    case TILE_EIGHT:
    case TILE_PLAIN:
    case TILE_MINE:
    case TILE_DETONATED:
    case TILE_FLAG_RIGHT:
    case TILE_FLAG_WRONG:
        return COLOR_PAIR((int)t + 1) | A_BOLD;
    case TILE_ZERO:
    case TILE_TWO:
    case TILE_FOUR:
    case TILE_FIVE:
    case TILE_SIX:
        return COLOR_PAIR((int)t + 1) | A_NORMAL;
    default:
        return COLOR_PAIR(0);
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
    wattrset(w, COLOR_PAIR(0));
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
        int max_x = getmaxx(stdscr);
        int max_y = getmaxy(stdscr);

        // print the grid
        curs_set(0);
        for (int y = 0; y < max_y; y++)
        {
            for (int x = 0; x < max_x / 2; x++)
            {
                enum tile t = game.get_tile(ax + x, ay + y);

                wmove(stdscr, y, x * 2 + 1);
                wattrset(stdscr, tile_attr(t));
                waddch(stdscr, tile_char(t));
            }
        }
        wmove(stdscr, cy, cx * 2 + 1);
        wrefresh(stdscr);
        curs_set(1);

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
            game.reveal(ax + cx, ay + cy);
            break;
        case ' ':
            if (!game.flag(ax + cx, ay + cy))
                game.chord(ax + cx, ay + cy);
            break;
        case 'r':
        case 'R':
            wclear(stdscr);
            wrefresh(stdscr);
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
