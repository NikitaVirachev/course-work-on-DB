#ifndef MYSERVER_H
#define MYSERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QBuffer>
#include "socketthread.h"

class myserver : public QTcpServer
{
    Q_OBJECT

public:
    myserver(QWidget *parent = nullptr);
    ~myserver();

public slots:
    void startServer();
    void incomingConnection(qintptr socketDescriptor);

private:
    //Ui::myserver *ui;
};
#endif // MYSERVER_H
