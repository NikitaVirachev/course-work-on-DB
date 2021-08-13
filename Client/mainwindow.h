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
#include "login.h"
#include "addmovie.h"
#include "adddirector.h"
#include "addstudio.h"
#include "addactor.h"
#include "addprotagonist.h"
#include "updatemovie.h"

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

    QJsonDocument doc;
    QJsonParseError docError;

public slots:
    void socketReady();
    void socketDisc();
    void conToHost(QString);
    void conToDB(QString, QString, QString, QString);
    void preparingAddMovie(QString, QString, QString, QString, QString, QString, QString, QPixmap, QString);
    void outputData();
    void preparingAddDirector(QString, QString, QString);
    void preparingAddStudio(QString);
    void preparingAddActor(QString, QString, QString, QPixmap);
    void preparingAddProtagonist(QString, QString);
    void prepareMovieInformationForUpdate(QString);
    void preparingUpdMovieWithPoster(QString, QString, QString, QString, QString, QString, QString, QString, QString, QPixmap);
    void preparingUpdMovieWithScenario(QString, QString, QString, QString, QString, QString, QString, QString, QString, QString);
    void preparingUpdMovieWithPosterAndScenario(QString, QString, QString, QString, QString, QString, QString, QString, QString, QPixmap, QString);
    void preparingUpdMovieWithout(QString, QString, QString, QString, QString, QString, QString, QString, QString);

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

signals:
    void sendTakeData(QList <QString>, QList <QString>, QList <QString>, QList <QString>);
    void sendStudioNames(QList <QString>);
    void sendRestartStudioWin();
    void sendAddProtagonist(QList <QString>);
    void sendUpdateMovie(QList <QString>);
    void sendInformationMovieUpd(QString, QString, QString, QString, QString, QString, QString, QByteArray, QByteArray, QList <QString>, QList <QString>, QList <QString>);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
