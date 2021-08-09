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

    bool checkField();

    QPixmap newActorPortrait;

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:

    void sendActorSignal(QString, QString, QString, QPixmap);

private:
    Ui::addActor *ui;
};

#endif // ADDACTOR_H
