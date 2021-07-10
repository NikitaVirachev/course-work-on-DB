#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

    QMessageBox* mes;

    bool lineEditCheck();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_lineEdit_4_textChanged(const QString &arg1);

    void on_lineEdit_5_textChanged(const QString &arg1);

    void on_pushButton_clicked();

signals:
    void sendSignalToHost(QString);
    void sendSignalToDB(QString, QString, QString, QString);

private:
    Ui::login *ui;
};

#endif // LOGIN_H
