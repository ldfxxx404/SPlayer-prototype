#include "control.h"
#include <termios.h>
#include <unistd.h>

void setNonCanonicalMode(bool enable)
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);

    if (enable)
    {
        term.c_lflag &= ~(ICANON | ECHO);
    }
    else
    {
        term.c_lflag |= ICANON | ECHO;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
