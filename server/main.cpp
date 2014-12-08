#include <QCoreApplication>
#include <QDebug>
#include "server.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));

    quint16 port = 0;

    if (argc < 2)
    {
        qWarning() << "No port. Using 1337!";
        port = 1337;
    } else
    {
        port = QString(argv[1]).toUInt();
    }

    QCoreApplication a(argc, argv);
    Server *server = new Server(port);
    QObject::connect(server, &Server::closed, &a, &QCoreApplication::quit);

    return a.exec();
}
