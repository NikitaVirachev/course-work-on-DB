#include "myserver.h"
#include "ui_myserver.h"
#include <iostream>

myserver::myserver(QWidget *parent)
    : QTcpServer(parent)
{
    //ui->setupUi(this);

}

myserver::~myserver()
{
    //delete ui;
}

void myserver::startServer(){
    std::setlocale(LC_ALL, "Rus");
    QTextStream cout(stdout); cout.setCodec("CP1251");

    if (this->listen(QHostAddress::Any,5555)){
        qDebug()<<"Сервер слушает";
    }
    else {
        qDebug()<<"Сервер не слушает: " << errorString();
    }
};

void myserver::incomingConnection(qintptr socketDescriptor)
{
    socketThread* thread = new socketThread(socketDescriptor);
    thread->start();
}
