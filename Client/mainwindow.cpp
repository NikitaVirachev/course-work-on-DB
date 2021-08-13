#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_3->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_4->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_4->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_5->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_5->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->label->clear();
    ui->label_2->clear();
    ui->progressBar->setRange(0,0);
    ui->progressBar->hide();
    ui->menu_2->setEnabled(false);
    ui->menu_3->setEnabled(false);
    ui->action_3->setEnabled(false);

    socket = new QTcpSocket(this);
        connect(socket,SIGNAL(readyRead()), this, SLOT(socketReady()));
        connect(socket,SIGNAL(disconnected()), this, SLOT(socketDisc()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::conToHost(QString ipAdress)
{
    socket->connectToHost(ipAdress, 5555);
}

void MainWindow::conToDB(QString adressDB, QString nameDB, QString login, QString password)
{
    if (socket->isOpen())
    {
        socket->write("{\"type\":\"connection\",\"params\":\"connectionInformation\""
                      ",\"adressDB\":\"" + adressDB.toUtf8() +
                      "\",\"nameDB\":\"" + nameDB.toUtf8() +
                      "\",\"login\":\"" + login.toUtf8() +
                      "\",\"password\":\"" + password.toUtf8() +
                      "\"}");
        socket->waitForBytesWritten(500);
    }
    else
    {
        QMessageBox::information(this,"Инофрмация","Соединение не установлено");
    }
}

void MainWindow::preparingAddMovie(QString title, QString releaseDate, QString boxOffice, QString budget, QString protagonistID, QString directorID, QString studioName, QPixmap poster, QString scenario)
{
    newScenario = scenario.toUtf8();

    QBuffer buffer(&newPoster);
    buffer.open(QIODevice::WriteOnly);
    poster.save(&buffer, "PNG");

    if (socket->isOpen())
    {
        socket->write("{\"type\":\"addNewMovie\",\"params\":\"basicInformation\""
                      ",\"title\":\"" + title.toUtf8() +
                      "\",\"releaseDate\":\"" + releaseDate.toUtf8() +
                      "\",\"boxOffice\":\"" + boxOffice.toUtf8() +
                      "\",\"budget\":\"" + budget.toUtf8() +
                      "\",\"protagonistID\":\"" + protagonistID.toUtf8() +
                      "\",\"directorID\":\"" + directorID.toUtf8() +
                      "\",\"studioName\":\"" + studioName.toUtf8() +
                      "\"}");
        socket->waitForBytesWritten(500);
    }
    else
    {
        QMessageBox::information(this,"Инофрмация","Соединение не установлено");
    }
}

void MainWindow::outputData()
{
    movies->clear();
    director->clear();
    studio->clear();
    protagonist->clear();
    actor->clear();
    ui->label->clear();
    ui->label_2->clear();
    ui->textEdit->clear();

    QStringList listHeaderMovies;
    listHeaderMovies.append("Номер фильма");
    listHeaderMovies.append("Название фильма");
    listHeaderMovies.append("Дата выхода");
    listHeaderMovies.append("Сборы");
    listHeaderMovies.append("Бюджет");
    movies->setHorizontalHeaderLabels(listHeaderMovies);

    QList<QStandardItem*> listMovie;

    QJsonArray docArr = doc.object().value("result").toArray();

    for (int i = 0; i < docArr.count(); ++i)
    {
        QStandardItem* movieID = new QStandardItem(docArr.at(i).toObject().value("movieID").toString());
        //listMovieID.append(docArr.at(i).toObject().value("movieID").toString());
        QStandardItem* title = new QStandardItem(docArr.at(i).toObject().value("title").toString());
        QStandardItem* releaseDate = new QStandardItem(docArr.at(i).toObject().value("releaseDate").toString());
        QStandardItem* boxOffice = new QStandardItem(docArr.at(i).toObject().value("boxOffice").toString());
        QStandardItem* budget = new QStandardItem(docArr.at(i).toObject().value("budget").toString());
        listMovie.append(movieID);
        listMovie.append(title);
        listMovie.append(releaseDate);
        listMovie.append(boxOffice);
        listMovie.append(budget);

        movies->appendRow(listMovie);
        listMovie.clear();
    }


    ui->tableView->setModel(movies);
}

void MainWindow::preparingAddDirector(QString firstName, QString lastName, QString dateOfBirth)
{
    if (socket->isOpen())
    {
        socket->write("{\"type\":\"addNewDirector\",\"params\":\"data\""
                      ",\"firstName\":\"" + firstName.toUtf8() +
                      "\",\"lastName\":\"" + lastName.toUtf8() +
                      "\",\"dateOfBirth\":\"" + dateOfBirth.toUtf8() +
                      "\"}");
        socket->waitForBytesWritten(500);
    }
    else
    {
        QMessageBox::information(this,"Инофрмация","Соединение не установлено");
    }
}

void MainWindow::preparingAddStudio(QString studioName)
{
    if (socket->isOpen())
    {
        socket->write("{\"type\":\"addNewStudio\",\"params\":\"data\""
                      ",\"studioName\":\"" + studioName.toUtf8() +
                      "\"}");
        socket->waitForBytesWritten(500);
    }
    else
    {
        QMessageBox::information(this,"Инофрмация","Соединение не установлено");
    }
}

void MainWindow::preparingAddActor(QString firstName, QString lastName, QString dateOfBirth, QPixmap actorPortrait)
{
    QBuffer buffer(&newActorPortrait);
    buffer.open(QIODevice::WriteOnly);
    actorPortrait.save(&buffer, "PNG");

    if (socket->isOpen())
    {
        socket->write("{\"type\":\"addNewActor\",\"params\":\"basicInformation\""
                      ",\"firstName\":\"" + firstName.toUtf8() +
                      "\",\"lastName\":\"" + lastName.toUtf8() +
                      "\",\"dateOfBirth\":\"" + dateOfBirth.toUtf8() +
                      "\"}");
        socket->waitForBytesWritten(500);
    }
    else
    {
        QMessageBox::information(this,"Инофрмация","Соединение не установлено");
    }
}

void MainWindow::preparingAddProtagonist(QString name, QString actorID)
{
    if (socket->isOpen())
    {
        socket->write("{\"type\":\"addNewProtagonist\",\"params\":\"data\""
                      ",\"name\":\"" + name.toUtf8() +
                      "\",\"actorID\":\"" + actorID.toUtf8() +
                      "\"}");
        socket->waitForBytesWritten(500);
    }
    else
    {
        QMessageBox::information(this,"Инофрмация","Соединение не установлено");
    }
}

void MainWindow::prepareMovieInformationForUpdate(QString movieID)
{
    if (socket->isOpen())
    {
        socket->write("{\"type\":\"updateMovie\",\"params\":\"findMovieInformation\",\"id\":\"" + movieID.toUtf8() + "\"}");
        socket->waitForBytesWritten(500);
    }
    else
    {
        QMessageBox::information(this,"Инофрмация","Соединение не установлено");
    }
}

void MainWindow::preparingUpdMovieWithPoster(QString oldMovieID, QString newMovieID, QString title, QString releaseDate, QString boxOffice, QString budget, QString directorID,
                                             QString protagonistID, QString studioName, QPixmap poster)
{
    QBuffer buffer(&newPoster);
    buffer.open(QIODevice::WriteOnly);
    poster.save(&buffer, "PNG");

    if (socket->isOpen())
    {
        socket->write("{\"type\":\"updateMovie\",\"params\":\"sendMovieInformation\",\"additionally\":\"poster\""
                      ",\"oldMovieID\":\"" + oldMovieID.toUtf8() +
                      "\",\"newMovieID\":\"" + newMovieID.toUtf8() +
                      "\",\"title\":\"" + title.toUtf8() +
                      "\",\"releaseDate\":\"" + releaseDate.toUtf8() +
                      "\",\"boxOffice\":\"" + boxOffice.toUtf8() +
                      "\",\"budget\":\"" + budget.toUtf8() +
                      "\",\"directorID\":\"" + directorID.toUtf8() +
                      "\",\"protagonistID\":\"" + protagonistID.toUtf8() +
                      "\",\"studioName\":\"" + studioName.toUtf8() +
                      "\"}");
        socket->waitForBytesWritten(500);
    }
    else
    {
        QMessageBox::information(this,"Инофрмация","Соединение не установлено");
    }
}

void MainWindow::preparingUpdMovieWithScenario(QString oldMovieID, QString newMovieID, QString title, QString releaseDate, QString boxOffice, QString budget, QString directorID,
                                               QString protagonistID, QString studioName, QString scenario)
{
    newScenario = scenario.toUtf8();

    if (socket->isOpen())
    {
        socket->write("{\"type\":\"updateMovie\",\"params\":\"sendMovieInformation\",\"additionally\":\"scenario\""
                      ",\"oldMovieID\":\"" + oldMovieID.toUtf8() +
                      "\",\"newMovieID\":\"" + newMovieID.toUtf8() +
                      "\",\"title\":\"" + title.toUtf8() +
                      "\",\"releaseDate\":\"" + releaseDate.toUtf8() +
                      "\",\"boxOffice\":\"" + boxOffice.toUtf8() +
                      "\",\"budget\":\"" + budget.toUtf8() +
                      "\",\"directorID\":\"" + directorID.toUtf8() +
                      "\",\"protagonistID\":\"" + protagonistID.toUtf8() +
                      "\",\"studioName\":\"" + studioName.toUtf8() +
                      "\"}");
        socket->waitForBytesWritten(500);
    }
    else
    {
        QMessageBox::information(this,"Инофрмация","Соединение не установлено");
    }
}

void MainWindow::preparingUpdMovieWithPosterAndScenario(QString oldMovieID, QString newMovieID, QString title, QString releaseDate,
                                                        QString boxOffice, QString budget, QString directorID, QString protagonistID,
                                                        QString studioName, QPixmap poster, QString scenario)
{
    newScenario = scenario.toUtf8();

    QBuffer buffer(&newPoster);
    buffer.open(QIODevice::WriteOnly);
    poster.save(&buffer, "PNG");

    if (socket->isOpen())
    {
        socket->write("{\"type\":\"updateMovie\",\"params\":\"sendMovieInformation\",\"additionally\":\"posterAndScenario\""
                      ",\"oldMovieID\":\"" + oldMovieID.toUtf8() +
                      "\",\"newMovieID\":\"" + newMovieID.toUtf8() +
                      "\",\"title\":\"" + title.toUtf8() +
                      "\",\"releaseDate\":\"" + releaseDate.toUtf8() +
                      "\",\"boxOffice\":\"" + boxOffice.toUtf8() +
                      "\",\"budget\":\"" + budget.toUtf8() +
                      "\",\"directorID\":\"" + directorID.toUtf8() +
                      "\",\"protagonistID\":\"" + protagonistID.toUtf8() +
                      "\",\"studioName\":\"" + studioName.toUtf8() +
                      "\"}");
        socket->waitForBytesWritten(500);
    }
    else
    {
        QMessageBox::information(this,"Инофрмация","Соединение не установлено");
    }
}

void MainWindow::preparingUpdMovieWithout(QString oldMovieID, QString newMovieID, QString title, QString releaseDate, QString boxOffice, QString budget, QString directorID,
                                          QString protagonistID, QString studioName)
{
    if (socket->isOpen())
    {
        socket->write("{\"type\":\"updateMovie\",\"params\":\"sendMovieInformation\",\"additionally\":\"basicOnly\""
                      ",\"oldMovieID\":\"" + oldMovieID.toUtf8() +
                      "\",\"newMovieID\":\"" + newMovieID.toUtf8() +
                      "\",\"title\":\"" + title.toUtf8() +
                      "\",\"releaseDate\":\"" + releaseDate.toUtf8() +
                      "\",\"boxOffice\":\"" + boxOffice.toUtf8() +
                      "\",\"budget\":\"" + budget.toUtf8() +
                      "\",\"directorID\":\"" + directorID.toUtf8() +
                      "\",\"protagonistID\":\"" + protagonistID.toUtf8() +
                      "\",\"studioName\":\"" + studioName.toUtf8() +
                      "\"}");
        socket->waitForBytesWritten(500);
    }
    else
    {
        QMessageBox::information(this,"Инофрмация","Соединение не установлено");
    }
}

void MainWindow::socketDisc()
{
    socket->deleteLater();
}

void MainWindow::socketReady()
{
    if (socket->waitForConnected(500))
    {
        socket->waitForReadyRead(500);

        if (!complexData)
        {
            Data = socket->readAll();
        }
        else
        {
            Data.append(socket->readAll());
            complexData = false;
        }

        doc = QJsonDocument::fromJson(Data, &docError);

        if ((docError.errorString()=="no error occurred") && (!pictureArrives) && (!actorPortraitArrives) && (!scenarioArrives) && (!updPosterArrives) && (!updScenarioArrives))
        {
            if ((doc.object().value("type").toString() == "connection") && (doc.object().value("params").toString() == "itog"))
            {
                outputData();
                logwin->hide();
                ui->menu_2->setEnabled(true);
                ui->menu_3->setEnabled(true);
                ui->action_3->setEnabled(true);
                ui->action->setEnabled(false);
                QMessageBox::information(this, "Информация", "Соединение установлено");
            }
            else if ((doc.object().value("type").toString() == "selectSelectInformationOnFilm") && (doc.object().value("params").toString() == "resultSelectInformationOnFilm"))
            {

                director->clear();
                QStringList listHeaderDirector;
                listHeaderDirector.append("Номер режисёра");
                listHeaderDirector.append("Имя");
                listHeaderDirector.append("Фамилия");
                listHeaderDirector.append("Год рождения");
                director->setHorizontalHeaderLabels(listHeaderDirector);

                QList<QStandardItem*> listDirector;

                QJsonArray docArr = doc.object().value("resultDirector").toArray();

                for (int i = 0; i < docArr.count(); ++i)
                {
                    QStandardItem* directorID = new QStandardItem(docArr.at(i).toObject().value("directorID").toString());
                    QStandardItem* firstName = new QStandardItem(docArr.at(i).toObject().value("firstName").toString());
                    QStandardItem* lastName = new QStandardItem(docArr.at(i).toObject().value("lastName").toString());
                    QStandardItem* dateOfBirth = new QStandardItem(docArr.at(i).toObject().value("dateOfBirth").toString());
                    listDirector.append(directorID);
                    listDirector.append(firstName);
                    listDirector.append(lastName);
                    listDirector.append(dateOfBirth);

                    director->appendRow(listDirector);
                    listDirector.clear();
                }

                studio->clear();
                QStringList listHeaderStudio;
                listHeaderStudio.append("Название студии");
                studio->setHorizontalHeaderLabels(listHeaderStudio);

                QList<QStandardItem*> listStudioName;

                QStandardItem* studioName = new QStandardItem(doc.object().value("resultStudioName").toString());
                listStudioName.append(studioName);
                studio->appendRow(listStudioName);
                listStudioName.clear();

                protagonist->clear();
                QStringList listHeaderProtagonist;
                listHeaderProtagonist.append("Номер главного героя");
                listHeaderProtagonist.append("Главный герой");
                protagonist->setHorizontalHeaderLabels(listHeaderProtagonist);

                QList<QStandardItem*> listProtagonist;

                docArr = doc.object().value("resultProtagonist").toArray();

                for (int i = 0; i < docArr.count(); ++i)
                {
                    QStandardItem* protagonistID = new QStandardItem(docArr.at(i).toObject().value("protagonistID").toString());
                    QStandardItem* protagonistName = new QStandardItem(docArr.at(i).toObject().value("name").toString());
                    listProtagonist.append(protagonistID);
                    listProtagonist.append(protagonistName);

                    protagonist->appendRow(listProtagonist);
                    listProtagonist.clear();
                }

                actor->clear();
                QStringList listHeaderActor;
                listHeaderActor.append("Номер актёра");
                listHeaderActor.append("Имя");
                listHeaderActor.append("Фамилия");
                listHeaderActor.append("Дата рождения");
                actor->setHorizontalHeaderLabels(listHeaderActor);

                QList<QStandardItem*> listActor;

                docArr = doc.object().value("resultActor").toArray();

                for (int i = 0; i < docArr.count(); ++i)
                {
                    QStandardItem* actorID = new QStandardItem(docArr.at(i).toObject().value("actorID").toString());
                    QStandardItem* actorFirstName = new QStandardItem(docArr.at(i).toObject().value("firstName").toString());
                    QStandardItem* actorLastName = new QStandardItem(docArr.at(i).toObject().value("lastName").toString());
                    QStandardItem* actorDateOfBirth = new QStandardItem(docArr.at(i).toObject().value("dateOfBirth").toString());
                    listActor.append(actorID);
                    listActor.append(actorFirstName);
                    listActor.append(actorLastName);
                    listActor.append(actorDateOfBirth);

                    actor->appendRow(listActor);
                    listActor.clear();
                }

                ui->tableView_2->setModel(director);
                ui->tableView_3->setModel(studio);
                ui->tableView_4->setModel(protagonist);
                ui->tableView_5->setModel(actor);

                socket->write("{\"type\":\"afterSelectInformationOnFilm\",\"params\":\"dataReceived\"}");
                socket->waitForBytesWritten(500);

            }
            else if ((doc.object().value("type").toString() == "select") && (doc.object().value("params").toString() == "size") && (doc.object().value("value").toString() == "informationOnFilm"))
            {
                requireSize = doc.object().value("length").toInt();
                socket->write("{\"type\":\"select\",\"value\":\"informationOnFilm\",\"params\":\"requestItog\"}");
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "afterSelectInformationOnFilm") && (doc.object().value("params").toString() == "size"))
            {
                requireSize = doc.object().value("length").toInt();
                pictureArrives = true;
                socket->write("{\"type\":\"selectPoster\"}");
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "selectActorPortrait") && (doc.object().value("params").toString() == "size"))
            {
                requireSize = doc.object().value("length").toInt();
                actorPortraitArrives = true;
                socket->write("{\"type\":\"selectActorPortrait\"}");
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "selectScenario") && (doc.object().value("params").toString() == "size"))
            {
                requireSize = doc.object().value("length").toInt();
                scenarioArrives = true;
                socket->write("{\"type\":\"selectScenario\"}");
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "resultSelectAllID") && (doc.object().value("params").toString() == "size"))
            {
                requireSize = doc.object().value("length").toInt();
                socket->write("{\"type\":\"resultSelectAllID\"}");
                socket->waitForBytesWritten(500);
            }
            else if (doc.object().value("type").toString() == "resultSelectAllID")
            {
                QJsonArray docArr = doc.object().value("result").toArray();
                QList <QString> listMovieID;
                QList <QString> listDirectorID;
                QList <QString> listProtagonistID;
                QList <QString> listStudioName;
                for (int i = 0; i < docArr.count(); ++i)
                {
                    listMovieID.append(docArr.at(i).toObject().value("MovieID").toString());
                    listDirectorID.append(docArr.at(i).toObject().value("DirectorID").toString());
                    listProtagonistID.append(docArr.at(i).toObject().value("ProtagonistID").toString());
                    listStudioName.append(docArr.at(i).toObject().value("StudioName").toString());
                }

                addMovieWin = new addMovie();

                connect(this,SIGNAL(sendTakeData(QList <QString>, QList <QString>, QList <QString>, QList <QString>)), addMovieWin, SLOT(acceptData(QList <QString>, QList <QString>, QList <QString>, QList <QString>)));
                emit sendTakeData(listMovieID, listDirectorID, listProtagonistID, listStudioName);

                addMovieWin->setWindowTitle("Добавить фильм");
                addMovieWin->show();

                connect(addMovieWin,SIGNAL(sendMovieSignal(QString, QString, QString, QString, QString, QString, QString, QPixmap, QString)), this, SLOT(preparingAddMovie(QString, QString, QString, QString, QString, QString, QString, QPixmap, QString)));
            }
            else if ((doc.object().value("type").toString() == "addNewMovie") && (doc.object().value("params").toString() == "findSizePoster"))
            {
                socket->write("{\"type\":\"addNewMovie\",\"params\":\"sizePoster\",\"length\":"+QByteArray::number(newPoster.size())+"}");
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "addNewMovie") && (doc.object().value("params").toString() == "requestNewPoster"))
            {
                socket->write(newPoster);
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "addNewMovie") && (doc.object().value("params").toString() == "findSizeScenario"))
            {
                socket->write("{\"type\":\"addNewMovie\",\"params\":\"sizeScenario\",\"length\":"+QByteArray::number(newScenario.size())+"}");
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "addNewMovie") && (doc.object().value("params").toString() == "requestNewScenario"))
            {
                socket->write(newScenario);
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "addNewMovie") && (doc.object().value("params").toString() == "movieAddedSuccessfully"))
            {
                addMovieWin->hide();
                ui->progressBar->show();
                ui->menubar->setEnabled(false);
                QMessageBox::information(this, "Информация", "Фильм успешно добавлен");
                socket->write("{\"type\":\"updateData\",\"params\":\"findSize\"}");
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "updateData") && (doc.object().value("params").toString() == "size"))
            {
                //qDebug() << "Поступило: " << doc.object().value("length").toInt();
                requireSize = doc.object().value("length").toInt();
                socket->write("{\"type\":\"updateData\",\"params\":\"requestItog\"}");
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "updateData") && (doc.object().value("params").toString() == "itog"))
            {
                outputData();
                ui->progressBar->hide();
                ui->menubar->setEnabled(true);
                QMessageBox::information(this, "Информация", "Данные обновлены");
            }
            else if ((doc.object().value("type").toString() == "addNewDirector") && (doc.object().value("params").toString() == "directorAddedSuccessfully"))
            {
                addDirectorWin->hide();
                QMessageBox::information(this, "Информация", "Режиссёр успешно добавлен");
            }
            else if ((doc.object().value("type").toString() == "resultSelectAllStudioName") && (doc.object().value("params").toString() == "size"))
            {
                requireSize = doc.object().value("length").toInt();
                socket->write("{\"type\":\"resultSelectAllStudioName\",\"params\":\"requestItog\"}");
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "resultSelectAllStudioName") && (doc.object().value("params").toString() == "itog"))
            {
                QJsonArray docArr = doc.object().value("result").toArray();
                QList <QString> listStudioName;
                for (int i = 0; i < docArr.count(); ++i)
                {
                    listStudioName.append(docArr.at(i).toObject().value("StudioName").toString());
                }

                addStudioWin = new addStudio();

                connect(this,SIGNAL(sendStudioNames(QList <QString>)), addStudioWin, SLOT(acceptData(QList <QString>)));
                emit sendStudioNames(listStudioName);

                addStudioWin->setWindowTitle("Добавить киностудию");
                addStudioWin->show();

                connect(addStudioWin,SIGNAL(sendAddStudioSignal(QString)), this, SLOT(preparingAddStudio(QString)));
            }
            else if ((doc.object().value("type").toString() == "addNewStudio") && (doc.object().value("params").toString() == "studioAddedSuccessfully"))
            {
                addStudioWin->hide();
                QMessageBox::information(this, "Информация", "Киностудия успешно добавлена");
            }
            else if ((doc.object().value("type").toString() == "addNewStudio") && (doc.object().value("params").toString() == "studioAddedFail"))
            {
                QMessageBox::information(this, "Ошибка", "Такая киностудия уже есть");
                connect(this,SIGNAL(sendRestartStudioWin()), addStudioWin, SLOT(restartStudioWin()));
                emit sendRestartStudioWin();
            }
            else if ((doc.object().value("type").toString() == "addNewActor") && (doc.object().value("params").toString() == "findSizeActorPortrait"))
            {
                socket->write("{\"type\":\"addNewActor\",\"params\":\"sizeActorPortrait\",\"length\":"+QByteArray::number(newActorPortrait.size())+"}");
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "addNewActor") && (doc.object().value("params").toString() == "requestNewActorPortrait"))
            {
                socket->write(newActorPortrait);
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "addNewActor") && (doc.object().value("params").toString() == "actorAddedSuccessfully"))
            {
                addActorWin->hide();
                QMessageBox::information(this, "Информация", "Актёр успешно добавлен");
            }
            else if ((doc.object().value("type").toString() == "selectAllActorID") && (doc.object().value("params").toString() == "size"))
            {
                requireSize = doc.object().value("length").toInt();
                socket->write("{\"type\":\"selectAllActorID\",\"params\":\"requestItog\"}");
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "selectAllActorID") && (doc.object().value("params").toString() == "itog"))
            {
                QJsonArray docArr = doc.object().value("result").toArray();
                QList <QString> listActorID;
                for (int i = 0; i < docArr.count(); ++i)
                {
                    listActorID.append(docArr.at(i).toObject().value("ActorID").toString());
                }

                addProtagonistWin = new addProtagonist();

                connect(this,SIGNAL(sendAddProtagonist(QList <QString>)), addProtagonistWin, SLOT(acceptData(QList <QString>)));
                emit sendAddProtagonist(listActorID);

                addProtagonistWin->setWindowTitle("Добавить главного героя");
                addProtagonistWin->show();

                connect(addProtagonistWin,SIGNAL(sendProtagonistSignal(QString, QString)), this, SLOT(preparingAddProtagonist(QString, QString)));
            }
            else if ((doc.object().value("type").toString() == "addNewProtagonist") && (doc.object().value("params").toString() == "protagonistAddedSuccessfully"))
            {
                addProtagonistWin->hide();
                QMessageBox::information(this, "Информация", "Главный герой успешно добавлен");
            }
            else if ((doc.object().value("type").toString() == "selectAllMovieID") && (doc.object().value("params").toString() == "itog"))
            {
                QJsonArray docArr = doc.object().value("result").toArray();
                QList <QString> listMovieID;
                for (int i = 0; i < docArr.count(); ++i)
                {
                    listMovieID.append(docArr.at(i).toObject().value("MovieID").toString());
                }

                updateMovieWin = new updateMovie();

                connect(this,SIGNAL(sendUpdateMovie(QList <QString>)), updateMovieWin, SLOT(acceptMovieID(QList <QString>)));
                emit sendUpdateMovie(listMovieID);

                updateMovieWin->setWindowTitle("Обновить фильм");
                updateMovieWin->show();

                connect(updateMovieWin,SIGNAL(sendMovieIDSignalUpd(QString)), this, SLOT(prepareMovieInformationForUpdate(QString)));
            }
            else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "size"))
            {
                requireSize = doc.object().value("length").toInt();
                socket->write("{\"type\":\"updateMovie\",\"params\":\"requestItog\"}");
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "itog"))
            {
                QJsonArray docArr = doc.object().value("result").toArray();
                QList <QString> listMovieID;
                for (int i = 0; i < docArr.count(); ++i)
                {
                    listMovieID.append(docArr.at(i).toObject().value("MovieID").toString());
                }

                updateMovieWin = new updateMovie();

                connect(this,SIGNAL(sendUpdateMovie(QList <QString>)), updateMovieWin, SLOT(acceptMovieID(QList <QString>)));
                emit sendUpdateMovie(listMovieID);

                updateMovieWin->setWindowTitle("Изменить фильм");
                updateMovieWin->show();

                connect(updateMovieWin,SIGNAL(sendMovieIDSignalUpd(QString)), this, SLOT(prepareMovieInformationForUpdate(QString)));
            }
            else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "movieInformation"))
            {
                updTitle = doc.object().value("title").toString();
                updReleaseDate = doc.object().value("releaseDate").toString();
                updBoxOffice = doc.object().value("boxOffice").toString();
                updBudget = doc.object().value("budget").toString();
                updMovieStudioName = doc.object().value("studioName").toString();
                updMovieDirectorID = doc.object().value("directorID").toString();
                updMovieProtagonistID = doc.object().value("protagonistID").toString();

                socket->write("{\"type\":\"updateMovie\",\"params\":\"requestPosterSize\"}");
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "sizePoster"))
            {
                requireSize = doc.object().value("length").toInt();
                updPosterArrives = true;
                socket->write("{\"type\":\"updateMovie\",\"params\":\"selectPoster\"}");
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "sizeScenario"))
            {
                requireSize = doc.object().value("length").toInt();
                updScenarioArrives = true;
                socket->write("{\"type\":\"updateMovie\",\"params\":\"selectScenario\"}");
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "resultAllID"))
            {
                QJsonArray directorArr = doc.object().value("Director").toArray();
                QList <QString> listDirectorID;
                for (int i = 0; i < directorArr.count(); ++i)
                {
                    listDirectorID.append(directorArr.at(i).toObject().value("ID").toString());
                }

                QJsonArray protagonistArr = doc.object().value("Protagonist").toArray();
                QList <QString> listProtagonistID;
                for (int i = 0; i < protagonistArr.count(); ++i)
                {
                    listProtagonistID.append(protagonistArr.at(i).toObject().value("ID").toString());
                }

                QJsonArray studioArr = doc.object().value("Studio").toArray();
                QList <QString> listStudioName;
                for (int i = 0; i < studioArr.count(); ++i)
                {
                    listStudioName.append(studioArr.at(i).toObject().value("StudioName").toString());
                }

                connect(this,SIGNAL(sendInformationMovieUpd(QString, QString, QString, QString, QString, QString, QString, QByteArray, QByteArray, QList <QString>, QList <QString>, QList <QString>)),
                        updateMovieWin, SLOT(acceptInformationMovieUpd(QString, QString, QString, QString, QString, QString, QString, QByteArray, QByteArray, QList <QString>, QList <QString>, QList <QString>)));
                emit sendInformationMovieUpd(updTitle, updReleaseDate, updBoxOffice, updBudget, updMovieStudioName, updMovieDirectorID,
                                             updMovieProtagonistID, updPoster, updScenario, listDirectorID, listProtagonistID, listStudioName);

                connect(updateMovieWin,SIGNAL(sendUpdateMovieWithPoster(QString, QString, QString, QString, QString, QString, QString, QString, QString, QPixmap)),
                        this, SLOT(preparingUpdMovieWithPoster(QString, QString, QString, QString, QString, QString, QString, QString, QString, QPixmap)));
                connect(updateMovieWin,SIGNAL(sendUpdateMovieWithScenario(QString, QString, QString, QString, QString, QString, QString, QString, QString, QString)),
                        this, SLOT(preparingUpdMovieWithScenario(QString, QString, QString, QString, QString, QString, QString, QString, QString, QString)));
                connect(updateMovieWin,SIGNAL(sendUpdateMovieWithPosterAndScenario(QString, QString, QString, QString, QString, QString, QString, QString, QString, QPixmap, QString)),
                        this, SLOT(preparingUpdMovieWithPosterAndScenario(QString, QString, QString, QString, QString, QString, QString, QString, QString, QPixmap, QString)));
                connect(updateMovieWin,SIGNAL(sendUpdateMovieWithout(QString, QString, QString, QString, QString, QString, QString, QString, QString)),
                        this, SLOT(preparingUpdMovieWithout(QString, QString, QString, QString, QString, QString, QString, QString, QString)));
            }
            else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "movieSuccessfullyUpdated"))
            {
                updateMovieWin->hide();
                ui->progressBar->show();
                ui->menubar->setEnabled(false);
                QMessageBox::information(this, "Информация", "Фильм успешно обновлён");
                socket->write("{\"type\":\"updateData\",\"params\":\"findSize\"}");
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "requestSizeNewPoster"))
            {
                if (doc.object().value("additionally").toString() == "scenario")
                {
                    socket->write("{\"type\":\"updateMovie\",\"params\":\"sizeNewPoster\",\"additionally\":\"scenario\",\"length\":"+QByteArray::number(newPoster.size())+"}");
                    socket->waitForBytesWritten(500);
                }
                else
                {
                    socket->write("{\"type\":\"updateMovie\",\"params\":\"sizeNewPoster\",\"length\":"+QByteArray::number(newPoster.size())+"}");
                    socket->waitForBytesWritten(500);
                }
            }
            else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "requestNewPoster"))
            {
                socket->write(newPoster);
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "requestSizeNewScenario"))
            {
                socket->write("{\"type\":\"updateMovie\",\"params\":\"sizeNewScenario\",\"length\":"+QByteArray::number(newScenario.size())+"}");
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "requestNewScenario"))
            {
                socket->write(newScenario);
                socket->waitForBytesWritten(500);
            }
            else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "movieUnsuccessfullyUpdated"))
            {
                updateMovieWin->close();
                ui->progressBar->show();
                ui->menubar->setEnabled(false);
                QMessageBox::information(this, "Ошибка", "Данные устарели. Повторите попытку");
                socket->write("{\"type\":\"updateData\",\"params\":\"findSize\"}");
                socket->waitForBytesWritten(500);
            }
            else
            {
                complexData = true;
                socketReady();
            }
        }
        else if (pictureArrives)
        {
            //qDebug() << "Сколько пришло: " << Data.size() << ", сколько должно быть: " << requireSize;
            if ((pictureArrives) && (requireSize == Data.size()))
            {
                QPixmap outPoster = QPixmap();
                outPoster.loadFromData(Data,"PNG");

                int w = ui->label->width();
                int h = ui->label->height();
                ui->label->setPixmap(outPoster.scaled(w,h,Qt::KeepAspectRatio));

                socket->write("{\"type\":\"selectActorPortrait\",\"params\":\"selectSize\"}");
                socket->waitForBytesWritten(500);

                pictureArrives = false;
            }
            else
            {
                complexData = true;
                socketReady();
            }
        }
        else if (actorPortraitArrives)
        {
            //qDebug() << "Сколько пришло: " << Data.size() << ", сколько должно быть: " << requireSize;
            if ((actorPortraitArrives) && (requireSize == Data.size()))
            {
                QPixmap outActorPortrait = QPixmap();
                outActorPortrait.loadFromData(Data,"PNG");

                int w = ui->label_2->width();
                int h = ui->label_2->height();
                ui->label_2->setPixmap(outActorPortrait.scaled(w,h,Qt::KeepAspectRatio));

                socket->write("{\"type\":\"selectScenario\",\"params\":\"selectSize\"}");
                socket->waitForBytesWritten(500);

                actorPortraitArrives = false;
            }
            else
            {
                complexData = true;
                socketReady();
            }
        }
        else if (scenarioArrives)
        {
            //qDebug() << "Сколько пришло: " << Data.size() << ", сколько должно быть: " << requireSize;
            if ((scenarioArrives) && (requireSize == Data.size()))
            {
                ui->textEdit->setText(Data);
                ui->progressBar->hide();
                ui->menubar->setEnabled(true);
                scenarioArrives = false;
            }
            else
            {
                complexData = true;
                socketReady();
            }
        }
        else if (updPosterArrives)
        {
            //qDebug() << "Сколько пришло: " << Data.size() << ", сколько должно быть: " << requireSize;
            if ((updPosterArrives) && (requireSize == Data.size()))
            {
                updPoster = Data;

                socket->write("{\"type\":\"updateMovie\",\"params\":\"requestScenarioSize\"}");
                socket->waitForBytesWritten(500);

                updPosterArrives = false;
            }
            else
            {
                complexData = true;
                socketReady();
            }
        }
        else if (updScenarioArrives)
        {
            //qDebug() << "Сколько пришло: " << Data.size() << ", сколько должно быть: " << requireSize;
            if (requireSize == Data.size())
            {
                updScenario = Data;

                socket->write("{\"type\":\"updateMovie\",\"params\":\"requestAllID\"}");
                socket->waitForBytesWritten(500);

                updScenarioArrives = false;
            }
            else
            {
                complexData = true;
                socketReady();
            }
        }
        else
        {
            QMessageBox::information(this, "Информация", "Ошибка с форматом передачи данных: " + docError.errorString());
        }
    }
}

void MainWindow::on_action_triggered()
{
    logwin = new login();
    connect(logwin,SIGNAL(sendSignalToHost(QString)), this, SLOT(conToHost(QString)));
    connect(logwin,SIGNAL(sendSignalToDB(QString, QString, QString, QString)), this, SLOT(conToDB(QString, QString, QString, QString)));
    logwin->setWindowTitle("Подключение");
    logwin->show();
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    director->clear();
    studio->clear();
    protagonist->clear();
    actor->clear();
    ui->label->clear();
    ui->label_2->clear();
    ui->textEdit->clear();
    ui->progressBar->show();
    ui->menubar->setEnabled(false);

    if (socket->isOpen())
    {
        int row = index.row();
        QString movieID = index.sibling(row, 0).data().toString();
        socket->write("{\"type\":\"selectSelectInformationOnFilm\",\"params\":\"informationOnFilm\",\"id\":\"" + movieID.toUtf8() + "\"}");
        socket->waitForBytesWritten(500);
    }
    else
    {
        QMessageBox::information(this,"Инофрмация","Соединение не установлено");
    }
}

void MainWindow::on_action_2_triggered()
{
    socket->write("{\"type\":\"selectAllID\"}");
    socket->waitForBytesWritten(500);
}

void MainWindow::on_action_3_triggered()
{
    ui->progressBar->show();
    ui->menubar->setEnabled(false);
    socket->write("{\"type\":\"updateData\",\"params\":\"findSize\"}");
    socket->waitForBytesWritten(500);
}


void MainWindow::on_action_4_triggered()
{
    addDirectorWin = new addDirector();

    addDirectorWin->setWindowTitle("Добавить режиссёра");
    addDirectorWin->show();

    connect(addDirectorWin,SIGNAL(sendAddDirectorSignal(QString, QString, QString)), this, SLOT(preparingAddDirector(QString, QString, QString)));
}


void MainWindow::on_action_5_triggered()
{
    socket->write("{\"type\":\"selectAllStudioName\",\"params\":\"findSize\"}");
    socket->waitForBytesWritten(500);
}


void MainWindow::on_action_6_triggered()
{
    addActorWin = new addActor();

    addActorWin->setWindowTitle("Добавить актёра");
    addActorWin->show();

    connect(addActorWin,SIGNAL(sendActorSignal(QString, QString, QString, QPixmap)), this, SLOT(preparingAddActor(QString, QString, QString, QPixmap)));
}


void MainWindow::on_action_7_triggered()
{
    socket->write("{\"type\":\"selectAllActorID\",\"params\":\"findSize\"}");
    socket->waitForBytesWritten(500);
}


void MainWindow::on_action_8_triggered()
{
    socket->write("{\"type\":\"selectAllMovieID\",\"params\":\"findAllMovieID\"}");
    socket->waitForBytesWritten(500);
}

