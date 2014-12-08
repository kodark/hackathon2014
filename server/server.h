#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QByteArray>
#include <QHash>
#include <QTimer>

#include "player.h"
#include "gameworld.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(quint16 port, QObject *parent = Q_NULLPTR);
    ~Server();

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

    void updateBullets();

private:
    QWebSocketServer *m_pWebSocketServer;
    QHash<QWebSocket *, int> m_clients;
    GameWorld m_world;
    QTimer bullet_timer;
};

#endif // SERVER_H
