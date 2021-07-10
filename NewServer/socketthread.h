#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QPixmap>

class socketThread : public QThread
{
    Q_OBJECT
public:
    socketThread(qintptr descriptor);
    ~socketThread();

    void run();

    QByteArray Data;
    QByteArray itog;
    QJsonDocument doc;
    QJsonParseError docError;
    QByteArray baPoster;
    QByteArray baActorPortrait;
    QByteArray baScenario;
    QString movieID;

public slots:
    void mySocketReady();
    void socketDisc();

private:
    int socketDescriptor;
    QTcpSocket* socket;
    QSqlDatabase db;
};

#endif // SOCKETTHREAD_H
