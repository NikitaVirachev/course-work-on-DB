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

bool addMovie::checkField()
{
    if(ui->lineEdit->text().isEmpty()) return false;
    if(ui->lineEdit_2->text().isEmpty()) return false;
    if(ui->lineEdit_3->text().isEmpty()) return false;
    if(newPoster.isNull()) return false;
    if(ui->textEdit->document()->isEmpty()) return false;

    return true;
}

void addMovie::acceptData(QList<QString> listMovieID, QList <QString> listDirectorID, QList <QString> listProtagonistID, QList <QString> listStudioName)
{
    localListMovieID = listMovieID;
    foreach( QString value, listDirectorID )
    {
        ui->comboBox->addItem(value);
    }
    foreach( QString value, listProtagonistID )
    {
        ui->comboBox_2->addItem(value);
    }
    foreach( QString value, listStudioName )
    {
        ui->comboBox_3->addItem(value);
    }
}

void addMovie::on_pushButton_3_clicked()
{
    ui->progressBar->show();
    emit sendMovieSignal(ui->lineEdit->text(), ui->dateEdit->text(), ui->lineEdit_2->text(), ui->lineEdit_3->text(),
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
    if (checkField())
    {
        ui->pushButton_3->setEnabled(true);
    }
    else
    {
        ui->pushButton_3->setEnabled(false);
    }
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

void addMovie::on_lineEdit_textChanged(const QString &arg1)
{
    if (checkField())
    {
        ui->pushButton_3->setEnabled(true);
    }
    else
    {
        ui->pushButton_3->setEnabled(false);
    }
}


void addMovie::on_lineEdit_2_textChanged(const QString &arg1)
{
    if (checkField())
    {
        ui->pushButton_3->setEnabled(true);
    }
    else
    {
        ui->pushButton_3->setEnabled(false);
    }
}


void addMovie::on_lineEdit_3_textChanged(const QString &arg1)
{
    if (checkField())
    {
        ui->pushButton_3->setEnabled(true);
    }
    else
    {
        ui->pushButton_3->setEnabled(false);
    }
}


void addMovie::on_textEdit_textChanged()
{
    if (checkField())
    {
        ui->pushButton_3->setEnabled(true);
    }
    else
    {
        ui->pushButton_3->setEnabled(false);
    }
}

