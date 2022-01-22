#ifndef TERRARIUM_NET
#define TERRARIUM_NET

#include <SPI.h>
#include <WiFi.h>
#include "display.h"
#include "status.h"

namespace Net
{
     void connect(bool interactive);
     char *statusToString(int code);
}

#endif