#include "joy.h"

#ifndef __ESPDATA__H__
#define __ESPDATA__H__

struct arrows
{
    bool up;
    bool down;
    bool left;
    bool right;
};

struct screenData
{
    int screen_1;
    int screen_2;
    int screen_3;
};

struct espdata
{
    joyData joy_data;
    arrows arrow_data;
    int battery;
};

#endif //!__ESPDATA__H__