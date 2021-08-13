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
}

updateDirector::~updateDirector()
{
    delete ui;
}

void updateDirector::acceptDirectorID(QList<QString> listDirectorID)
{
    ui->lineEdit->clear();
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
    ui->lineEdit_2->setText(firstName);
    ui->lineEdit_3->setText(lastName);
    ui->dateEdit->setDate(QDate::fromString(dateOfBirth, "yyyy-MM-dd"));
    ui->dateEdit->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->progressBar->hide();
}

void updateDirector::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if (ui->comboBox->currentText() != " ")
    {
        ui->lineEdit->clear();
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
    bool uniqueID = true;
    bool directorIDFlag = false;
    QString directorID = "";

    if (ui->lineEdit->text() == "")
    {
        directorID = ui->comboBox->currentText();
        directorIDFlag = true;
    }
    else
    {
        for (int i = 0; i < localListDirectorID.length(); ++i)
        {
            if (localListDirectorID[i] == ui->lineEdit->text())
            {
                uniqueID = false;
            }
        }

        if (uniqueID)
        {
            directorID = ui->lineEdit->text();
            directorIDFlag = true;
        }
        else
        {
            QMessageBox::information(this,"Ошибка","Необходимо использовать уникальный номер");
        }
    }

    if (directorIDFlag)
    {
        ui->progressBar->show();
        ui->pushButton->setEnabled(false);

        sendUpdateDirector(ui->comboBox->currentText(), directorID, ui->lineEdit_2->text(), ui->lineEdit_3->text(), ui->dateEdit->text());
    }
}
