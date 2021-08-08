#ifndef ADDMOVIE_H
#define ADDMOVIE_H

#include <QWidget>
#include <QDebug>
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

    QList<QString> localListMovieID;
    QPixmap newPoster;
    QFile newScenario;

    bool checkField();

public slots:
    void acceptData(QList <QString>, QList <QString>, QList <QString>, QList <QString>);

private slots:

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_textEdit_textChanged();

signals:

    void sendMovieSignal(QString, QString, QString, QString, QString, QString, QString, QPixmap, QString);

private:
    Ui::addMovie *ui;
};

#endif // ADDMOVIE_H
