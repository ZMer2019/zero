//
// Created by edward on 2021/11/16.
//

#ifndef ZERO_RGBLED_H
#define ZERO_RGBLED_H
#include <led.h>
#include <gpio.h>
namespace Navio{
    class RGBLed {
    public:
        RGBLed();
        bool initialize();
        void setColor(Colors c);

    private:
        Pin *pinR;
        Pin *pinG;
        Pin *pinB;
    };
}



#endif //ZERO_RGBLED_H
