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
    qDebug() << "Я тут";
}

addMovie::~addMovie()
{
    delete ui;
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
    if ((newPoster.isNull()) || (newScenario.size() == 0) || (ui->lineEdit->text().isEmpty()) || (ui->lineEdit_2->text().isEmpty()) || (ui->lineEdit_3->text().isEmpty()))
    {
        QMessageBox::information(this, "Ошибка", "Необходимо заполнить все поля");
    }
    else
    {
        QString title = ui->lineEdit->text();
        QString releaseDate = ui->lineEdit_2->text();
        qDebug() << "Date: " << releaseDate;
    }
}

void addMovie::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Выбрать файл"), "D:/Учёба/2 семестр/БД/Решение/Файлы для бд", tr("Images (*.png)"));
    QPixmap newPixmap;
    newPixmap.load(fileName);
    newPoster = newPixmap;
    int w = ui->label_8->width();
    int h = ui->label_8->height();
    ui->label_8->setPixmap(newPixmap.scaled(w,h,Qt::KeepAspectRatio));
}

void addMovie::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("выбрать файл"), "D:/Учёба/2 семестр/БД/Решение/Файлы для бд", tr("Images (*.txt)"));
    QFile newScenario(fileName);
    if (newScenario.open(QIODevice::ReadOnly))
    {
        QString temp(newScenario.readAll());
        ui->textEdit->append(temp);
    }
}
