#ifndef ADDPROTAGONIST_H
#define ADDPROTAGONIST_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class addProtagonist;
}

class addProtagonist : public QWidget
{
    Q_OBJECT

public:
    explicit addProtagonist(QWidget *parent = nullptr);
    ~addProtagonist();

    bool checkField();

public slots:
    void acceptData(QList <QString>);

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_clicked();

signals:
    void sendProtagonistSignal(QString, QString);

private:
    Ui::addProtagonist *ui;
};

#endif // ADDPROTAGONIST_H
