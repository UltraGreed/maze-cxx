//
// Copy/pasted from internet by ultragreed on 10/9/22.
//

#ifndef THEBESTLABYRINTH_KEYREAD_H
#define THEBESTLABYRINTH_KEYREAD_H

#include <cstdio>
#include <sys/time.h>
#include <iostream>
#include <termios.h>
#include <csignal>

int keyRead() {
    static const int STDIN = fileno(stdin);
    int returnedValue = 0;
    int inputValue = 0;
    fd_set set;
    struct timeval tv{};
    static bool initialized = false;
    if (!initialized) {
        // Use termios to turn off line buffering
        termios term{};
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, nullptr);
        initialized = true;
    }

    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&set);
    FD_SET(STDIN, &set);
    /* Wait up to zero seconds. */
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    returnedValue = select(STDIN + 1, &set, nullptr, nullptr, &tv);
    if (returnedValue < 0) {
        perror("select() error: ");
    }
    else if (returnedValue) {
        read(STDIN, &inputValue, 1);
    }
    return inputValue;
}

#endif //THEBESTLABYRINTH_KEYREAD_H
