#ifndef ADDMOVIE_H
#define ADDMOVIE_H

#include <QWidget>
#include <QDebug>
#include <QTcpSocket>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class addMovie;
}

class addMovie : public QWidget
{
    Q_OBJECT

public:
    explicit addMovie(QWidget *parent = nullptr);
    ~addMovie();

    QTcpSocket* localSocket;
    QList<QString> localListMovieID;
    QPixmap newPoster;
    QFile newScenario;

public slots:
    void acceptData(QList <QString>, QList <QString>, QList <QString>, QList <QString>);

private slots:

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::addMovie *ui;
};

#endif // ADDMOVIE_H
