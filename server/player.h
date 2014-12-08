#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

#include "function.h"

class Player
{
public:
    Player()
    {
        rpn = toRPN(function);
    }

    double x = 0;
    double y = 0;
    double a = 0;

    static constexpr double r = 25;

    double bullet_lifetime = 5000;
    double bullet_timeout = 300;
    qint64 last_bullet = 0;
    int health = 100;
    bool init = false;
    QString name = "default";
    QString function = "0";
    QString rpn = "(0)";
    bool dead = false;

    int setFunction(const QString& fun)
    {
        function = fun;
        rpn = toRPN(fun);

        if (rpn == "IDI NAHUI")
            return -1;

        return 0;
    }

    void damage()
    {
        health -= 10;
        if (health <= 0)
        {
            health = 100;
            x = random() % 500 + 50;
            y = random() % 500 + 50;
            dead = true;
        }
    }

};

#endif // PLAYER_H
