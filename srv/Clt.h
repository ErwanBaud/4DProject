#ifndef CLT_H
#define CLT_H

#include <QtNetwork>


class Clt
{
    public:
        Clt();
        Clt(QString h, QString p);

        QString hostPort; // Identité du serveur
        quint16 status;
        // 0 -> dead
        // 1 -> alive
        // 2 -> connected

        QTime timeOut;

        QHostAddress getHost();
        quint16 getPort();
        void setHost(QString h);
        void setPort(QString p);

        QTcpSocket *socket;

    private:
        QHostAddress host; // Adresse IP du client
        quint16 port; // Port du client
};

bool operator==(const Clt clt1, const  Clt clt2);
bool operator==(const Clt clt1, const  Clt *cIt);
uint qHash(const Clt &c);

#endif // CLT_H
