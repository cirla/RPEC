/**
 *
 */

#include <cstdlib>

#include "game.hpp"

#if defined(_WIN32) && defined(UNICODE)
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    rpec::Game game;
    game.run();

    return EXIT_SUCCESS;
}

