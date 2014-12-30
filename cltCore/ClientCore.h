#ifndef HEADER_CLIENT
#define HEADER_CLIENT

#include <QtNetwork>


class Client : public QObject
{
    Q_OBJECT

    public:
        Client();
        bool ready = false; // Le client est il pret ?
        QString hostPort; // Identité du client

    private:
        QHostAddress host; // Adresse IP du client
        quint16 port; // Port du client
        quint16 appPort = 50885; // Port de l'appli

        QUdpSocket *udpBroadSocket; // Broadcast iamAlive
        QTcpServer *client; // Socket d'ecoute
        QList<QTcpSocket *> others; // Sockets vers les autres clients

        quint16 tailleMessage;

        QTimer *tAlive;

    public slots:
        //void initConnexion(QString serveurIP, int serveurPort);
        //void envoyer(QString mess);

    private slots:
        //void donneesRecues();
        //void connecte();
        //void deconnecte();
        //void erreurSocket(QAbstractSocket::SocketError erreur);

        void iamAlive();
};

#endif
