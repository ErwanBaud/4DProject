#include <iostream>
#include "ClientCore.h"

using namespace std;

Client::Client()
{
    // Gestion du client
    client = new QTcpServer(this);
    if (!client->listen(QHostAddress("127.0.0.1"), 0)) // Démarrage du client sur 127.0.0.1 et sur un port aleatoire
    {
        // Si le client n'a pas été démarré correctement
        cout << "Le client n'a pas pu etre demarre. Raison : " + client->errorString().toStdString() << endl;
    }
    else
    {
        host = client->serverAddress();
        port = client->serverPort();
        hostPort = host.toString() + ":" + QString::number(port);

        tailleMessage = 0;
        ready = false;

        tAlive = new QTimer();
        tAlive->setInterval(2000);

        udpBroadSocket = new QUdpSocket(this);

        //connect(client, SIGNAL(newConnection()), this, SLOT(nouvelleConnexion()));
        connect(tAlive, SIGNAL(timeout()), this, SLOT(iamAlive()));
        tAlive->start();
    }
}



// Envoi du message iamAlive
void Client::iamAlive()
{
    cout << "I am alive on " + hostPort.toStdString() << endl;
    QByteArray datagram;
    datagram.append(host.toString());
    datagram.append("#");
    datagram.append(QByteArray::number(port));
    udpBroadSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, appPort);
}


/*
// Tentative de connexion au serveur
void Client::initConnexion()//QString serveurIP, int serveurPort)
{
    // On annonce sur la fenêtre qu'on est en train de se connecter
    cout << "Tentative de connexion en cours..." << endl;

    socket->abort(); // On désactive les connexions précédentes s'il y en a
    socket->connectToHost(serveurIP, serveurPort); // On se connecte au serveur demandé
    socket->waitForConnected();
}



// Envoi d'un message au serveur
void Client::envoyer(QString mess)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << hostPort << QTime::currentTime() << mess;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    socket->write(paquet); // On envoie le paquet
    socket->waitForBytesWritten();
}

// On a reçu un paquet (ou un sous-paquet)
void Client::donneesRecues()
{
    // Même principe que lorsque le serveur reçoit un paquet :
    // On essaie de récupérer la taille du message
    // Une fois qu'on l'a, on attend d'avoir reçu le message entier (en se basant sur la taille annoncée tailleMessage)

    QDataStream in(socket);

    if (tailleMessage == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
             return;

        in >> tailleMessage;
    }

    if (socket->bytesAvailable() < tailleMessage)
        return;


    // Si on arrive jusqu'à cette ligne, on peut récupérer le message entier
    QString hostPort, messageRecu;
    QTime time;
    int ms;

    in >> hostPort;
    in >> time;
    in >> messageRecu;

    ms = QTime::currentTime().msecsTo(time);

    // On affiche le message sur la zone de Chat
   cout << hostPort.toStdString() << " " << ms << " : " << messageRecu.toStdString() << endl;

    // On remet la taille du message à 0 pour pouvoir recevoir de futurs messages
    tailleMessage = 0;
}

// Ce slot est appelé lorsque la connexion au serveur a réussi
void Client::connecte()
{
    host = socket->localAddress();
    port = socket->localPort();
    hostPort = host.toString() + ":" + QString::number(port);
    cout << hostPort.toStdString() << endl;
    cout << "Connexion reussie !" << endl;
    ready = true;
}

// Ce slot est appelé lorsqu'on est déconnecté du serveur
void Client::deconnecte()
{
    cout << "Deconnecte du serveur." << endl;
}

// Ce slot est appelé lorsqu'il y a une erreur
void Client::erreurSocket(QAbstractSocket::SocketError erreur)
{
    switch(erreur) // On affiche un message différent selon l'erreur qu'on nous indique
    {
        case QAbstractSocket::HostNotFoundError:
            cout << "ERREUR : le serveur n'a pas pu etre trouve. Verifiez l'IP et le port." << endl;
            break;
        case QAbstractSocket::ConnectionRefusedError:
            cout << "ERREUR : le serveur a refuse la connexion. Verifiez si le programme \"serveur\" a bien ete lance. Verifiez aussi l'IP et le port." << endl;
            break;
        case QAbstractSocket::RemoteHostClosedError:
            cout << "ERREUR : le serveur a coupe la connexion." << endl;
            break;
        default:
            cout << "ERREUR : " << socket->errorString().toStdString() << endl;
    }
}
*/
