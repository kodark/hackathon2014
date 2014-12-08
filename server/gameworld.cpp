#include "gameworld.h"

GameWorld::GameWorld()
{
    timer.start();
}

int GameWorld::addPlayer()
{
    players[++lastid] = Player();
    return lastid;
}

void GameWorld::removePlayer(int id)
{
    auto it = players.find(id);
    Player p = it.value();
    players.erase(it);
}

int GameWorld::initPlayer(int id, QString fun, QString name)
{
    players[id].name = name;
    int r = players[id].setFunction(fun);

    if (r != 0)
        return r;

    players[id].init = true;

    return 0;
}

int GameWorld::updatePosition(int id, double x, double y, double a)
{
    if (x < 0 || x > width)
        x = 0;

    if (y < 0 || y > width)
        y = 0;

    players[id].x = x;
    players[id].y = y;
    players[id].a = a;

    return 0;
}

void GameWorld::addBullet(int id)
{
    if (timer.elapsed() - players[id].last_bullet < players[id].bullet_timeout)
        return;

    qint64 bid = ++lastbulletid;

    Player *p = &players[id];

    bullets[bid] = Bullet();
    bullets[bid].id = id;
    bullets[bid].x0 = p->x;
    bullets[bid].y0 = p->y;
    bullets[bid].lastx = p->x;
    bullets[bid].lasty = p->y;
    bullets[bid].sina0 = sin(p->a);
    bullets[bid].cosa0 = cos(p->a);
    bullets[bid].rpn = p->rpn.toUtf8().constData();
    bullets[bid].lifetime = p->bullet_lifetime;
    bullets[bid].time = timer.elapsed();

    bullets[bid].init();

    players[id].last_bullet = timer.elapsed();
}

QString GameWorld::getPlayerDesc(int id)
{
    return (QString::number(id) + " " +
            QString::number(players[id].x) + " " +
            QString::number(players[id].y) + " " +
            QString::number(players[id].a));
}
