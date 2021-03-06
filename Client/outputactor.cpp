#include "outputactor.h"
#include "ui_outputactor.h"

outputActor::outputActor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::outputActor)
{
    ui->setupUi(this);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuReq(QPoint)));
    flagContextMenu = false;
    ui->label_2->clear();
    ui->progressBar->setRange(0,0);
    ui->progressBar->hide();
}

outputActor::~outputActor()
{
    delete ui;
}

void outputActor::acceptActor(QJsonArray docArr)
{
    actor->clear();
    QStringList listHeaderActor;
    listHeaderActor.append("Номер актёра");
    listHeaderActor.append("Имя");
    listHeaderActor.append("Фамилия");
    listHeaderActor.append("Дата рождения");
    actor->setHorizontalHeaderLabels(listHeaderActor);

    QList<QStandardItem*> listActor;
    QStandardItem* actorFirstName;
    QStandardItem* actorLastName;
    QStandardItem* actorDateOfBirth;

    for (int i = 0; i < docArr.count(); ++i)
    {
        QStandardItem* actorID = new QStandardItem(docArr.at(i).toObject().value("actorID").toString());
        if (docArr.at(i).toObject().value("firstName").toString() == "")
        {
            actorFirstName = new QStandardItem("NULL");
        }
        else
        {
            actorFirstName = new QStandardItem(docArr.at(i).toObject().value("firstName").toString());
        }
        if (docArr.at(i).toObject().value("lastName").toString() == "")
        {
            actorLastName = new QStandardItem("NULL");
        }
        else
        {
            actorLastName = new QStandardItem(docArr.at(i).toObject().value("lastName").toString());
        }
        if (docArr.at(i).toObject().value("dateOfBirth").toString() == "")
        {
            actorDateOfBirth = new QStandardItem("NULL");
        }
        else
        {
            actorDateOfBirth = new QStandardItem(docArr.at(i).toObject().value("dateOfBirth").toString());
        }
        listActor.append(actorID);
        listActor.append(actorFirstName);
        listActor.append(actorLastName);
        listActor.append(actorDateOfBirth);

        actor->appendRow(listActor);
        listActor.clear();
    }
    ui->tableView->setModel(actor);

    ui->progressBar->hide();
    flagContextMenu = true;
}

void outputActor::acceptActorPortrait(QByteArray actorPortrait, QJsonArray docArr)
{
    movies->clear();
    QStringList listHeaderMovies;
    listHeaderMovies.append("Номер фильма");
    listHeaderMovies.append("Название фильма");
    listHeaderMovies.append("Дата выхода");
    listHeaderMovies.append("Сборы");
    listHeaderMovies.append("Бюджет");
    movies->setHorizontalHeaderLabels(listHeaderMovies);

    QList<QStandardItem*> listMovie;

    QStandardItem* title;
    QStandardItem* releaseDate;
    QStandardItem* boxOffice;
    QStandardItem* budget;

    for (int i = 0; i < docArr.count(); ++i)
    {
        QStandardItem* movieID = new QStandardItem(docArr.at(i).toObject().value("movieID").toString());
        if (docArr.at(i).toObject().value("title").toString() != "")
        {
            title = new QStandardItem(docArr.at(i).toObject().value("title").toString());
        }
        else
        {
            title = new QStandardItem("NULL");
        }
        if (docArr.at(i).toObject().value("releaseDate").toString() != "")
        {
            releaseDate = new QStandardItem(docArr.at(i).toObject().value("releaseDate").toString());
        }
        else
        {
            releaseDate = new QStandardItem("NULL");
        }
        if (docArr.at(i).toObject().value("boxOffice").toString() != "0")
        {
            boxOffice = new QStandardItem(docArr.at(i).toObject().value("boxOffice").toString());
        }
        else
        {
            boxOffice = new QStandardItem("NULL");
        }
        if (docArr.at(i).toObject().value("budget").toString() != "0")
        {
            budget = new QStandardItem(docArr.at(i).toObject().value("budget").toString());
            qDebug() << docArr.at(i).toObject().value("budget").toString();
        }
        else
        {
            budget = new QStandardItem("NULL");
        }
        listMovie.append(movieID);
        listMovie.append(title);
        listMovie.append(releaseDate);
        listMovie.append(boxOffice);
        listMovie.append(budget);

        movies->appendRow(listMovie);
        listMovie.clear();
    }
    ui->tableView_2->setModel(movies);

    if (actorPortrait != "NULL")
    {
        QPixmap outPortrait = QPixmap();
        outPortrait.loadFromData(actorPortrait,"PNG");
        int w = ui->label_2->width();
        int h = ui->label_2->height();
        ui->label_2->setPixmap(outPortrait.scaled(w,h,Qt::KeepAspectRatio));
    }
    ui->progressBar->hide();
    flagContextMenu = true;
}

void outputActor::customMenuReq(QPoint pos)
{
    if (flagContextMenu)
    {
        QModelIndex index = ui->tableView->indexAt(pos);
        globalID = ui->tableView->model()->data(ui->tableView->model()->index(index.row(),0)).toString();

        QMenu* menu = new QMenu(this);
        QAction* deleteActor = new QAction("Удалить актёра", this);
        connect(deleteActor, SIGNAL(triggered()), this, SLOT(deleteActor()));

        menu->addAction(deleteActor);
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void outputActor::deleteActor()
{
    flagContextMenu = false;
    ui->progressBar->show();
    emit sendDeleteActorSignal(globalID);
}

void outputActor::on_tableView_clicked(const QModelIndex &index)
{
    ui->label_2->clear();
    ui->progressBar->show();
    flagContextMenu = false;
    int row = index.row();
    QString actorID = index.sibling(row, 0).data().toString();
    emit requestPhotoActor(actorID);
}

