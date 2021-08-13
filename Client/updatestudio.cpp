#include "updatestudio.h"
#include "ui_updatestudio.h"

updateStudio::updateStudio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::updateStudio)
{
    ui->setupUi(this);

    ui->pushButton->setEnabled(false);
    ui->progressBar->setRange(0,0);
    ui->progressBar->hide();
}

updateStudio::~updateStudio()
{
    delete ui;
}

void updateStudio::acceptStudioName(QList<QString> listStudioName)
{
    ui->lineEdit->clear();

    localListStudioName = listStudioName;
    ui->comboBox->addItem(" ");
    foreach( QString value, listStudioName )
    {
        ui->comboBox->addItem(value);
    }
    ui->comboBox->setCurrentIndex(0);
}

void updateStudio::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if (ui->comboBox->currentText() != " ")
    {
        ui->lineEdit->setText(ui->comboBox->currentText());
        ui->pushButton->setEnabled(true);
    }
}


void updateStudio::on_pushButton_clicked()
{
    bool uniqueStudioName = true;
    bool StudioNameFlag = false;
    QString studioName = "";

    if (ui->lineEdit->text() == "")
    {
        studioName = ui->comboBox->currentText();
        StudioNameFlag = true;
    }
    else
    {
        for (int i = 0; i < localListStudioName.length(); ++i)
        {
            if (localListStudioName[i] == ui->lineEdit->text())
            {
                uniqueStudioName = false;
            }
        }

        if (uniqueStudioName)
        {
            studioName = ui->lineEdit->text();
            StudioNameFlag = true;
        }
        else
        {
            QMessageBox::information(this,"Ошибка","Необходимо использовать уникальное название");
        }
    }

    if (StudioNameFlag)
    {
        ui->progressBar->show();
        ui->pushButton->setEnabled(false);

        sendUpdateStudio(ui->comboBox->currentText(), studioName);
    }
}

