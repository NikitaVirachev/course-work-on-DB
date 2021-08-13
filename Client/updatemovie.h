#ifndef UPDATEMOVIE_H
#define UPDATEMOVIE_H

#include <QWidget>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

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

    bool posterFlag = false;
    bool scenarioFlag = false;

    QList<QString> localListMovieID;

public slots:
    void acceptMovieID(QList <QString>);
    void acceptInformationMovieUpd(QString, QString, QString, QString, QString, QString, QString, QByteArray, QByteArray, QList <QString>, QList <QString>, QList <QString>);

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
    void sendMovieIDSignalUpd(QString);
    void sendUpdateMovieWithPoster(QString, QString, QString, QString, QString, QString, QString, QString, QString, QPixmap);
    void sendUpdateMovieWithScenario(QString, QString, QString, QString, QString, QString, QString, QString, QString, QString);
    void sendUpdateMovieWithPosterAndScenario(QString, QString, QString, QString, QString, QString, QString, QString, QString, QPixmap, QString);
    void sendUpdateMovieWithout(QString, QString, QString, QString, QString, QString, QString, QString, QString);

private:
    Ui::updateMovie *ui;
};

#endif // UPDATEMOVIE_H
