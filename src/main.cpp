#include <csignal>
#include <cstdlib>

#include "textui.hpp"

int main()
{
    minesweeper game(0.17);

    signal(SIGINT, SIG_IGN);
    start_textui(game);

    return EXIT_SUCCESS;
}
