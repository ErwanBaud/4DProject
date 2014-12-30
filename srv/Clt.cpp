#include "Clt.h"

Clt::Clt(){}

Clt::Clt(QString h, QString p)
{
    setHost(h);
    setPort(p);
    hostPort = host.toString() + ":" + QString::number(port);
}

QHostAddress Clt::getHost()
{
    return host;
}

quint16 Clt::getPort()
{
    return port;
}

void Clt::setHost(QString h)
{
    if (!h.isEmpty()) host = QHostAddress(h);
}

void Clt::setPort(QString p)
{
    if (!p.isEmpty()) port = p.toUShort();
}




bool operator==(const Clt clt1, const Clt clt2)
{
    bool r = false;
    if(clt1.hostPort.compare(clt2.hostPort) == 0) r = true;
    return r;
}


bool operator==(const Clt clt1, const Clt *cIt)
{
    bool r = false;
    if(clt1.hostPort.compare((*cIt).hostPort) == 0) r = true;
    return r;
}


uint qHash(const Clt &c)
{
    return qHash(c.hostPort);
}
