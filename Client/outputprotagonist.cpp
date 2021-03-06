#include "outputprotagonist.h"
#include "ui_outputprotagonist.h"

outputProtagonist::outputProtagonist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::outputProtagonist)
{
    ui->setupUi(this);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuReq(QPoint)));
    flagContextMenu = false;
    ui->progressBar->setRange(0,0);
    ui->progressBar->hide();
}

outputProtagonist::~outputProtagonist()
{
    delete ui;
}

void outputProtagonist::acceptProtagonist(QJsonArray docArr, QJsonArray docArr2)
{
    protagonist->clear();
    QStringList listHeaderProtagonist;
    listHeaderProtagonist.append("Номер главного героя");
    listHeaderProtagonist.append("Главный герой");
    listHeaderProtagonist.append("Номер актёра");
    protagonist->setHorizontalHeaderLabels(listHeaderProtagonist);

    QList<QStandardItem*> listProtagonist;
    QStandardItem* protagonistName;
    QStandardItem* actorID;

    for (int i = 0; i < docArr.count(); ++i)
    {
        QStandardItem* protagonistID = new QStandardItem(docArr.at(i).toObject().value("protagonistID").toString());
        if (docArr.at(i).toObject().value("name").toString() == "")
        {
            protagonistName = new QStandardItem("NULL");
        }
        else
        {
            protagonistName = new QStandardItem(docArr.at(i).toObject().value("name").toString());
        }
        if (docArr.at(i).toObject().value("actorID").toString() == "0")
        {
            actorID = new QStandardItem("NULL");
        }
        else
        {
            actorID = new QStandardItem(docArr.at(i).toObject().value("actorID").toString());
        }
        listProtagonist.append(protagonistID);
        listProtagonist.append(protagonistName);
        listProtagonist.append(actorID);

        protagonist->appendRow(listProtagonist);
        listProtagonist.clear();
    }
    ui->tableView->setModel(protagonist);

    protagonistAndActor->clear();
    QStringList listHeaderProtagonistAndActor;
    listHeaderProtagonistAndActor.append("Главный герой");
    listHeaderProtagonistAndActor.append("Имя актёра");
    listHeaderProtagonistAndActor.append("Фамилия актёра");
    protagonistAndActor->setHorizontalHeaderLabels(listHeaderProtagonistAndActor);

    QList<QStandardItem*> listProtagonistAndActor;
    QStandardItem* name;
    QStandardItem* firstName;
    QStandardItem* lastName;

    for (int i = 0; i < docArr2.count(); ++i)
    {
        if (docArr2.at(i).toObject().value("nameProtagonist").toString() == "")
        {
            name = new QStandardItem("NULL");
        }
        else
        {
            name = new QStandardItem(docArr2.at(i).toObject().value("nameProtagonist").toString());
        }
        if (docArr2.at(i).toObject().value("firstName").toString() == "")
        {
            firstName = new QStandardItem("NULL");
        }
        else
        {
            firstName = new QStandardItem(docArr2.at(i).toObject().value("firstName").toString());
        }
        if (docArr2.at(i).toObject().value("lastName").toString() == "")
        {
            lastName = new QStandardItem("NULL");
        }
        else
        {
            lastName = new QStandardItem(docArr2.at(i).toObject().value("lastName").toString());
        }
        listProtagonistAndActor.append(name);
        listProtagonistAndActor.append(firstName);
        listProtagonistAndActor.append(lastName);

        protagonistAndActor->appendRow(listProtagonistAndActor);
        listProtagonistAndActor.clear();
    }
    ui->tableView_2->setModel(protagonistAndActor);

    ui->progressBar->hide();
    flagContextMenu = true;
}

void outputProtagonist::customMenuReq(QPoint pos)
{
    if (flagContextMenu)
    {
        QModelIndex index = ui->tableView->indexAt(pos);
        globalID = ui->tableView->model()->data(ui->tableView->model()->index(index.row(),0)).toString();

        QMenu* menu = new QMenu(this);
        QAction* deleteProtagonist = new QAction("Удалить главного героя", this);
        connect(deleteProtagonist, SIGNAL(triggered()), this, SLOT(deleteProtagonist()));

        menu->addAction(deleteProtagonist);
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void outputProtagonist::deleteProtagonist()
{
    flagContextMenu = false;
    ui->progressBar->show();
    emit sendDeleteProtagonistSignal(globalID);
}
