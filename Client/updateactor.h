#ifndef UPDATEACTOR_H
#define UPDATEACTOR_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include "paint.h"

namespace Ui {
class updateActor;
}

class updateActor : public QWidget
{
    Q_OBJECT

public:
    explicit updateActor(QWidget *parent = nullptr);
    ~updateActor();

    QPixmap newActorPortrait;
    QPixmap outPortrait;

    bool portraitFlag = false;

    QList<QString> localListActorID;

    Paint* paintWin;

public slots:
    void acceptActorID(QList <QString>);
    void acceptInformationActorUpd(QString, QString, QString, QByteArray);
    void getNewPortrait(QPixmap);

signals:
    void sendActorIDSignalUpd(QString);
    void sendUpdateActorWithPortrait(QString, QString, QString, QString, QString, QPixmap);
    void sendUpdateActorWithout(QString, QString, QString, QString, QString);
    void sendImageForChange(QPixmap);

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_pushButton_3_clicked();

private:
    Ui::updateActor *ui;
};

#endif // UPDATEACTOR_H
