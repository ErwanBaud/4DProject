#ifndef HEADER_SERVEUR
#define HEADER_SERVEUR

#include <QtWidgets>
#include <QtNetwork>
#include <set>
#include "ui_Serveur.h"
#include "Clt.h"


class Serveur : public QWidget, private Ui::Serveur
{
    Q_OBJECT


    public:
        Serveur();
        QString hostPort; // Identité du serveur
        QList<Clt *> clients; // Liste des clients "alive"

        Clt *clientIsIn(Clt *client, QList<Clt *> &clients);
        //void envoyerATous(const QString &message);
        //void envoyerATous(const QString hostPort, const QTime time, const QString &message);
        //void envoyerAuxAutres(const QString &message);

    private slots:
        // void nouvelleConnexion();
        //void donneesRecues();
        //void deconnexionClient();
        void clientAlive();
        void whoIsAlive();
        void deadCollector();

    private:
        //QLabel *etatServeur;
        //QPushButton *boutonQuitter;

        QHostAddress host; // Adresse IP du serveur
        quint16 port; // Port du serveur
        quint16 appPort = 50885; // Port de l'appli


        QTimer *tDeadCollector;

        QUdpSocket *udpBroadSocket; // Socket d'ecoute des alive
        QTcpServer *serveur; // Socket d'ecoute
        quint16 tailleMessage;

        void connectTo(Clt c);
};

#endif
