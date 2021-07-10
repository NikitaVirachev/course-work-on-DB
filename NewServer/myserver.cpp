#include "myserver.h"
#include "ui_myserver.h"

myserver::myserver(QWidget *parent)
    : QTcpServer(parent)
    , ui(new Ui::myserver)
{
    //ui->setupUi(this);

}

myserver::~myserver()
{
    delete ui;
}

void myserver::startServer(){

    if (this->listen(QHostAddress::Any,5555)){
        qDebug()<<"Сервер слушает";
    }
    else {
        qDebug()<<"Сервер не слушает: " << errorString();
    }

    /*QPixmap myPixmap("D:/1.png");
    QByteArray bytes;
    QBuffer buffer(&bytes);
    qDebug() << buffer.open(QIODevice::WriteOnly);
    qDebug() << myPixmap.save(&buffer, "PNG");
    qDebug() << bytes;*/
};

void myserver::incomingConnection(qintptr socketDescriptor)
{
    socketThread* thread = new socketThread(socketDescriptor);
    //connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
