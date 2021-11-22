//
// Created by edward on 2021/11/16.
//

#ifndef ZERO_LED_H
#define ZERO_LED_H

namespace Navio{
    enum Colors{
        Black,
        Red,
        Green,
        Blue,
        Cyan,
        Magenta,
        Yellow,
        White
    };

    class Led{
    public:
        virtual bool initialize() = 0;
        virtual void setColor(Colors c) = 0;
    };
}

#endif //ZERO_LED_H
