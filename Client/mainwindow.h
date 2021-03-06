#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QStandardItem>
#include <QBuffer>
#include <QPrinter>
#include <QTextDocument>
#include <QAxObject>
#include <qt_windows.h>
#include <QAxBase>
#include "login.h"
#include "addmovie.h"
#include "adddirector.h"
#include "addstudio.h"
#include "addactor.h"
#include "addprotagonist.h"
#include "updatemovie.h"
#include "updatedirector.h"
#include "updatestudio.h"
#include "updateactor.h"
#include "updateprotagonist.h"
#include "outputdirector.h"
#include "outputstudio.h"
#include "outputactor.h"
#include "outputprotagonist.h"
#include "storedprocedures.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    login* logwin;
    addMovie* addMovieWin;
    addDirector* addDirectorWin;
    addStudio* addStudioWin;
    addActor* addActorWin;
    addProtagonist* addProtagonistWin;
    updateMovie* updateMovieWin;
    updateDirector* updateDorectorWin;
    updateStudio* updateStudioWin;
    updateActor* updateActorWin;
    updateProtagonist* updateProtagonistWin;
    outputDirector* outputDirectorWin;
    outputStudio* outputStudioWin;
    outputActor* outputActorWin;
    outputProtagonist* outputProtagonistWin;
    storedProcedures* storedProceduresWin;

    QTcpSocket* socket;
    QByteArray Data;
    QByteArray itog;

    QStandardItemModel* movies = new QStandardItemModel(nullptr);
    QStandardItemModel* director = new QStandardItemModel(nullptr);
    QStandardItemModel* studio = new QStandardItemModel(nullptr);
    QStandardItemModel* protagonist = new QStandardItemModel(nullptr);
    QStandardItemModel* actor = new QStandardItemModel(nullptr);

    int requireSize;
    bool complexData = false;
    bool pictureArrives = false;
    bool actorPortraitArrives = false;
    bool scenarioArrives = false;
    bool updPosterArrives = false;
    bool updScenarioArrives = false;
    bool updPortraitArrives = false;
    bool actorPortraitOutputArrives = false;

    QByteArray newPoster;
    QByteArray newScenario;
    QByteArray newActorPortrait;

    QString updTitle;
    QString updReleaseDate;
    QString updBoxOffice;
    QString updBudget;
    QString updMovieStudioName;
    QString updMovieDirectorID;
    QString updMovieProtagonistID;
    QByteArray updPoster;
    QByteArray updScenario;
    QString updActorFirstName;
    QString updActorLastName;
    QString updActorDateOfBirth;
    QString globalActorID;
    QJsonArray buffInform;

    QJsonDocument doc;
    QJsonParseError docError;

    bool flagContextMenu;
    QString globalID;

public slots:
    void socketReady();
    void socketDisc();
    void conToHost(QString);
    void slotError(QAbstractSocket::SocketError);
    void conToDB(QString, QString, QString, QString);
    void preparingAddMovie(QString, QString, QString, QString, QString, QString, QString, QPixmap, QString);
    void outputData();
    void preparingAddDirector(QString, QString, QString);
    void preparingAddStudio(QString);
    void preparingAddActor(QString, QString, QString, QPixmap);
    void preparingAddProtagonist(QString, QString);
    void prepareMovieInformationForUpdate(QString);
    void preparingUpdMovieWithPoster(QString, QString, QString, QString, QString, QString, QString, QString, QPixmap, QString, QString, QString, QString);
    void preparingUpdMovieWithScenario(QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString);
    void preparingUpdMovieWithPosterAndScenario( QString, QString, QString, QString, QString, QString, QString, QString, QPixmap, QString, QString, QString, QString, QString);
    void preparingUpdMovieWithout(QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString);
    void prepareDirectorInformationForUpdate(QString);
    void preparingUpdDirector(QString, QString, QString, QString, QString, QString, QString);
    void preparingUpdStudio(QString, QString);
    void prepareActorInformationForUpdate(QString);
    void preparingUpdActorWithPortrait(QString, QString, QString, QString, QPixmap, QString, QString, QString);
    void preparingUpdActorWithout( QString, QString, QString, QString, QString, QString, QString);
    void prepareProtagonistInformationForUpdate(QString);
    void preparingUpdProtagonist(QString, QString, QString, QString);
    void customMenuReq(QPoint);
    void deleteMovie();
    void deleteDirector(QString);
    void deleteStudio(QString);
    void requestPhotoActor(QString actorID);
    void deleteActor(QString);
    void deleteProtagonist(QString);

private slots:
    void on_action_triggered();

    void on_tableView_clicked(const QModelIndex &index);

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_action_4_triggered();

    void on_action_5_triggered();

    void on_action_6_triggered();

    void on_action_7_triggered();

    void on_action_8_triggered();

    void on_action_9_triggered();

    void on_action_10_triggered();

    void on_action_11_triggered();

    void on_action_12_triggered();

    void on_action_13_triggered();

    void on_action_14_triggered();

    void on_action_15_triggered();

    void on_action_16_triggered();

    void on_action_17_triggered();

    void on_action_19_triggered();

    void on_actionExcel_triggered();

signals:
    void sendTakeData(QList <QString>, QList <QString>, QList <QString>, QList <QString>);
    void sendStudioNames(QList <QString>);
    void sendRestartStudioWin();
    void sendAddProtagonist(QList <QString>);
    void sendUpdateMovie(QList <QString>);
    void sendInformationMovieUpd(QString, QString, QString, QString, QString, QString, QString, QByteArray, QByteArray, QList <QString>, QList <QString>, QList <QString>);
    void sendUpdateDirector(QList <QString>);
    void sendInformationDirectorUpd(QString, QString, QString);
    void sendUpdateStudio(QList <QString>);
    void sendUpdateActor(QList <QString>);
    void sendInformationActorUpd(QString, QString, QString, QByteArray);
    void sendUpdateProtagonist(QList <QString>);
    void sendInformationProtagonistUpd(QString, QString, QList <QString>);
    void sendOutputDirector(QJsonArray);
    void sendOutputStudio(QJsonArray);
    void sendOutputActor(QJsonArray);
    void sendOutputActorPortrait(QByteArray, QJsonArray);
    void sendOutputProtagonist(QJsonArray, QJsonArray);
    void sendStoredProcedures(QJsonArray, QJsonArray);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
