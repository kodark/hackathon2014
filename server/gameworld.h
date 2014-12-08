#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <QHash>
#include <QDebug>
#include <QElapsedTimer>

#include "player.h"
#include "bullet.h"

#define pow2(A) (A)*(A)

class GameWorld
{
public:
    GameWorld();

    QHash<int, Player> players;
    QHash<qint64, Bullet> bullets;

    double width = 2000;
    double height = 2000;

    int addPlayer();
    void removePlayer(int);
    int initPlayer(int, QString, QString);
    int updatePosition(int, double, double, double);

    void addBullet(int);
    void updateBullets();

    QString getPlayerDesc(int);

    int lastid = 0;
    qint64 lastbulletid = 0;

    QElapsedTimer timer;
};

#endif // GAMEWORLD_H
