#include "myserver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    myserver Server;
    Server.startServer();

    return a.exec();
}
