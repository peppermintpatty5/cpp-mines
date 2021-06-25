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
    struct minesweeper g;

    g.density = 0.17;
    g.xray = false;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            char flag = argv[i][1];

            switch (flag)
            {
            case 'd':
                if (argv[i][2] != '\0')
                    g.density = std::atof(&argv[i][2]);
                else if (i + 1 < argc)
                    g.density = std::atof(argv[++i]);
                break;
            case 'h':
                std::printf(HELP_FMT, argv[0]);
                std::exit(EXIT_SUCCESS);
            case 'x':
                g.xray = true;
                break;
            default:
                std::fprintf(stderr, "invalid option -- '%c'\n", flag);
                std::exit(EXIT_FAILURE);
            }
        }
    }

    signal(SIGINT, SIG_IGN);
    start_textui(&g);

    return EXIT_SUCCESS;
}
