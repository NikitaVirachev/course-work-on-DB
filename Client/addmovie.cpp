#include "addmovie.h"
#include "ui_addmovie.h"

addMovie::addMovie(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addMovie)
{
    ui->setupUi(this);

    ui->lineEdit_2->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));
    ui->lineEdit_3->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));
    ui->label_8->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->pushButton_3->setEnabled(false);
    ui->progressBar->setRange(0,0);
    ui->progressBar->hide();
}

addMovie::~addMovie()
{
    delete ui;
}

void addMovie::acceptData(QList<QString> listMovieID, QList <QString> listDirectorID, QList <QString> listProtagonistID, QList <QString> listStudioName)
{
    localListMovieID = listMovieID;
    foreach( QString value, listProtagonistID )
    {
        ui->comboBox->addItem(value);
    }
    ui->comboBox->addItem("");
    ui->comboBox->setCurrentIndex(listProtagonistID.length()+1);
    foreach( QString value, listDirectorID )
    {
        ui->comboBox_2->addItem(value);
    }
    ui->comboBox_2->addItem("");
    ui->comboBox_2->setCurrentIndex(listDirectorID.length()+1);
    foreach( QString value, listStudioName )
    {
        ui->comboBox_3->addItem(value);
    }
    ui->comboBox_3->addItem("");
    ui->comboBox_3->setCurrentIndex(listStudioName.length()+1);
}

void addMovie::on_pushButton_3_clicked()
{
    ui->progressBar->show();

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
        newPoster = QPixmap();
    }

    emit sendMovieSignal(ui->lineEdit->text(), date, ui->lineEdit_2->text(), ui->lineEdit_3->text(),
                         ui->comboBox->currentText(), ui->comboBox_2->currentText(), ui->comboBox_3->currentText(), newPoster,
                         ui->textEdit->toPlainText());
}

void addMovie::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Выбрать файл"), "C:/Users/Nikita/Desktop/Учёба/БД/Решение/Файлы для бд", tr("Images (*.png)"));
    QPixmap newPixmap;
    newPixmap.load(fileName);
    newPoster = newPixmap;
    int w = ui->label_8->width();
    int h = ui->label_8->height();
    ui->label_8->setPixmap(newPixmap.scaled(w,h,Qt::KeepAspectRatio));
    ui->pushButton_3->setEnabled(true);
}

void addMovie::on_pushButton_2_clicked()
{
    ui->textEdit->clear();
    QString fileName = QFileDialog::getOpenFileName(this, tr("выбрать файл"), "C:/Users/Nikita/Desktop/Учёба/БД/Решение/Файлы для бд", tr("Images (*.txt)"));
    QFile scenario(fileName);
    if (scenario.open(QIODevice::ReadOnly))
    {
        QString temp(scenario.readAll());
        ui->textEdit->append(temp);
    }
}


void addMovie::on_checkBox_stateChanged(int arg1)
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


void addMovie::on_checkBox_2_stateChanged(int arg1)
{
    if (arg1 == 2)
    {
        ui->pushButton->setEnabled(false);
        ui->pushButton_3->setEnabled(true);
        ui->label_8->clear();
    }
    else
    {
        ui->pushButton->setEnabled(true);
        ui->pushButton_3->setEnabled(false);
    }
}

