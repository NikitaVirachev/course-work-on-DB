#ifndef ADDSTUDIO_H
#define ADDSTUDIO_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class addStudio;
}

class addStudio : public QWidget
{
    Q_OBJECT

public:
    explicit addStudio(QWidget *parent = nullptr);
    ~addStudio();

    QList<QString> checkListStudioName;

    bool checkField();

public slots:
    void acceptData(QList <QString>);
    void restartStudioWin();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_clicked();

signals:

    void sendAddStudioSignal(QString);

private:
    Ui::addStudio *ui;
};

#endif // ADDSTUDIO_H
