#include "Serveur.h"


/* Constructeur
 * */
Serveur::Serveur()
{
    setupUi(this);

    connect(boutonQuitter, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(boutonWho, SIGNAL(clicked()), this, SLOT(whoIsAlive()));
    connect(boutonStart, SIGNAL(clicked()), this, SLOT(startClient()));


    //QVBoxLayout *layout = new QVBoxLayout;
    //layout->addWidget(etatServeur);
    //layout->addWidget(boutonQuitter);
    //setLayout(layout);

    //setWindowTitle(tr("ZeroChat - Serveur"));

    // Démarrage du serveur
    appPort = 50885;

    // Initialisation des composants pour la gestion des iamALive reçus
    udpBroadSocket = new QUdpSocket(this);
    if ( (udpBroadSocket->bind(appPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) )
        listeMessages->append(tr("     Socket UDP ready."));
    else
        listeMessages->append(tr("     Erreur socket UDP."));

    //udpBroadSocket->bind(QHostAddress("127.0.0.1"), appPort);
    connect(udpBroadSocket, SIGNAL(readyRead()), this, SLOT(clientAlive()));

    // Si le serveur a été démarré correctement
    host = QHostAddress("127.0.0.1");
    port = udpBroadSocket->localPort();
    hostPort = host.toString() + ":" + QString::number(port);

    // Mise à jour des labels de l'interface
    serveurIP->setText(host.toString());
    serveurPort->setText(QString::number(appPort));

    listeMessages->append(tr("Le serveur a ete demarre sur le port <strong>") + QString::number(port));

    // Initialisation des composants du deadCollector
    tDeadCollector = new QTimer();
    tDeadCollector->setInterval(3000);
    connect(tDeadCollector, SIGNAL(timeout()), this, SLOT(deadCollector()));
    tDeadCollector->start();

    tailleMessage = 0;
}



/* Executé a chaque reception d'un iamAlive
 * Ajout des nouveaux clients à la liste
 * Rafraichissement des timeout
 * */
void Serveur::clientAlive()
{
    // Tant qu'il y a des paquets reçus
     while (udpBroadSocket->hasPendingDatagrams())
     {
         QByteArray datagram;
         QList<QByteArray> mess;
         datagram.resize(udpBroadSocket->pendingDatagramSize());
         udpBroadSocket->readDatagram(datagram.data(), datagram.size());
         mess = datagram.split('#');
         Client *client = new Client(mess[0], mess[1]); // Création d'un objet client
         Client *pClient = clientIsIn(client, clients); // Vérification de la présence dans la liste

         // S'il n'y est pas
         if ( pClient == NULL )
         {
             client->status = 1; // Statut = alive
             client->timeOut = QTime::currentTime();
             connectTo(client);
             clients.append(client);
         }
         // S'il y est
         else
             // Refresh timeout du client trouvé
             (*pClient).timeOut = QTime::currentTime();
     }
}



/* Retourne un pointeur vers le client s'il est dans la liste
 * Retourne un pointeur NULL sinon
 * */
Client * Serveur::clientIsIn(Client *client, QList<Client *> &clients)
{
    QList<Client *>::Iterator clientIterator = clients.end();
    for( clientIterator = clients.begin(); clientIterator != clients.end(); ++clientIterator )
        if( *client == *clientIterator )
            return *clientIterator;

    return NULL;
}



/* Affiche les clients contenu dans la liste du serveur
 *  */
void Serveur::whoIsAlive()
{
    listeMessages->append("");
    listeMessages->append("Are alive on this server :");

    for(QList<Client *>::Iterator clientIterator = clients.begin(); clientIterator != clients.end(); ++clientIterator )
    {

        if( (*clientIterator)->status != 0 )
            listeMessages->append("\t" + (*clientIterator)->hostPort
                              + "  status: " + QString::number((*clientIterator)->status)
                              + "  timeOut: " + (*clientIterator)->timeOut.toString() + "s.");
    }
    listeMessages->append("");

    listeMessages->append("Are dead on this server :");
    for(QList<Client *>::Iterator clientIterator = clients.begin(); clientIterator != clients.end(); ++clientIterator )
    {

        if( (*clientIterator)->status == 0 )
            listeMessages->append("\t" + (*clientIterator)->hostPort
                              + "  status: " + QString::number((*clientIterator)->status)
                              + "  timeOut: " + (*clientIterator)->timeOut.toString() + "s.");
    }
    listeMessages->append("");
}


/* Actualisation des statuts des clients
 * */
void Serveur::deadCollector()
{
    for(QList<Client *>::Iterator clientIterator = clients.begin(); clientIterator != clients.end(); ++clientIterator )
        if( (*clientIterator)->status != 0 )
            if( (*clientIterator)->timeOut < QTime::currentTime().addSecs(-3))
            {
                (*clientIterator)->status = 0;
                listeMessages->append("\t" + (*clientIterator)->hostPort + " is dead !");
            }
}



/* Lance un client sur 127.0.0.1 et un port aléatoire
 *  */
void Serveur::startClient()
{
    QString program = "C:\\Users\\Erwan\\ISIMA\\3A\\Projet\\Qt\\deploy\\cltCore.exe";
    QProcess *myProcess = new QProcess(this);
    myProcess->startDetached(program);

    listeMessages->append("");
    listeMessages->append("Client démarré !");
    listeMessages->append("");
}



/* Ouverture d'une socket hyperviseur-client
 * */
void Serveur::connectTo(Client *client)
{
    listeMessages->append("    Tentative de connexion a " + client->hostPort);

        client->toClient->connectToHost(client->getHost(), client->getPort()); // On se connecte au serveur demandé
        client->toClient->waitForConnected();
        client->status = 2;
        listeMessages->append("    Connecte a " + client->hostPort);
        //connect(client->toClient, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
        connect(client->toClient, SIGNAL(disconnected()), this, SLOT(deconnexionClient()));
}



/* Deconnexion de la socket d'un client
 * */
void Serveur::deconnexionClient()
{
    // On détermine quel client se déconnecte
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
        return;


    foreach(Client *c, clients)
        if( c->toClient == socket )
        {
            c->toClient->close();
            c->toClient->deleteLater();
            listeMessages->append("    /!\\" + c->hostPort + " vient de se deconnecter.");
            break;
        }
}


/*

void Serveur::donneesRecues()
{
    // 1 : on reçoit un paquet (ou un sous-paquet) d'un des clients

    // On détermine quel client envoie le message (recherche du QTcpSocket du client)
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
        return;

    // Si tout va bien, on continue : on récupère le message
    QDataStream in(socket);

    if (tailleMessage == 0) // Si on ne connaît pas encore la taille du message, on essaie de la récupérer
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16)) // On n'a pas reçu la taille du message en entier
             return;

        in >> tailleMessage; // Si on a reçu la taille du message en entier, on la récupère
    }

    // Si on connaît la taille du message, on vérifie si on a reçu le message en entier
    if (socket->bytesAvailable() < tailleMessage) // Si on n'a pas encore tout reçu, on arrête la méthode
        return;


    // Si on arrive jusqu'à cette ligne, on peut récupérer le message entier
    QString sender, messageRecu;
    QTime time;
    in >> sender;
    in >> time;
    in >> messageRecu;


    // 2 : on renvoie le message à tous les clients
    //envoyerATous(hostPort, time, messageRecu);
    listeMessages->append(time.toString() + " - Recu de " + sender + " : " + messageRecu);

    // 3 : remise de la taille du message à 0 pour permettre la réception des futurs messages
    tailleMessage = 0;
}


void Serveur::envoyerATous(const QString hostPort, const QTime time, const QString &message)
{
    // Préparation du paquet
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0; // On écrit 0 au début du paquet pour réserver la place pour écrire la taille
    out << hostPort << time << message;
    out.device()->seek(0); // On se replace au début du paquet
    out << (quint16) (paquet.size() - sizeof(quint16)); // On écrase le 0 qu'on avait réservé par la longueur du message


    // Envoi du paquet préparé à tous les clients connectés au serveur
    for (int i = 0; i < clients.size(); i++)
    {
        clients[i]->write(paquet);
    }

}

void Serveur::envoyerATous(const QString &message)
{
    // Préparation du paquet
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0; // On écrit 0 au début du paquet pour réserver la place pour écrire la taille
    out << hostPort << QTime::currentTime() << message;
    out.device()->seek(0); // On se replace au début du paquet
    out << (quint16) (paquet.size() - sizeof(quint16)); // On écrase le 0 qu'on avait réservé par la longueur du message


    // Envoi du paquet préparé à tous les clients connectés au serveur
    for (int i = 0; i < clients.size(); i++)
    {
        clients[i]->write(paquet);
    }

}

void Serveur::envoyerAuxAutres(const QString &message)
{
    // Préparation du paquet
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0; // On écrit 0 au début du paquet pour réserver la place pour écrire la taille
    out << message;
    out.device()->seek(0); // On se replace au début du paquet
    out << (quint16) (paquet.size() - sizeof(quint16)); // On écrase le 0 qu'on avait réservé par la longueur du message


    // Envoi du paquet préparé à tous les clients connectés au serveur
    for (int i = 0; i < clients.size(); i++)
    {
        clients[i]->write(paquet);
    }

}
*/

