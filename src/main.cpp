#include <csignal>
#include <cstdio>
#include <cstdlib>

#include "textui.hpp"

int main(int argc, char const *argv[])
{
    double density = 0.17;
    bool xray = false;
    minesweeper *game;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            char flag = argv[i][1];

            switch (flag)
            {
            case 'd':
                density = std::atof(&argv[i][2]);
                break;
            case 'h':
                std::printf("help\n");
                std::exit(EXIT_SUCCESS);
            case 'x':
                xray = true;
                break;
            default:
                std::fprintf(stderr, "invalid option -- '%c'\n", flag);
                std::exit(EXIT_FAILURE);
            }
        }
    }

    game = new minesweeper(density, xray);

    signal(SIGINT, SIG_IGN);
    start_textui(*game);

    return EXIT_SUCCESS;
}
