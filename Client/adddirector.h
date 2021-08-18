#ifndef ADDDIRECTOR_H
#define ADDDIRECTOR_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class addDirector;
}

class addDirector : public QWidget
{
    Q_OBJECT

public:
    explicit addDirector(QWidget *parent = nullptr);
    ~addDirector();

private slots:

    void on_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

signals:

    void sendAddDirectorSignal(QString, QString, QString);

private:
    Ui::addDirector *ui;
};

#endif // ADDDIRECTOR_H
