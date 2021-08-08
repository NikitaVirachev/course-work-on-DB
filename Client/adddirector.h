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

    bool checkField();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_pushButton_clicked();

signals:

    void sendAddDirectorSignal(QString, QString, QString);

private:
    Ui::addDirector *ui;
};

#endif // ADDDIRECTOR_H
