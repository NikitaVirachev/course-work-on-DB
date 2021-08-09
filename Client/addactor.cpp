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

bool addActor::checkField()
{
    if(ui->lineEdit->text().isEmpty()) return false;
    if(ui->lineEdit_2->text().isEmpty()) return false;
    if(newActorPortrait.isNull()) return false;

    return true;
}

void addActor::on_lineEdit_textChanged(const QString &arg1)
{
    if (checkField())
    {
        ui->pushButton_2->setEnabled(true);
    }
    else
    {
        ui->pushButton_2->setEnabled(false);
    }
}


void addActor::on_lineEdit_2_textChanged(const QString &arg1)
{
    if (checkField())
    {
        ui->pushButton_2->setEnabled(true);
    }
    else
    {
        ui->pushButton_2->setEnabled(false);
    }
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
    if (checkField())
    {
        ui->pushButton_2->setEnabled(true);
    }
    else
    {
        ui->pushButton_2->setEnabled(false);
    }
}


void addActor::on_pushButton_2_clicked()
{
    ui->progressBar->show();
    emit sendActorSignal(ui->lineEdit->text(), ui->lineEdit_2->text(), ui->dateEdit->text(), newActorPortrait);
}

