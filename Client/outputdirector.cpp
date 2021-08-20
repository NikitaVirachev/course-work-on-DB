#include "outputdirector.h"
#include "ui_outputdirector.h"

outputDirector::outputDirector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::outputDirector)
{
    ui->setupUi(this);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuReq(QPoint)));
    flagContextMenu = false;
    ui->progressBar->setRange(0,0);
    ui->progressBar->hide();
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
    }
    ui->tableView->setModel(director);

    ui->progressBar->hide();
    flagContextMenu = true;
}

void outputDirector::customMenuReq(QPoint pos)
{
    if (flagContextMenu)
    {
        QModelIndex index = ui->tableView->indexAt(pos);
        globalID = ui->tableView->model()->data(ui->tableView->model()->index(index.row(),0)).toString();

        QMenu* menu = new QMenu(this);
        QAction* deleteDirector = new QAction("Удалить режиссёра", this);
        connect(deleteDirector, SIGNAL(triggered()), this, SLOT(deleteDirector()));

        menu->addAction(deleteDirector);
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void outputDirector::deleteDirector()
{
    flagContextMenu = false;
    ui->progressBar->show();
    emit sendDeleteDirectorSignal(globalID);
}
