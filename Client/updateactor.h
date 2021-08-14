#ifndef UPDATEACTOR_H
#define UPDATEACTOR_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

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

    bool portraitFlag = false;

    QList<QString> localListActorID;

public slots:
    void acceptActorID(QList <QString>);
    void acceptInformationActorUpd(QString, QString, QString, QByteArray);

signals:
    void sendActorIDSignalUpd(QString);
    void sendUpdateActorWithPortrait(QString, QString, QString, QString, QString, QPixmap);
    void sendUpdateActorWithout(QString, QString, QString, QString, QString);

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::updateActor *ui;
};

#endif // UPDATEACTOR_H
