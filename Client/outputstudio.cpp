#include "outputstudio.h"
#include "ui_outputstudio.h"

outputStudio::outputStudio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::outputStudio)
{
    ui->setupUi(this);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuReq(QPoint)));
    flagContextMenu = false;
    ui->progressBar->setRange(0,0);
    ui->progressBar->hide();
}

outputStudio::~outputStudio()
{
    delete ui;
}

void outputStudio::acceptStudio(QJsonArray docArr)
{
    studio->clear();
    QStringList listHeaderStudio;
    listHeaderStudio.append("Название киностудии");
    studio->setHorizontalHeaderLabels(listHeaderStudio);

    QList<QStandardItem*> listStudioName;

    for (int i = 0; i < docArr.count(); ++i)
    {
        QStandardItem* studioName = new QStandardItem(docArr.at(i).toObject().value("studioName").toString());
        listStudioName.append(studioName);
        studio->appendRow(listStudioName);
        listStudioName.clear();
    }
    ui->tableView->setModel(studio);

    ui->progressBar->hide();
    flagContextMenu = true;
}

void outputStudio::customMenuReq(QPoint pos)
{
    if (flagContextMenu)
    {
        QModelIndex index = ui->tableView->indexAt(pos);
        globalID = ui->tableView->model()->data(ui->tableView->model()->index(index.row(),0)).toString();

        QMenu* menu = new QMenu(this);
        QAction* deleteStudio = new QAction("Удалить киностудию", this);
        connect(deleteStudio, SIGNAL(triggered()), this, SLOT(deleteStudio()));

        menu->addAction(deleteStudio);
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void outputStudio::deleteStudio()
{
    flagContextMenu = false;
    ui->progressBar->show();
    emit sendDeleteStudioSignal(globalID);
}
