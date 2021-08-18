#include "addactor.h"
#include "ui_addactor.h"

addActor::addActor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addActor)
{
    ui->setupUi(this);
    ui->label_4->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->pushButton_2->setEnabled(false);
    ui->progressBar->setRange(0,0);
    ui->progressBar->hide();
}

addActor::~addActor()
{
    delete ui;
}

void addActor::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Выбрать файл"), "C:/Users/Nikita/Desktop/Учёба/БД/Решение/Файлы для бд", tr("Images (*.png)"));
    QPixmap newPixmap;
    newPixmap.load(fileName);
    newActorPortrait = newPixmap;
    int w = ui->label_4->width();
    int h = ui->label_4->height();
    ui->label_4->setPixmap(newPixmap.scaled(w,h,Qt::KeepAspectRatio));
    ui->pushButton_2->setEnabled(true);
}


void addActor::on_pushButton_2_clicked()
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

    if (ui->checkBox_2->checkState())
    {
        newActorPortrait = QPixmap();
    }

    ui->progressBar->show();
    emit sendActorSignal(ui->lineEdit->text(), ui->lineEdit_2->text(), date, newActorPortrait);
}


void addActor::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == 2)
    {
        ui->dateEdit->setDate(QDate::fromString("1900-01-01", "yyyy-MM-dd"));
        ui->dateEdit->setEnabled(false);
    }
    else
    {
        ui->dateEdit->setDate(QDate::currentDate());
        ui->dateEdit->setEnabled(true);
    }
}


void addActor::on_checkBox_2_stateChanged(int arg1)
{
    if (arg1 == 2)
    {
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(true);
        ui->label_4->clear();
    }
    else
    {
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(false);
    }
}

