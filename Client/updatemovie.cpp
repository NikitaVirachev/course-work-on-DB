#include "updatemovie.h"
#include "ui_updatemovie.h"

updateMovie::updateMovie(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::updateMovie)
{
    ui->setupUi(this);

    ui->lineEdit_2->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));
    ui->lineEdit_3->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));
    ui->lineEdit_4->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));
    ui->label_10->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->progressBar->setRange(0,0);
    ui->progressBar->hide();
}

updateMovie::~updateMovie()
{
    delete ui;
}

void updateMovie::acceptMovieID(QList<QString> listMovieID)
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setEnabled(false);
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_4->clear();
    ui->label_10->clear();
    ui->textEdit->clear();
    ui->progressBar->hide();

    localListMovieID = listMovieID;
    ui->comboBox->addItem(" ");
    foreach( QString value, listMovieID )
    {
        ui->comboBox->addItem(value);
    }
    ui->comboBox->setCurrentIndex(0);
}

void updateMovie::acceptInformationMovieUpd(QString title, QString releaseDate, QString boxOffice, QString budget, QString studioName, QString directorID, QString protagonistID, QByteArray poster, QByteArray scenario, QList<QString> listDirectorID, QList<QString> listProtagonistID, QList<QString> listStudioName)
{
    for (int i = 0; i < listDirectorID.length(); ++i)
    {
        ui->comboBox_2->addItem(listDirectorID[i]);
        if (listDirectorID[i] == directorID)
        {
            ui->comboBox_2->setCurrentIndex(i);
        }
    }
    ui->comboBox_2->addItem(" ");

    for (int i = 0; i < listProtagonistID.length(); ++i)
    {
        ui->comboBox_3->addItem(listProtagonistID[i]);
        if (listProtagonistID[i] == protagonistID)
        {
            ui->comboBox_3->setCurrentIndex(i);
        }
    }
    ui->comboBox_3->addItem(" ");

    for (int i = 0; i < listStudioName.length(); ++i)
    {
        ui->comboBox_4->addItem(listStudioName[i]);
        if (listStudioName[i] == studioName)
        {
            ui->comboBox_4->setCurrentIndex(i);
        }
    }
    ui->comboBox_4->addItem(" ");

    ui->lineEdit->setText(title);
    ui->dateEdit->setDate(QDate::fromString(releaseDate, "yyyy-MM-dd"));
    ui->dateEdit->setEnabled(true);
    ui->lineEdit_2->setText(boxOffice);
    ui->lineEdit_3->setText(budget);

    QPixmap outPoster = QPixmap();
    outPoster.loadFromData(poster,"PNG");
    int w = ui->label_10->width();
    int h = ui->label_10->height();
    ui->label_10->setPixmap(outPoster.scaled(w,h,Qt::KeepAspectRatio));

    ui->textEdit->setText(scenario);

    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);

    ui->progressBar->hide();
}

void updateMovie::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if (ui->comboBox->currentText() != " ")
    {
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        ui->lineEdit_4->clear();
        ui->dateEdit->setDate(QDate::currentDate());
        ui->dateEdit->setEnabled(false);
        ui->comboBox_2->clear();
        ui->comboBox_3->clear();
        ui->comboBox_4->clear();
        ui->label_10->clear();
        ui->textEdit->clear();
        ui->progressBar->show();
        emit sendMovieIDSignalUpd(arg1);
    }
}


void updateMovie::on_pushButton_3_clicked()
{
    bool uniqueID = true;
    bool movieIDFlag = false;
    QString movieID = "";

    if (ui->lineEdit_4->text() == "")
    {
        movieID = ui->comboBox->currentText();
        movieIDFlag = true;
    }
    else
    {
        for (int i = 0; i < localListMovieID.length(); ++i)
        {
            if (localListMovieID[i] == ui->lineEdit_4->text())
            {
                uniqueID = false;
            }
        }

        if (uniqueID)
        {
            movieID = ui->lineEdit_4->text();
            movieIDFlag = true;
        }
        else
        {
            QMessageBox::information(this,"Ошибка","Необходимо использовать уникальный номер");
        }
    }

    if (movieIDFlag)
    {
        ui->progressBar->show();
        ui->pushButton_3->setEnabled(false);

        if (posterFlag && scenarioFlag)
        {
            sendUpdateMovieWithPosterAndScenario(ui->comboBox->currentText(), movieID, ui->lineEdit->text(), ui->dateEdit->text(), ui->lineEdit_2->text(), ui->lineEdit_3->text(),
                                      ui->comboBox_2->currentText(), ui->comboBox_3->currentText(), ui->comboBox_4->currentText(), newPoster,
                                      ui->textEdit->toPlainText());
        }
        else if (posterFlag)
        {
            sendUpdateMovieWithPoster(ui->comboBox->currentText(), movieID, ui->lineEdit->text(), ui->dateEdit->text(), ui->lineEdit_2->text(), ui->lineEdit_3->text(),
                                      ui->comboBox_2->currentText(), ui->comboBox_3->currentText(), ui->comboBox_4->currentText(), newPoster);
        }
        else if (scenarioFlag)
        {
            sendUpdateMovieWithScenario(ui->comboBox->currentText(), movieID, ui->lineEdit->text(), ui->dateEdit->text(), ui->lineEdit_2->text(), ui->lineEdit_3->text(),
                                        ui->comboBox_2->currentText(), ui->comboBox_3->currentText(), ui->comboBox_4->currentText(),
                                        ui->textEdit->toPlainText());
        }
        else
        {
            sendUpdateMovieWithout(ui->comboBox->currentText(), movieID, ui->lineEdit->text(), ui->dateEdit->text(), ui->lineEdit_2->text(), ui->lineEdit_3->text(),
                                   ui->comboBox_2->currentText(), ui->comboBox_3->currentText(), ui->comboBox_4->currentText());
        }
    }
}


void updateMovie::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Выбрать файл"), "C:/Users/Nikita/Desktop/Учёба/БД/Решение/Файлы для бд", tr("Images (*.png)"));
    QPixmap newPixmap;
    newPixmap.load(fileName);
    newPoster = newPixmap;
    int w = ui->label_10->width();
    int h = ui->label_10->height();
    ui->label_10->setPixmap(newPixmap.scaled(w,h,Qt::KeepAspectRatio));

    posterFlag = true;
}


void updateMovie::on_pushButton_2_clicked()
{
    ui->textEdit->clear();
    QString fileName = QFileDialog::getOpenFileName(this, tr("выбрать файл"), "C:/Users/Nikita/Desktop/Учёба/БД/Решение/Файлы для бд", tr("Images (*.txt)"));
    QFile scenario(fileName);
    if (scenario.open(QIODevice::ReadOnly))
    {
        QString temp(scenario.readAll());
        ui->textEdit->append(temp);
    }

    scenarioFlag = true;
}

