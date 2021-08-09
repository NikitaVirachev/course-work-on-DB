#include "addstudio.h"
#include "ui_addstudio.h"

addStudio::addStudio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addStudio)
{
    ui->setupUi(this);

    ui->pushButton->setEnabled(false);
    ui->progressBar->setRange(0,0);
    ui->progressBar->hide();
}

addStudio::~addStudio()
{
    delete ui;
}

bool addStudio::checkField()
{
    if(ui->lineEdit->text().isEmpty()) return false;

    return true;
}

void addStudio::acceptData(QList<QString> listStudioName)
{
    checkListStudioName = listStudioName;
}

void addStudio::restartStudioWin()
{
    ui->progressBar->hide();
}

void addStudio::on_lineEdit_textChanged(const QString &arg1)
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


void addStudio::on_pushButton_clicked()
{
    bool presenceRepeat = false;
    foreach( QString value, checkListStudioName )
    {
        if (ui->lineEdit->text() == value)
        {
            presenceRepeat = true;
        }
    }

    if (presenceRepeat)
    {
        QMessageBox::information(this, "Ошибка", "Такая киностудия уже есть");
    }
    else
    {
        ui->progressBar->show();
        emit sendAddStudioSignal(ui->lineEdit->text());
    }
}

