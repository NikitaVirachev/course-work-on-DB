#ifndef ADDACTOR_H
#define ADDACTOR_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

namespace Ui {
class addActor;
}

class addActor : public QWidget
{
    Q_OBJECT

public:
    explicit addActor(QWidget *parent = nullptr);
    ~addActor();

    QPixmap newActorPortrait;

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

signals:

    void sendActorSignal(QString, QString, QString, QPixmap);

private:
    Ui::addActor *ui;
};

#endif // ADDACTOR_H
