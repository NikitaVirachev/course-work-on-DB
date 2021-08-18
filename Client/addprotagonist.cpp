#include "addprotagonist.h"
#include "ui_addprotagonist.h"

addProtagonist::addProtagonist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addProtagonist)
{
    ui->setupUi(this);

    ui->pushButton->setEnabled(false);
    ui->progressBar->setRange(0,0);
    ui->progressBar->hide();
}

addProtagonist::~addProtagonist()
{
    delete ui;
}

void addProtagonist::acceptData(QList<QString> listActorID)
{
    foreach( QString value, listActorID )
    {
        ui->comboBox->addItem(value);
    }
    ui->comboBox->addItem("");

    ui->pushButton->setEnabled(true);
}


void addProtagonist::on_pushButton_clicked()
{
    ui->progressBar->show();
    emit sendProtagonistSignal(ui->lineEdit->text(), ui->comboBox->currentText());
}

