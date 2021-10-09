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
    QString oldTitle;
    QString newReleaseDate;
    QString oldReleaseDate;
    QString newBoxOffice;
    QString oldBoxOffice;
    QString newBudget;
    QString oldBudget;
    QString newProtagonistID;
    QString newDirectorID;
    QString newStudioName;
    QByteArray newPoster;
    QString newFirstNameActor;
    QString newLastNameActor;
    QString newDateOfBirthActor;

    QString oldDirectorID;
    QString newFirstName;
    QString oldFirstName;
    QString newLastName;
    QString oldLastName;
    QString newDateOfBirth;
    QString oldDateOfBirth;

    QString oldStudioName;

    QString newActorID;
    QString oldActorID;

    bool checkMovieID(QString movieID);
    bool checkInformationOnFilm(QString movieID, QString title, QString releaseDate, QString boxOffice, QString budget);
    bool checkDirectorID(QString directorID);
    bool checkInformationDirector(QString directorID, QString firstName, QString lastName, QString dateOfBirtday);
    bool checkStudioName(QString studioName);
    bool checkActorID(QString actorID);
    bool checkInformationActor(QString actorID, QString firstName, QString lastName, QString dateOfBirtday);
    bool checkProtagonistID(QString protagonistID);
    bool checkInformationProtagonist(QString protagonistID, QString oldName);

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
