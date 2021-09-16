#include "myserver.h"
#include <QTextCodec>
#include <QApplication>
#undef main

int main(int argc, char *argv[])
{
    QTextCodec *coding = QTextCodec::codecForName("cp-1251");
    coding->setCodecForLocale(coding);

    QApplication a(argc, argv);

    myserver Server;
    Server.startServer();

    return a.exec();
}
