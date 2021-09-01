#ifndef TERRARIUM_ENCODER
#define TERRARIUM_ENCODER

#include <EncButton.h>

namespace Encoder
{
    void setup();
    void tick();
    bool isLeft();
    bool isRight();
    bool isTurn();
}

#endif