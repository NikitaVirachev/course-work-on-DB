#ifndef UPDATEDIRECTOR_H
#define UPDATEDIRECTOR_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class updateDirector;
}

class updateDirector : public QWidget
{
    Q_OBJECT

public:
    explicit updateDirector(QWidget *parent = nullptr);
    ~updateDirector();

    QList<QString> localListDirectorID;
    QString oldFirstName;
    QString oldLastName;
    QString oldDateOfBirth;

public slots:
    void acceptDirectorID(QList <QString>);
    void acceptInformationDirectorUpd(QString, QString, QString);

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

signals:
    void sendDirectorIDSignalUpd(QString);
    void sendUpdateDirector(QString, QString, QString, QString, QString, QString, QString);

private:
    Ui::updateDirector *ui;
};

#endif // UPDATEDIRECTOR_H
