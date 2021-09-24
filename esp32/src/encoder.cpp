#include "encoder.h"

namespace Encoder
{

#define CLK 34
#define DT 32
#define SW 33

    EncButton<EB_TICK, CLK, DT, SW> enc;

    void setup()
    {
        enc.counter = 0;
    }

    void tick()
    {
        enc.tick();
    }

    bool isLeft()
    {
        return enc.isLeft();
    }

    bool isRight()
    {
        return enc.isLeft();
    }

    bool isTurn()
    {
        //Serial.print("turn ");
        //Serial.println(enc.counter);
        return enc.isTurn();
    }
}