#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    ui->lineEdit_5->setEchoMode(QLineEdit::Password);

    ui->lineEdit->setText("127.0.0.1");
    ui->lineEdit_2->setText("NIKITAPC");
    ui->lineEdit_3->setText("B1");
    ui->lineEdit_4->setText("user1");
    ui->lineEdit_5->setText("123");
}

login::~login()
{
    delete ui;
}

bool login::lineEditCheck()
{
    if(ui->lineEdit->text().isEmpty()) return false;
    if(ui->lineEdit_2->text().isEmpty()) return false;
    if(ui->lineEdit_3->text().isEmpty()) return false;
    if(ui->lineEdit_4->text().isEmpty()) return false;

    return true;
}

void login::on_lineEdit_textChanged(const QString &arg1)
{
    if (lineEditCheck())
    {
        ui->pushButton->setEnabled(true);
    }
    else
    {
        ui->pushButton->setEnabled(false);
    }
}

void login::on_lineEdit_2_textChanged(const QString &arg1)
{
    if (lineEditCheck())
    {
        ui->pushButton->setEnabled(true);
    }
    else
    {
        ui->pushButton->setEnabled(false);
    }
}

void login::on_lineEdit_3_textChanged(const QString &arg1)
{
    if (lineEditCheck())
    {
        ui->pushButton->setEnabled(true);
    }
    else
    {
        ui->pushButton->setEnabled(false);
    }
}

void login::on_lineEdit_4_textChanged(const QString &arg1)
{
    if (lineEditCheck())
    {
        ui->pushButton->setEnabled(true);
    }
    else
    {
        ui->pushButton->setEnabled(false);
    }
}

void login::on_lineEdit_5_textChanged(const QString &arg1)
{
    if (lineEditCheck())
    {
        ui->pushButton->setEnabled(true);
    }
    else
    {
        ui->pushButton->setEnabled(false);
    }
}

void login::on_pushButton_clicked()
{
    emit sendSignalToHost(ui->lineEdit->text());
    emit sendSignalToDB(ui->lineEdit_2->text(), ui->lineEdit_3->text(), ui->lineEdit_4->text(), ui->lineEdit_5->text());
}
