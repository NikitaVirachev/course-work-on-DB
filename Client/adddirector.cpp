#include "adddirector.h"
#include "ui_adddirector.h"

addDirector::addDirector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addDirector)
{
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());
    ui->progressBar->setRange(0,0);
    ui->progressBar->hide();
}

addDirector::~addDirector()
{
    delete ui;
}

void addDirector::on_pushButton_clicked()
{
    QString date;

    if (ui->checkBox->checkState())
    {
        date = "";
    }
    else
    {
        date = ui->dateEdit->text();
    }

    ui->progressBar->show();
    emit sendAddDirectorSignal(ui->lineEdit->text(), ui->lineEdit_2->text(), date);
}


void addDirector::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == 2)
    {
        ui->dateEdit->setDate(QDate::fromString("1900-01-01", "yyyy-MM-dd"));
        ui->dateEdit->setEnabled(false);
        ui->pushButton->setEnabled(true);
    }
    else
    {
        ui->dateEdit->setDate(QDate::currentDate());
        ui->dateEdit->setEnabled(true);
    }
}

