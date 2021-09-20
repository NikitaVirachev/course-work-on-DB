#ifndef UPDATEMOVIE_H
#define UPDATEMOVIE_H

#include <QWidget>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include "paint.h"

namespace Ui {
class updateMovie;
}

class updateMovie : public QWidget
{
    Q_OBJECT

public:
    explicit updateMovie(QWidget *parent = nullptr);
    ~updateMovie();

    QPixmap newPoster;
    QFile newScenario;
    QPixmap outPoster;

    bool posterFlag = false;
    bool scenarioFlag = false;

    QList<QString> localListMovieID;
    QString localScenario;

    Paint* paintWin;

public slots:
    void acceptMovieID(QList <QString>);
    void acceptInformationMovieUpd(QString, QString, QString, QString, QString, QString, QString, QByteArray, QByteArray, QList <QString>, QList <QString>, QList <QString>);
    void getNewPoster(QPixmap);

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_textEdit_textChanged();

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_pushButton_4_clicked();

signals:
    void sendMovieIDSignalUpd(QString);
    void sendUpdateMovieWithPoster(QString, QString, QString, QString, QString, QString, QString, QString, QPixmap);
    void sendUpdateMovieWithScenario(QString, QString, QString, QString, QString, QString, QString, QString, QString);
    void sendUpdateMovieWithPosterAndScenario(QString, QString, QString, QString, QString, QString, QString, QString, QPixmap, QString);
    void sendUpdateMovieWithout(QString, QString, QString, QString, QString, QString, QString, QString);
    void sendImageForChange(QPixmap);

private:
    Ui::updateMovie *ui;
};

#endif // UPDATEMOVIE_H
