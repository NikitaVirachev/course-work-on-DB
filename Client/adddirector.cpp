#include "adddirector.h"
#include "ui_adddirector.h"

addDirector::addDirector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addDirector)
{
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());
    ui->pushButton->setEnabled(false);
    ui->progressBar->setRange(0,0);
    ui->progressBar->hide();
}

addDirector::~addDirector()
{
    delete ui;
}

bool addDirector::checkField()
{
    if(ui->lineEdit->text().isEmpty()) return false;
    if(ui->lineEdit_2->text().isEmpty()) return false;

    return true;
}

void addDirector::on_lineEdit_textChanged(const QString &arg1)
{
    if (checkField())
    {
        ui->pushButton->setEnabled(true);
    }
    else
    {
        ui->pushButton->setEnabled(false);
    }
}


void addDirector::on_lineEdit_2_textChanged(const QString &arg1)
{
    if (checkField())
    {
        ui->pushButton->setEnabled(true);
    }
    else
    {
        ui->pushButton->setEnabled(false);
    }
}


void addDirector::on_pushButton_clicked()
{
    ui->progressBar->show();
    emit sendAddDirectorSignal(ui->lineEdit->text(), ui->lineEdit_2->text(), ui->dateEdit->text());
}

