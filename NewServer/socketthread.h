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
#include <QAxObject>
#include <QTextStream>
#include <QPrinter>
#include <QTextDocument>

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
    QString directorID;
    QString actorID;
    QString protagonistID;

    int requireSize;
    bool complexData = false;
    bool posterArrives = false;
    bool actorPortraitArrives = false;
    bool scenarioArrives = false;
    bool newPosterArrives = false;
    bool newScenarioArrives = false;
    bool posterAndScenario = false;
    bool newActorPortraitArrives = false;

    QString newMovieID;
    QString oldMovieID;
    QString newTitle;
    QString newReleaseDate;
    QString newBoxOffice;
    QString newBudget;
    QString newProtagonistID;
    QString newDirectorID;
    QString newStudioName;
    QByteArray newPoster;
    QString newFirstNameActor;
    QString newLastNameActor;
    QString newDateOfBirthActor;

    QString oldDirectorID;
    QString newFirstName;
    QString newLastName;
    QString newDateOfBirth;

    QString oldStudioName;

    QString newActorID;
    QString oldActorID;

    bool checkMovieID(QString movieID);
    bool checkInformationOnFilm(QString movieID, QString title, QString releaseDate, QString boxOffice, QString budget);
    bool checkDirectorID(QString directorID);
    bool checkStudioName(QString studioName);
    bool checkActorID(QString actorID);
    bool checkProtagonistID(QString protagonistID);

public slots:
    void mySocketReady();
    void socketDisc();
    void downloadInformationAboutFilms();

private:
    int socketDescriptor;
    QTcpSocket* socket;
    QSqlDatabase db;
};

#endif // SOCKETTHREAD_H
