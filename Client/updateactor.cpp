#include "updateactor.h"
#include "ui_updateactor.h"

updateActor::updateActor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::updateActor)
{
    ui->setupUi(this);

    ui->label_7->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->progressBar->setRange(0,0);
    ui->progressBar->hide();
}

updateActor::~updateActor()
{
    delete ui;
}

void updateActor::acceptActorID(QList<QString> listActorID)
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setEnabled(false);
    ui->label_7->clear();
    ui->progressBar->hide();

    localListActorID = listActorID;
    ui->comboBox->addItem(" ");
    foreach( QString value, listActorID )
    {
        ui->comboBox->addItem(value);
    }
    ui->comboBox->setCurrentIndex(0);
}

void updateActor::acceptInformationActorUpd(QString firstName, QString lastName, QString dateOfBirth, QByteArray actorPortrait)
{
    ui->lineEdit_2->setText(firstName);
    ui->lineEdit_3->setText(lastName);
    ui->dateEdit->setDate(QDate::fromString(dateOfBirth, "yyyy-MM-dd"));
    ui->dateEdit->setEnabled(true);

    QPixmap outPoster = QPixmap();
    outPoster.loadFromData(actorPortrait,"PNG");
    int w = ui->label_7->width();
    int h = ui->label_7->height();
    ui->label_7->setPixmap(outPoster.scaled(w,h,Qt::KeepAspectRatio));

    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->progressBar->hide();
}

void updateActor::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if (ui->comboBox->currentText() != " ")
    {
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        ui->dateEdit->setDate(QDate::currentDate());
        ui->dateEdit->setEnabled(false);
        ui->label_7->clear();
        ui->progressBar->show();
        emit sendActorIDSignalUpd(arg1);
    }
}


void updateActor::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Выбрать файл"), "C:/Users/Nikita/Desktop/Учёба/БД/Решение/Файлы для бд", tr("Images (*.png)"));
    QPixmap newPixmap;
    newPixmap.load(fileName);
    newActorPortrait = newPixmap;
    int w = ui->label_7->width();
    int h = ui->label_7->height();
    ui->label_7->setPixmap(newPixmap.scaled(w,h,Qt::KeepAspectRatio));

    portraitFlag = true;
}


void updateActor::on_pushButton_2_clicked()
{
    bool uniqueID = true;
    bool actorIDFlag = false;
    QString actorID = "";

    if (ui->lineEdit->text() == "")
    {
        actorID = ui->comboBox->currentText();
        actorIDFlag = true;
    }
    else
    {
        for (int i = 0; i < localListActorID.length(); ++i)
        {
            if (localListActorID[i] == ui->lineEdit->text())
            {
                uniqueID = false;
            }
        }

        if (uniqueID)
        {
            actorID = ui->lineEdit->text();
            actorIDFlag = true;
        }
        else
        {
            QMessageBox::information(this,"Ошибка","Необходимо использовать уникальный номер");
        }
    }

    if (actorIDFlag)
    {
        ui->progressBar->show();
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);

        if (portraitFlag)
        {
            emit sendUpdateActorWithPortrait(ui->comboBox->currentText(), actorID, ui->lineEdit_2->text(), ui->lineEdit_3->text(), ui->dateEdit->text(), newActorPortrait);
            portraitFlag = false;
        }
        else
        {
            emit sendUpdateActorWithout(ui->comboBox->currentText(), actorID, ui->lineEdit_2->text(), ui->lineEdit_3->text(), ui->dateEdit->text());
        }
    }
}

