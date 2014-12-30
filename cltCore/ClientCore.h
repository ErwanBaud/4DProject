#ifndef HEADER_CLIENT
#define HEADER_CLIENT

#include <QtNetwork>

/* ClientCore : Processus client
 * */
class ClientCore : public QObject
{
    Q_OBJECT

    public:
        ClientCore();
        bool ready = false; // Le client est il pret ?
        QString hostPort; // Identité du processus client

    private:
        QHostAddress host; // Adresse IP du client
        quint16 port; // Port du client
        quint16 appPort = 50885; // Port sur lequel l'hyperviseur écoute

        QUdpSocket *udpBroadSocket; // Socket pour le broadcast iamAlive
        QTcpServer *client; // Socket d'ecoute
        QList<QTcpSocket *> others; // Sockets vers les autres clients

        quint16 tailleMessage;

        QTimer *tAlive; // Timer pour le broadcast iamAlive

    public slots:
        //void initConnexion(QString serveurIP, int serveurPort);
        //void envoyer(QString mess);

    private slots:
        //void donneesRecues();
        //void connecte();
        //void deconnecte();
        //void erreurSocket(QAbstractSocket::SocketError erreur);

        void iamAlive(); // Envoi un broadcast annonçant la présence du processus client
};

#endif
