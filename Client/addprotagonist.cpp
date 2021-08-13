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

bool addProtagonist::checkField()
{
    if(ui->lineEdit->text().isEmpty()) return false;

    return true;
}

void addProtagonist::acceptData(QList<QString> listProtagonistID)
{
    foreach( QString value, listProtagonistID )
    {
        ui->comboBox->addItem(value);
    }
}

void addProtagonist::on_lineEdit_textChanged(const QString &arg1)
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


void addProtagonist::on_pushButton_clicked()
{
    ui->progressBar->show();
    emit sendProtagonistSignal(ui->lineEdit->text(), ui->comboBox->currentText());
}

