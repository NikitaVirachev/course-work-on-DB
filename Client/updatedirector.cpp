#include "updatedirector.h"
#include "ui_updatedirector.h"

updateDirector::updateDirector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::updateDirector)
{
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->progressBar->setRange(0,0);
    ui->progressBar->hide();
    ui->checkBox->setEnabled(false);
}

updateDirector::~updateDirector()
{
    delete ui;
}

void updateDirector::acceptDirectorID(QList<QString> listDirectorID)
{
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setEnabled(false);

    localListDirectorID = listDirectorID;
    ui->comboBox->addItem(" ");
    foreach( QString value, listDirectorID )
    {
        ui->comboBox->addItem(value);
    }
    ui->comboBox->setCurrentIndex(0);
}

void updateDirector::acceptInformationDirectorUpd(QString firstName, QString lastName, QString dateOfBirth)
{
    oldFirstName = firstName;
    oldLastName = lastName;
    oldDateOfBirth = dateOfBirth;
    ui->lineEdit_2->setText(firstName);
    ui->lineEdit_3->setText(lastName);
    if (dateOfBirth == "")
    {
        ui->checkBox->setChecked(true);
    }
    else
    {
        ui->dateEdit->setDate(QDate::fromString(dateOfBirth, "yyyy-MM-dd"));
        ui->dateEdit->setEnabled(true);
    }
    ui->pushButton->setEnabled(true);
    ui->progressBar->hide();
    ui->checkBox->setEnabled(true);
}

void updateDirector::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if (ui->comboBox->currentText() != " ")
    {
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        ui->dateEdit->setDate(QDate::currentDate());
        ui->dateEdit->setEnabled(false);
        ui->progressBar->show();
        emit sendDirectorIDSignalUpd(arg1);
    }
}


void updateDirector::on_pushButton_clicked()
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
    ui->pushButton->setEnabled(false);

    sendUpdateDirector(ui->comboBox->currentText(), ui->lineEdit_2->text(), ui->lineEdit_3->text(), date, oldFirstName, oldLastName, oldDateOfBirth);
}


void updateDirector::on_checkBox_stateChanged(int arg1)
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

