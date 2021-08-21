#include "storedprocedures.h"
#include "ui_storedprocedures.h"

storedProcedures::storedProcedures(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::storedProcedures)
{
    ui->setupUi(this);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

storedProcedures::~storedProcedures()
{
    delete ui;
}

void storedProcedures::accept(QJsonArray docArr1, QJsonArray docArr2)
{
    storedProcedure1->clear();
    QStringList listHeaderStoredProcedure1;
    listHeaderStoredProcedure1.append("Год выхода фильма");
    storedProcedure1->setHorizontalHeaderLabels(listHeaderStoredProcedure1);

    QList<QStandardItem*> listStoredProcedure1;

    for (int i = 0; i < docArr1.count(); ++i)
    {
        QStandardItem* releaseDate = new QStandardItem(docArr1.at(i).toObject().value("releaseDate").toString());
        listStoredProcedure1.append(releaseDate);

        storedProcedure1->appendRow(listStoredProcedure1);
        listStoredProcedure1.clear();
    }

    storedProcedure2->clear();
    QStringList listHeaderStoredProcedure2;
    listHeaderStoredProcedure2.append("Средний бюджет");
    listHeaderStoredProcedure2.append("Средние сборы");
    storedProcedure2->setHorizontalHeaderLabels(listHeaderStoredProcedure2);

    QList<QStandardItem*> listStoredProcedure2;
    QStandardItem* boxOffice;
    QStandardItem* budget;

    for (int i = 0; i < docArr2.count(); ++i)
    {
        if (docArr2.at(i).toObject().value("boxOffice").toString() != "0")
        {
            boxOffice = new QStandardItem(docArr2.at(i).toObject().value("averageBoxOffice").toString());
        }
        else
        {
            boxOffice = new QStandardItem("NULL");
        }
        if (docArr2.at(i).toObject().value("budget").toString() != "0")
        {
            budget = new QStandardItem(docArr2.at(i).toObject().value("averageBudget").toString());
        }
        else
        {
            budget = new QStandardItem("NULL");
        }
        listStoredProcedure2.append(boxOffice);
        listStoredProcedure2.append(budget);

        storedProcedure2->appendRow(listStoredProcedure2);
        listStoredProcedure2.clear();
    }

    ui->tableView->setModel(storedProcedure1);
    ui->tableView_2->setModel(storedProcedure2);
}
