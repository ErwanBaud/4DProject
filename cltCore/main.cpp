# include <QCoreApplication>
# include <cstring>
# include <iostream>
# include "ClientCore.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Client clt;
    QTextStream s(stdin);
    QString mess;
    quint16 i = 1000;

    cout << "Client" << endl;

    //clt.initConnexion("127.0.0.1", 50885);

//    do
//    {
//        cout << "Envoyer :" << endl;
//        mess = s.readLine();
//        clt.envoyer(mess);
//        //--i;
//    }
//    while(1);

    return a.exec();
}
