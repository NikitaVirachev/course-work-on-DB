#ifndef UPDATEPROTAGONIST_H
#define UPDATEPROTAGONIST_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class updateProtagonist;
}

class updateProtagonist : public QWidget
{
    Q_OBJECT

public:
    explicit updateProtagonist(QWidget *parent = nullptr);
    ~updateProtagonist();

    QList<QString> localListProtagonistID;
    QString oldName;

public slots:
    void acceptProtagonistID(QList <QString>);
    void acceptInformationProtagonistUpd(QString, QString, QList <QString>);

signals:
    void sendProtagonistIDSignalUpd(QString);
    void sendUpdateProtagonist(QString, QString, QString, QString);

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::updateProtagonist *ui;
};

#endif // UPDATEPROTAGONIST_H
