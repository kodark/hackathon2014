#ifndef BULLET_H
#define BULLET_H

#include "function.h"

#define BULLET_DX 0.1
#define COEF 10

class Bullet
{
public:
    Bullet() {}
    int id = 0;

    double t = 0;

    double x = 0;
    double y = 0;

    double lastx = 0;
    double lasty = 0;

    double yin0 = 0;

    double x0 = 0;
    double y0 = 0;
    double cosa0 = 0;
    double sina0 = 0;

    int lifetime = 0;

    qint64 time = 0;

    bool valid = true;

    string rpn = "0";

    void init()
    {
        yin0 = calc(rpn, 0);
    }

    void update()
    {
        lastx = x;
        lasty = y;

        t += BULLET_DX;
        y = calc(rpn, t) - yin0;

        double xr = t * cosa0 + y * sina0;
        double yr = t * sina0 - y * cosa0;

        double t = yr;
        yr = -xr;
        xr = t;

        x = x0 + COEF * xr;
        y = y0 + COEF * yr;
    }
};

#endif // BULLET_H
