//
// Created by sha0 on 17/06/15.
//

#ifndef WFLOOD_CHRONO_H
#define WFLOOD_CHRONO_H

#include <sys/timeb.h>


class Chrono {
    bool running;
    unsigned int timeout;


protected:

    unsigned int getSeconds() {
        struct timeb tp;
        ftime(&tp);
        return tp.time;
    }

public:

    Chrono() {
        running = false;
    }

    void start(unsigned int timeout) {
        this->timeout = getSeconds() + timeout;
        running = true;
    }

    bool isRunning() {
        return running;
    }

    unsigned int remain() {
        return (timeout - getSeconds());
    }

    bool end() {
        if (running && getSeconds() > timeout) {
            running = false;
            cout << "timeout." << endl;
            return true;
        }
        return false;
    }




};

#endif //WFLOOD_CHRONO_H
