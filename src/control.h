#pragma once

#include <termios.h>
#include <unistd.h>

void setNonCanonicalMode(bool enable){

    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);

    if (enable){

        ttystate.c_lflag &= ~(ICANON | ECHO);
        ttystate.c_cc[VMIN] = 1;
    }
    else{
        
        ttystate.c_lflag |= ICANON;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}