#include "outputdirector.h"
#include "ui_outputdirector.h"

outputDirector::outputDirector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::outputDirector)
{
    ui->setupUi(this);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

outputDirector::~outputDirector()
{
    delete ui;
}

void outputDirector::acceptDirector(QJsonArray docArr)
{
    director->clear();
    QStringList listHeaderDirector;
    listHeaderDirector.append("Номер режисёра");
    listHeaderDirector.append("Имя");
    listHeaderDirector.append("Фамилия");
    listHeaderDirector.append("Год рождения");
    director->setHorizontalHeaderLabels(listHeaderDirector);

    QList<QStandardItem*> listDirector;
    QStandardItem* firstName;
    QStandardItem* lastName;
    QStandardItem* dateOfBirth;

    for (int i = 0; i < docArr.count(); ++i)
    {
        QStandardItem* directorID = new QStandardItem(docArr.at(i).toObject().value("directorID").toString());
        if (docArr.at(i).toObject().value("firstName").toString() == "")
        {
            firstName = new QStandardItem("NULL");
        }
        else
        {
            firstName = new QStandardItem(docArr.at(i).toObject().value("firstName").toString());
        }
        if (docArr.at(i).toObject().value("lastName").toString() == "")
        {
            lastName = new QStandardItem("NULL");
        }
        else
        {
            lastName = new QStandardItem(docArr.at(i).toObject().value("lastName").toString());
        }
        if (docArr.at(i).toObject().value("dateOfBirth").toString() == "")
        {
            dateOfBirth = new QStandardItem("NULL");
        }
        else
        {
            dateOfBirth = new QStandardItem(docArr.at(i).toObject().value("dateOfBirth").toString());
        }
        listDirector.append(directorID);
        listDirector.append(firstName);
        listDirector.append(lastName);
        listDirector.append(dateOfBirth);

        director->appendRow(listDirector);
        listDirector.clear();

        ui->tableView->setModel(director);
    }
}
