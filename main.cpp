#include <QCoreApplication>
#include "connexionhandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ConnexionHandler *connexionHandler = new ConnexionHandler();
    connexionHandler->start();

    return a.exec();
}
