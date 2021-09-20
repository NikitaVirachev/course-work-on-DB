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

    if (dateOfBirth == "")
    {
        ui->checkBox->setChecked(true);
    }
    else
    {
        ui->dateEdit->setDate(QDate::fromString(dateOfBirth, "yyyy-MM-dd"));
        ui->dateEdit->setEnabled(true);
    }

    if (actorPortrait != "NULL")
    {
        outPortrait = QPixmap();
        outPortrait.loadFromData(actorPortrait,"PNG");
        int w = ui->label_7->width();
        int h = ui->label_7->height();
        ui->label_7->setPixmap(outPortrait.scaled(w,h,Qt::KeepAspectRatio));
        ui->pushButton->setEnabled(true);
    }
    else
    {
        ui->checkBox_2->setChecked(true);
    }

    ui->pushButton_2->setEnabled(true);
    ui->progressBar->hide();
}

void updateActor::getNewPortrait(QPixmap newImage)
{
    paintWin->close();
    portraitFlag = true;
    outPortrait = newImage;
    newActorPortrait = newImage;
    int w = ui->label_7->width();
    int h = ui->label_7->height();
    ui->label_7->setPixmap(outPortrait.scaled(w,h,Qt::KeepAspectRatio));
}

void updateActor::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if (ui->comboBox->currentText() != " ")
    {
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
    outPortrait = newPixmap;
    int w = ui->label_7->width();
    int h = ui->label_7->height();
    ui->label_7->setPixmap(newPixmap.scaled(w,h,Qt::KeepAspectRatio));

    portraitFlag = true;
}


void updateActor::on_pushButton_2_clicked()
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
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

    if (portraitFlag)
    {
        emit sendUpdateActorWithPortrait(ui->comboBox->currentText(), ui->lineEdit_2->text(), ui->lineEdit_3->text(), date, newActorPortrait);
        portraitFlag = false;
    }
    else
    {
        emit sendUpdateActorWithout(ui->comboBox->currentText(), ui->lineEdit_2->text(), ui->lineEdit_3->text(), date);
    }
}


void updateActor::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == 2)
    {
        ui->dateEdit->setDate(QDate::fromString("1900-01-01", "yyyy-MM-dd"));
        ui->dateEdit->setEnabled(false);
        ui->pushButton_2->setEnabled(true);
    }
    else
    {
        ui->dateEdit->setDate(QDate::currentDate());
        ui->dateEdit->setEnabled(true);
    }
}


void updateActor::on_checkBox_2_stateChanged(int arg1)
{
    if (arg1 == 2)
    {
        ui->pushButton->setEnabled(false);
        ui->label_7->clear();
        portraitFlag = true;
    }
    else
    {
        int w = ui->label_7->width();
        int h = ui->label_7->height();
        ui->label_7->setPixmap(outPortrait.scaled(w,h,Qt::KeepAspectRatio));
        ui->pushButton->setEnabled(true);
        portraitFlag = false;
    }
}


void updateActor::on_pushButton_3_clicked()
{
    paintWin = new Paint();

    connect(this,SIGNAL(sendImageForChange(QPixmap)), paintWin, SLOT(acceptImage(QPixmap)));
    emit sendImageForChange(outPortrait);

    connect(paintWin,SIGNAL(sendNewImage(QPixmap)),
            this, SLOT(getNewPortrait(QPixmap)));

    paintWin->setWindowTitle("Изменить портрет");
    paintWin->show();
}
