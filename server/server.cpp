#include "server.h"

#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>

QT_USE_NAMESPACE

Server::Server(quint16 port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("8000 Server"),
                                            QWebSocketServer::NonSecureMode, this)),
    m_clients()
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Server listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Server::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &Server::closed);
    }

    connect(&bullet_timer, &QTimer::timeout, this, &Server::updateBullets);

    bullet_timer.setInterval(5);
    bullet_timer.start();
}

Server::~Server()
{
    m_pWebSocketServer->close();
    for (QWebSocket *s : m_clients.keys())
    {
        delete s;
    }
}

void Server::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Server::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &Server::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &Server::socketDisconnected);

    m_clients[pSocket] = m_world.addPlayer();
}

void Server::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (!pClient) {
        return;
    }

    int id = m_clients.value(pClient);

    QStringList strlist = message.split(' ');
    QString cmd = strlist[0];

    int r = 0;

    switch(cmd.toInt())
    {
    case 1:
        if (strlist.size() < 3)
            pClient->sendTextMessage(QString("error not enougth arguments"));
        r = m_world.initPlayer(id, strlist[1], strlist[2]);
        if (r != 0)
        {
            pClient->sendTextMessage(QString("error") + QString::number(r));
            m_world.initPlayer(id, "0", strlist[2]);
        }

        for (QWebSocket* s: m_clients.keys())
        {
            if (s != pClient)
            {
                s->sendTextMessage("4 " + QString::number(id) + " " + strlist[2]);
            }
        }

        for (auto it = m_world.players.begin(); it != m_world.players.end(); ++it)
        {
            if (it.key() != id)
            {
                pClient->sendTextMessage("4 " +
                                         QString::number(it.key()) + " " +
                                         it.value().name);
                pClient->sendTextMessage("5 " + m_world.getPlayerDesc(it.key()));
            }
        }

        qDebug() << id << "connected";
        qDebug() << '\t'<< "name: " << m_world.players[id].name;
        qDebug() << '\t'<< "x: " << m_world.players[id].x;
        qDebug() << '\t'<< "y: " << m_world.players[id].y;
        qDebug() << '\t'<< "fun: " << m_world.players[id].function;
        qDebug() << '\t'<< "rpn: " << m_world.players[id].rpn;
        break;

    case 2:
        if (strlist.size() < 4)
            pClient->sendTextMessage(QString("error not enougth arguments"));
        r = m_world.updatePosition(id, strlist[1].toDouble(), strlist[2].toDouble(), strlist[3].toDouble());
        if (r != 0)
            pClient->sendTextMessage(QString("error") + QString::number(r));

        for (QWebSocket *s : m_clients.keys())
        {
            if (s != pClient)
                s->sendTextMessage("5 " + m_world.getPlayerDesc(id));
        }

        break;

    case 3:
        m_world.addBullet(id);
        break;
    }
}

void Server::processBinaryMessage(QByteArray)
{
}

void Server::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    int id = m_clients.value(pClient);
    if (pClient) {
        m_clients.remove(pClient);
        pClient->deleteLater();
    }

    m_world.removePlayer(id);

    for (QWebSocket* s: m_clients.keys())
    {
        s->sendTextMessage("6 " + QString::number(id));
    }
}

void Server::updateBullets()
{
    QList<qint64> to_delete;

    for (auto it = m_world.bullets.begin(); it != m_world.bullets.end(); ++it)
    {
        it.value().update();

        double x = it.value().x;
        double y = it.value().y;

        if (x < 0 || y < 0 || x > m_world.width || y > m_world.height)
        {
            it.value().valid = false;
            continue;
        }

        if (it.value().time + it.value().lifetime < m_world.timer.elapsed())
        {
            it.value().valid = false;
            continue;
        }

        for (auto pit = m_world.players.begin(); pit != m_world.players.end(); ++pit)
        {
            if (pit.key() != it.value().id)
            {
                if (pow2(x - pit.value().x) + pow2(y - (pit.value().y)) < pow2(Player::r))
                {
                    it.value().valid = false;
                    pit.value().damage();
                    for (QWebSocket* s: m_clients.keys())
                    {
                        s->sendTextMessage("10 " + QString::number(pit.key()) + " " + QString::number(pit.value().health));
                    }
                }

                if (pit.value().dead)
                {
                    m_clients.key(pit.key())->sendTextMessage("9 " + QString::number(pit.value().x) + " " + QString::number(pit.value().y));
                    for (QWebSocket* s: m_clients.keys())
                    {
                        s->sendTextMessage("5 " + m_world.getPlayerDesc(pit.key()));
                    }
                    pit.value().dead = false;
                }
            }
        }
    }

    for (QWebSocket* s: m_clients.keys())
    {
        for (auto it = m_world.bullets.begin(); it != m_world.bullets.end(); ++it)
        {
            if (it.value().valid)
                s->sendTextMessage("7 " + QString::number(it.key()) + " " + QString::number(it.value().id) + " " + QString::number(it.value().x) + " " + QString::number(it.value().y));
            else
            {
                s->sendTextMessage("8 " + QString::number(it.key()));
                to_delete.push_back(it.key());
            }
        }
    }

    for (qint64 k : to_delete)
    {
        m_world.bullets.remove(k);
    }
}
