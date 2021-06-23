#include <csignal>
#include <cstdio>
#include <cstdlib>

#include "textui.hpp"

static char const *const HELP_FMT =
    "Usage: %s [OPTION]...\n"
    "\n"
    "Plays a game of minesweeper\n"
    "\n"
    "optional arguments:\n"
    "  -d N       set mine density to 0 <= N <= 1, default N = 0.17\n"
    "  -h,        show this help message and exit\n"
    "  -x         enable X-ray cheats\n";

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
                if (argv[i][2] != '\0')
                    density = std::atof(&argv[i][2]);
                else if (i + 1 < argc)
                    density = std::atof(argv[++i]);
                break;
            case 'h':
                std::printf(HELP_FMT, argv[0]);
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
