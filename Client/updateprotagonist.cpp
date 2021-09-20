#include "updateprotagonist.h"
#include "ui_updateprotagonist.h"

updateProtagonist::updateProtagonist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::updateProtagonist)
{
    ui->setupUi(this);

    ui->pushButton->setEnabled(false);
    ui->progressBar->setRange(0,0);
    ui->progressBar->hide();
}

updateProtagonist::~updateProtagonist()
{
    delete ui;
}

void updateProtagonist::acceptProtagonistID(QList<QString> listProtagonistID)
{
    ui->lineEdit->clear();
    ui->comboBox_2->clear();
    ui->progressBar->hide();

    localListProtagonistID = listProtagonistID;
    ui->comboBox->addItem(" ");
    foreach( QString value, listProtagonistID )
    {
        ui->comboBox->addItem(value);
    }
    ui->comboBox->setCurrentIndex(0);
}

void updateProtagonist::acceptInformationProtagonistUpd(QString name, QString actorID, QList<QString> listActorID)
{
    ui->comboBox_2->clear();

    for (int i = 0; i < listActorID.length(); ++i)
    {
        ui->comboBox_2->addItem(listActorID[i]);
        //qDebug() << "listActorID[i]: " << listActorID[i] << ", actorID: " << actorID;
        if (listActorID[i] == actorID)
        {
            ui->comboBox_2->setCurrentIndex(i);
        }
    }
    ui->comboBox_2->addItem("");
    if (actorID == "0")
    {
        ui->comboBox_2->setCurrentIndex(listActorID.length()+1);
    }

    ui->lineEdit->setText(name);

    ui->pushButton->setEnabled(true);
    ui->comboBox->setEnabled(true);
    ui->progressBar->hide();
}

void updateProtagonist::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if (ui->comboBox->currentText() != " ")
    {
        ui->lineEdit->clear();
        ui->comboBox_2->clear();
        ui->progressBar->show();
        ui->comboBox->setEnabled(false);
        emit sendProtagonistIDSignalUpd(arg1);
    }
}


void updateProtagonist::on_pushButton_clicked()
{
    ui->progressBar->show();
    ui->pushButton->setEnabled(false);
    sendUpdateProtagonist(ui->comboBox->currentText(), ui->lineEdit->text(), ui->comboBox_2->currentText());
}

