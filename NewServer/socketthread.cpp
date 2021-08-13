#include "socketthread.h"

socketThread::socketThread(qintptr descriptor):socketDescriptor(descriptor)
{

}

socketThread::~socketThread()
{
    delete socket;
}

void socketThread::run()
{
    socket = new QTcpSocket();
    socket->setSocketDescriptor(socketDescriptor);

    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(mySocketReady()), Qt::DirectConnection);
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisc()), Qt::DirectConnection);

    qDebug()<<"Клиент " << socketDescriptor << " подключился к серверу приложения";
    exec();
}

bool socketThread::checkMovieID(QString movieID)
{
    QList <QString> listMovieID;

    QSqlQuery* queryAllMovieID = new QSqlQuery(db);
    if (queryAllMovieID->exec("SELECT MovieID FROM Movie"))
    {
        while (queryAllMovieID->next())
        {
            listMovieID.append(queryAllMovieID->value(0).toString());
        }
    }

    foreach( QString value, listMovieID )
    {
        if (value == movieID)
        {
            return true;
        }
    }

    return false;
}

void socketThread::downloadInformationAboutFilms()
{
    QSqlQuery* movies = new QSqlQuery(db);
    if (movies->exec("SELECT MovieID, Title, ReleaseDate, BoxOffice, Budget FROM Movie"))
    {
        while (movies->next())
        {

            itog.append("{\"movieID\":\""+movies->value(0).toString()+
                        "\",\"title\":\""+movies->value(1).toString()+
                        "\",\"releaseDate\":\""+movies->value(2).toString()+
                        "\",\"boxOffice\":\""+movies->value(3).toString()+
                        "\",\"budget\":\""+movies->value(4).toString()+
                        "\"},");
        }
        itog.remove(itog.length()-1,1);
    }
    else
    {
        qDebug()<<"Запрос на фильмы не выполнен";
    }

    delete movies;
}

void socketThread::mySocketReady()
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

    if ((docError.errorString()=="no error occurred") && (!posterArrives) && (!actorPortraitArrives) && (!scenarioArrives))
    {        
        if ((doc.object().value("type").toString() == "connection") && (doc.object().value("params").toString() == "connectionInformation"))
        {
            qDebug()<< "Клиент " << socketDescriptor<<" запрашивает подключение к серверу БД";
            QString connectionName = QString::number(socketDescriptor);
            db = QSqlDatabase::addDatabase("QODBC", connectionName);
            db.setDatabaseName("DRIVER={SQL Server};SERVER="+doc.object().value("adressDB").toString()+";DATABASE="+doc.object().value("nameDB").toString()+";");
            db.setUserName(doc.object().value("login").toString());
            db.setPassword(doc.object().value("password").toString());
            db.open();

            itog = "{\"type\":\"connection\",\"params\":\"itog\",\"result\":[";

            if (db.isOpen())
            {
                qDebug()<<"Соединение с БД установлено";
                downloadInformationAboutFilms();
            }
            else
            {
                socket->write("{\"type\":\"connect\", \"status\":\"no\"}");
                qDebug()<<"Соединение с БД НЕ установлено";
            }

            itog.append("]}");

            socket->write(itog);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "connection") && (doc.object().value("params").toString() == "requestItog"))
        {
            //qDebug() << "Я тут";
            socket->write(itog);
            qDebug() << "Размер ответного сообщения: " << socket->bytesToWrite();
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "selectSelectInformationOnFilm") && (doc.object().value("params").toString() == "informationOnFilm"))
        {
            movieID = doc.object().value("id").toString();

            qDebug()<< "Клиент " << socketDescriptor<<" запрашивает информацию о фильме № " << movieID;

            itog = "{\"type\":\"selectSelectInformationOnFilm\",\"params\":\"resultSelectInformationOnFilm\",\"resultDirector\":[";

            if (db.isOpen())
            {
                QSqlQuery* queryDirector = new QSqlQuery(db);
                if (queryDirector->exec("SELECT DirectorID FROM Movie WHERE MovieID = " + movieID))
                {
                    queryDirector->first();
                    QString directorID = queryDirector->value(0).toString();
                    QSqlQuery* director = new QSqlQuery(db);

                    if (director->exec("SELECT DirectorID, FirstName, LastName, DateOfBirth FROM Director WHERE DirectorID = " + directorID))
                    {
                        while (director->next())
                        {

                            itog.append("{\"directorID\":\""+director->value(0).toString()+
                                        "\",\"firstName\":\""+director->value(1).toString()+
                                        "\",\"lastName\":\""+director->value(2).toString()+
                                        "\",\"dateOfBirth\":\""+director->value(3).toString()+
                                        "\"},");
                        }
                        itog.remove(itog.length()-1,1);
                        itog.append("],\"resultStudioName\":\"");
                    }
                    else
                    {
                        qDebug()<<"Запрос на режисёра не выполнен";
                    }
                }
                else
                {
                    //ошибка
                    qDebug() << "Ошибка в получении id режисёра: " << queryDirector->lastError().text();
                }

                QSqlQuery* queryStudioName = new QSqlQuery(db);
                if (queryStudioName->exec("SELECT StudioName FROM Movie WHERE MovieID = " + movieID))
                {
                    queryStudioName->first();
                    QString studioName = queryStudioName->value(0).toString();
                    itog.append(studioName + "\",\"resultProtagonist\":[");
                }
                else
                {
                    //ошибка
                    qDebug() << "Ошибка в получении названия студии: " << queryStudioName->lastError().text();
                }

                QSqlQuery* queryProtagonist = new QSqlQuery(db);
                if (queryProtagonist->exec("SELECT ProtagonistID FROM Movie WHERE MovieID = " + movieID))
                {
                    queryProtagonist->first();
                    QString protagonistID = queryProtagonist->value(0).toString();
                    QSqlQuery* protagonist = new QSqlQuery(db);
                    if (protagonist->exec("SELECT Name FROM Protagonist WHERE ProtagonistID = " + protagonistID))
                    {
                        while (protagonist->next())
                        {

                            itog.append("{\"protagonistID\":\""+protagonistID+
                                        "\",\"name\":\""+protagonist->value(0).toString()+
                                        "\"},");
                        }
                        itog.remove(itog.length()-1,1);
                        itog.append("],\"resultActor\":[");
                    }
                    else
                    {
                        qDebug()<<"Запрос на главного героя не выполнен";
                    }
                }
                else
                {
                    //ошибка
                    qDebug() << "Ошибка в получении id главного героя: " << queryProtagonist->lastError().text();
                }


                QSqlQuery* queryActor = new QSqlQuery(db);
                if (queryActor->exec("SELECT ActorID FROM Protagonist JOIN Movie ON Protagonist.ProtagonistID = Movie.ProtagonistID WHERE MovieID = " + movieID))
                {
                    queryActor->first();
                    QString actorID = queryActor->value(0).toString();
                    QSqlQuery* actor = new QSqlQuery(db);
                    if (actor->exec("SELECT ActorID, FirstName, LastName, DateOfBirth FROM Actor WHERE ActorID = " + actorID))
                    {
                        while (actor->next())
                        {

                            itog.append("{\"actorID\":\""+actor->value(0).toString()+
                                        "\",\"firstName\":\""+actor->value(1).toString()+
                                        "\",\"lastName\":\""+actor->value(2).toString()+
                                        "\",\"dateOfBirth\":\""+actor->value(3).toString()+
                                        "\"},");
                        }
                        itog.remove(itog.length()-1,1);
                    }
                    else
                    {
                        qDebug()<<"Запрос на актёра не выполнен";
                    }
                }
                else
                {
                    //ошибка
                    qDebug() << "Ошибка в получении id главного героя: " << queryActor->lastError().text();
                }
            }
            itog.append("]}");

            socket->write(itog);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "select") && (doc.object().value("value").toString() == "informationOnFilm") && (doc.object().value("params").toString() == "requestItog"))
        {
            socket->write(itog);
            //qDebug() << "Размер ответного сообщения: " << socket->bytesToWrite();
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "afterSelectInformationOnFilm") && (doc.object().value("params").toString() == "dataReceived"))
        {
            if (db.isOpen())
            {
                QSqlQuery* queryPoster = new QSqlQuery(db);
                if (queryPoster->exec("SELECT Poster FROM Movie WHERE MovieID = " + movieID))
                {
                    queryPoster->first();
                    baPoster = queryPoster->value(0).toByteArray();

                    socket->write("{\"type\":\"afterSelectInformationOnFilm\",\"params\":\"size\",\"length\":"+QByteArray::number(baPoster.size())+"}");
                    socket->waitForBytesWritten(500);
                }
            }

        }
        else if (doc.object().value("type").toString() == "selectPoster")
        {
            socket->write(baPoster);
            //qDebug() << "Размер ответного сообщения: " << socket->bytesToWrite();
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "selectActorPortrait") && (doc.object().value("params").toString() == "selectSize"))
        {
            if (db.isOpen())
            {
                QSqlQuery* queryActorPortrait = new QSqlQuery(db);
                if (queryActorPortrait->exec("SELECT ActorPortrait FROM Actor JOIN Protagonist ON Protagonist.ActorID=Actor.ActorID JOIN Movie ON Movie.ProtagonistID=Protagonist.ProtagonistID WHERE MovieID = " + movieID))
                {
                    queryActorPortrait->first();
                    baActorPortrait = queryActorPortrait->value(0).toByteArray();

                    socket->write("{\"type\":\"selectActorPortrait\",\"params\":\"size\",\"length\":"+QByteArray::number(baActorPortrait.size())+"}");
                    socket->waitForBytesWritten(500);
                }
            }
        }
        else if (doc.object().value("type").toString() == "selectActorPortrait")
        {
            socket->write(baActorPortrait);
            //qDebug() << "Размер ответного сообщения: " << socket->bytesToWrite();
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "selectScenario") && (doc.object().value("params").toString() == "selectSize"))
        {
            if (db.isOpen())
            {
                QSqlQuery* queryScenario = new QSqlQuery(db);
                if (queryScenario->exec("SELECT Scenario FROM Movie WHERE MovieID = " + movieID))
                {
                    queryScenario->first();
                    baScenario = queryScenario->value(0).toByteArray();

                    socket->write("{\"type\":\"selectScenario\",\"params\":\"size\",\"length\":"+QByteArray::number(baScenario.size())+"}");
                    socket->waitForBytesWritten(500);
                }
            }
        }
        else if (doc.object().value("type").toString() == "selectScenario")
        {
            socket->write(baScenario);
            //qDebug() << "Размер ответного сообщения: " << socket->bytesToWrite();
            socket->waitForBytesWritten(500);
        }
        else if (doc.object().value("type").toString() == "selectAllID")
        {
            itog = "{\"type\":\"resultSelectAllID\",\"result\":[";
            if (db.isOpen())
            {
                QSqlQuery* queryAllID = new QSqlQuery(db);
                if (queryAllID->exec("SELECT MovieID, DirectorID, ProtagonistID, StudioName FROM Movie"))
                {
                    while (queryAllID->next())
                    {
                        itog.append("{\"MovieID\":\""+queryAllID->value(0).toString()+
                                    "\",\"DirectorID\":\""+queryAllID->value(1).toString()+
                                    "\",\"ProtagonistID\":\""+queryAllID->value(2).toString()+
                                    "\",\"StudioName\":\""+queryAllID->value(3).toString()+
                                    "\"},");
                    }
                    itog.remove(itog.length()-1,1);
                }
            }
            itog.append("]}");
            socket->write("{\"type\":\"resultSelectAllID\",\"params\":\"size\",\"length\":"+QByteArray::number(itog.size())+"}");
            socket->waitForBytesWritten(500);
        }
        else if (doc.object().value("type").toString() == "resultSelectAllID")
        {
            socket->write(itog);
            //qDebug() << "Размер ответного сообщения: " << socket->bytesToWrite();
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "addNewMovie") && (doc.object().value("params").toString() == "basicInformation"))
        {
            //qDebug()<<"Клиент " << socketDescriptor << " хочет добавить новый фильм";

            newTitle = doc.object().value("title").toString();
            newReleaseDate = doc.object().value("releaseDate").toString();
            newBoxOffice = doc.object().value("boxOffice").toString();
            newBudget = doc.object().value("budget").toString();
            newProtagonistID = doc.object().value("protagonistID").toString();
            newDirectorID = doc.object().value("directorID").toString();
            newStudioName = doc.object().value("studioName").toString();

            socket->write("{\"type\":\"addNewMovie\",\"params\":\"findSizePoster\"}");
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "addNewMovie") && (doc.object().value("params").toString() == "sizePoster"))
        {
            requireSize = doc.object().value("length").toInt();

            posterArrives = true;
            socket->write("{\"type\":\"addNewMovie\",\"params\":\"requestNewPoster\"}");
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "addNewMovie") && (doc.object().value("params").toString() == "sizeScenario"))
        {
            requireSize = doc.object().value("length").toInt();

            scenarioArrives = true;
            socket->write("{\"type\":\"addNewMovie\",\"params\":\"requestNewScenario\"}");
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "updateData") && (doc.object().value("params").toString() == "findSize"))
        {
            itog = "{\"type\":\"updateData\",\"params\":\"itog\",\"result\":[";

            if (db.isOpen())
            {
                qDebug()<< "Клиент " << socketDescriptor<<" запрашивает обновление информации о фильмах";

                downloadInformationAboutFilms();
            }
            else
            {
                socket->write("{\"type\":\"connect\", \"status\":\"no\"}");
                qDebug()<<"Соединение с БД НЕ установлено";
            }

            itog.append("]}");

            socket->write("{\"type\":\"updateData\",\"params\":\"size\",\"length\":"+QByteArray::number(itog.size())+"}");
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "updateData") && (doc.object().value("params").toString() == "requestItog"))
        {
            socket->write(itog);
            //qDebug() << "Размер ответного сообщения: " << socket->bytesToWrite();
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "addNewDirector") && (doc.object().value("params").toString() == "data"))
        {
            QString newDirectorID;
            QSqlQuery* queryNewDirectorID = new QSqlQuery(db);
            if (queryNewDirectorID->exec("SELECT MAX(DirectorID) FROM Director"))
            {
                queryNewDirectorID->first();
                newDirectorID = QString::number(queryNewDirectorID->value(0).toInt() + 1);
            }

            QSqlQuery* query = new QSqlQuery(db);
            query->prepare("INSERT INTO Director (DirectorID, FirstName, LastName, DateOfBirth) "
                           "VALUES (:DirectorID, :FirstName, :LastName, :DateOfBirth)");
            query->bindValue(":DirectorID", newDirectorID);
            query->bindValue(":FirstName", doc.object().value("firstName").toString());
            query->bindValue(":LastName", doc.object().value("lastName").toString());
            query->bindValue(":DateOfBirth", doc.object().value("dateOfBirth").toString());

            if(!query->exec())
            {
                qDebug() << "Запрос на добавление режиссёра составлен неверно!";
            }
            else
            {
                qDebug()<<"Клиент " << socketDescriptor << " добавил нового режиссёра с id = " << newDirectorID;
                socket->write("{\"type\":\"addNewDirector\",\"params\":\"directorAddedSuccessfully\"}");
                socket->waitForBytesWritten(500);
            }
        }
        else if ((doc.object().value("type").toString() == "selectAllStudioName") && (doc.object().value("params").toString() == "findSize"))
        {
            itog = "{\"type\":\"resultSelectAllStudioName\",\"params\":\"itog\",\"result\":[";
            if (db.isOpen())
            {
                QSqlQuery* queryAllStudioName = new QSqlQuery(db);
                if (queryAllStudioName->exec("SELECT StudioName FROM Studio"))
                {
                    while (queryAllStudioName->next())
                    {
                        itog.append("{\"StudioName\":\""+queryAllStudioName->value(0).toString()+
                                    "\"},");
                    }
                    itog.remove(itog.length()-1,1);
                }
            }
            itog.append("]}");
            socket->write("{\"type\":\"resultSelectAllStudioName\",\"params\":\"size\",\"length\":"+QByteArray::number(itog.size())+"}");
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "resultSelectAllStudioName") && (doc.object().value("params").toString() == "requestItog"))
        {
            socket->write(itog);
            //qDebug() << "Размер ответного сообщения: " << socket->bytesToWrite();
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "addNewStudio") && (doc.object().value("params").toString() == "data"))
        {
            if (db.isOpen())
            {
                QList <QString> listStudioName;
                QSqlQuery* queryAllStudioName = new QSqlQuery(db);
                if (queryAllStudioName->exec("SELECT StudioName FROM Studio"))
                {
                    while (queryAllStudioName->next())
                    {
                        listStudioName.append(queryAllStudioName->value(0).toString());
                    }
                }

                bool presenceRepeat = false;
                foreach( QString value, listStudioName )
                {
                    if (doc.object().value("studioName").toString() == value)
                    {
                        presenceRepeat = true;
                    }
                }

                if (presenceRepeat)
                {
                    qDebug()<<"Клиент " << socketDescriptor << " попыталься добавить киностудию " << doc.object().value("studioName").toString() << ", которая уже есть";
                    socket->write("{\"type\":\"addNewStudio\",\"params\":\"studioAddedFail\"}");
                    socket->waitForBytesWritten(500);
                }
                else
                {
                    QSqlQuery* query = new QSqlQuery(db);
                    query->prepare("INSERT INTO Studio (StudioName) "
                                   "VALUES (:StudioName)");
                    query->bindValue(":StudioName", doc.object().value("studioName").toString());

                    if(!query->exec())
                    {
                        qDebug() << "Запрос на добавление киностудии составлен неверно!";
                    }
                    else
                    {
                        qDebug()<<"Клиент " << socketDescriptor << " добавил новую киностудию с названием " << doc.object().value("studioName").toString();
                        socket->write("{\"type\":\"addNewStudio\",\"params\":\"studioAddedSuccessfully\"}");
                        socket->waitForBytesWritten(500);
                    }
                }
            }
        }
        else if ((doc.object().value("type").toString() == "addNewActor") && (doc.object().value("params").toString() == "basicInformation"))
        {
            //qDebug()<<"Клиент " << socketDescriptor << " хочет добавить нового актёра";

            newFirstNameActor = doc.object().value("firstName").toString();
            newLastNameActor = doc.object().value("lastName").toString();
            newDateOfBirthActor = doc.object().value("dateOfBirth").toString();

            socket->write("{\"type\":\"addNewActor\",\"params\":\"findSizeActorPortrait\"}");
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "addNewActor") && (doc.object().value("params").toString() == "sizeActorPortrait"))
        {
            requireSize = doc.object().value("length").toInt();

            actorPortraitArrives = true;
            socket->write("{\"type\":\"addNewActor\",\"params\":\"requestNewActorPortrait\"}");
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "selectAllActorID") && (doc.object().value("params").toString() == "findSize"))
        {
            itog = "{\"type\":\"selectAllActorID\",\"params\":\"itog\",\"result\":[";
            if (db.isOpen())
            {
                QSqlQuery* queryAllActorID = new QSqlQuery(db);
                if (queryAllActorID->exec("SELECT ActorID FROM Actor"))
                {
                    while (queryAllActorID->next())
                    {
                        itog.append("{\"ActorID\":\""+queryAllActorID->value(0).toString()+
                                    "\"},");
                    }
                    itog.remove(itog.length()-1,1);
                }
            }
            itog.append("]}");
            socket->write("{\"type\":\"selectAllActorID\",\"params\":\"size\",\"length\":"+QByteArray::number(itog.size())+"}");
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "selectAllActorID") && (doc.object().value("params").toString() == "requestItog"))
        {
            socket->write(itog);
            //qDebug() << "Размер ответного сообщения: " << socket->bytesToWrite();
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "addNewProtagonist") && (doc.object().value("params").toString() == "data"))
        {
            QString newProtagonistID;
            QSqlQuery* queryNewProtagonistID = new QSqlQuery(db);
            if (queryNewProtagonistID->exec("SELECT MAX(ProtagonistID) FROM Protagonist"))
            {
                queryNewProtagonistID->first();
                newProtagonistID = QString::number(queryNewProtagonistID->value(0).toInt() + 1);
            }

            QSqlQuery* query = new QSqlQuery(db);
            query->prepare("INSERT INTO Protagonist (ProtagonistID, ActorID, Name) "
                           "VALUES (:ProtagonistID, :ActorID, :Name)");
            query->bindValue(":ProtagonistID", newProtagonistID);
            query->bindValue(":ActorID", doc.object().value("actorID").toString());
            query->bindValue(":Name", doc.object().value("name").toString());

            if(!query->exec())
            {
                qDebug() << "Запрос на добавление режиссёра составлен неверно!";
            }
            else
            {
                qDebug()<<"Клиент " << socketDescriptor << " добавил нового главного героя с id = " << newProtagonistID;
                socket->write("{\"type\":\"addNewProtagonist\",\"params\":\"protagonistAddedSuccessfully\"}");
                socket->waitForBytesWritten(500);
            }
        }
        else if ((doc.object().value("type").toString() == "selectAllMovieID") && (doc.object().value("params").toString() == "findAllMovieID"))
        {
            itog = "{\"type\":\"selectAllMovieID\",\"params\":\"itog\",\"result\":[";
            if (db.isOpen())
            {
                QSqlQuery* queryAllMovieID = new QSqlQuery(db);
                if (queryAllMovieID->exec("SELECT MovieID FROM Movie"))
                {
                    while (queryAllMovieID->next())
                    {
                        itog.append("{\"MovieID\":\""+queryAllMovieID->value(0).toString()+
                                    "\"},");
                    }
                    itog.remove(itog.length()-1,1);
                }
            }
            itog.append("]}");
            socket->write(itog);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "findMovieInformation"))
        {
            movieID = doc.object().value("id").toString();

            itog = "{\"type\":\"updateMovie\",\"params\":\"movieInformation\",";

            QSqlQuery* movies = new QSqlQuery(db);
            if (movies->exec("SELECT Title, ReleaseDate, BoxOffice, Budget, StudioName, DirectorID, ProtagonistID FROM Movie WHERE MovieID = " + movieID))
            {
                movies->first();
                itog.append("\"title\":\""+movies->value(0).toString()+
                            "\",\"releaseDate\":\""+movies->value(1).toString()+
                            "\",\"boxOffice\":\""+movies->value(2).toString()+
                            "\",\"budget\":\""+movies->value(3).toString()+
                            "\",\"studioName\":\""+movies->value(4).toString()+
                            "\",\"directorID\":\""+movies->value(5).toString()+
                            "\",\"protagonistID\":\""+movies->value(6).toString()+
                            "\"");
            }
            else
            {
                qDebug()<<"Запрос на фильмы не выполнен";
            }

            itog.append("}");

            socket->write(itog);
            socket->waitForBytesWritten(500);

            delete movies;
        }
        else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "requestPosterSize"))
        {
            if (db.isOpen())
            {
                QSqlQuery* queryPoster = new QSqlQuery(db);
                if (queryPoster->exec("SELECT Poster FROM Movie WHERE MovieID = " + movieID))
                {
                    queryPoster->first();
                    baPoster = queryPoster->value(0).toByteArray();

                    socket->write("{\"type\":\"updateMovie\",\"params\":\"sizePoster\",\"length\":"+QByteArray::number(baPoster.size())+"}");
                    socket->waitForBytesWritten(500);
                }
            }
        }
        else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "selectPoster"))
        {
            socket->write(baPoster);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "requestScenarioSize"))
        {
            if (db.isOpen())
            {
                QSqlQuery* queryScenario = new QSqlQuery(db);
                if (queryScenario->exec("SELECT Scenario FROM Movie WHERE MovieID = " + movieID))
                {
                    queryScenario->first();
                    baScenario = queryScenario->value(0).toByteArray();

                    socket->write("{\"type\":\"updateMovie\",\"params\":\"sizeScenario\",\"length\":"+QByteArray::number(baScenario.size())+"}");
                    socket->waitForBytesWritten(500);
                }
            }
        }
        else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "selectScenario"))
        {
            socket->write(baScenario);
            //qDebug() << "Размер ответного сообщения: " << socket->bytesToWrite();
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "requestAllID"))
        {
            itog = "{\"type\":\"updateMovie\",\"params\":\"resultAllID\",\"Director\":[";
            if (db.isOpen())
            {
                QSqlQuery* queryDirectorID = new QSqlQuery(db);
                if (queryDirectorID->exec("SELECT DirectorID FROM Director"))
                {
                    while (queryDirectorID->next())
                    {
                        itog.append("{\"ID\":\""+queryDirectorID->value(0).toString()+
                                    "\"},");
                    }
                    itog.remove(itog.length()-1,1);
                }

                itog.append("],\"Protagonist\":[");

                QSqlQuery* queryProtagonistID = new QSqlQuery(db);
                if (queryProtagonistID->exec("SELECT ProtagonistID FROM Protagonist"))
                {
                    while (queryProtagonistID->next())
                    {
                        itog.append("{\"ID\":\""+queryProtagonistID->value(0).toString()+
                                    "\"},");
                    }
                    itog.remove(itog.length()-1,1);
                }

                itog.append("],\"Studio\":[");

                QSqlQuery* queryStudioName = new QSqlQuery(db);
                if (queryStudioName->exec("SELECT StudioName FROM Studio"))
                {
                    while (queryStudioName->next())
                    {
                        itog.append("{\"StudioName\":\""+queryStudioName->value(0).toString()+
                                    "\"},");
                    }
                    itog.remove(itog.length()-1,1);
                }
            }
            itog.append("]}");
            socket->write(itog);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "sendMovieInformation"))
        {
            newMovieID = doc.object().value("newMovieID").toString();
            oldMovieID = doc.object().value("oldMovieID").toString();
            newTitle = doc.object().value("title").toString();
            newReleaseDate = doc.object().value("releaseDate").toString();
            newBoxOffice = doc.object().value("boxOffice").toString();
            newBudget = doc.object().value("budget").toString();
            newProtagonistID = doc.object().value("protagonistID").toString();
            newDirectorID = doc.object().value("directorID").toString();
            newStudioName = doc.object().value("studioName").toString();

            if (doc.object().value("additionally").toString() == "basicOnly")
            {
                QSqlQuery* query = new QSqlQuery(db);
                query->prepare("UPDATE Movie SET MovieID=?, Title=?, ReleaseDate=?, BoxOffice=?, Budget=?, ProtagonistID=?, DirectorID=?, StudioName=? WHERE MovieID=?");
                query->bindValue(0, newMovieID);
                query->bindValue(1, newTitle);
                query->bindValue(2, newReleaseDate);
                query->bindValue(3, newBoxOffice);
                query->bindValue(4, newBudget);
                query->bindValue(5, newProtagonistID);
                query->bindValue(6, newDirectorID);
                query->bindValue(7, newStudioName);
                query->bindValue(8, oldMovieID);

                if(!query->exec())
                {
                    qDebug() << "Запрос на обновление фильма составлен неверно!";
                }
                else
                {
                    if (checkMovieID(newMovieID))
                    {
                        qDebug()<<"Клиент " << socketDescriptor << " обновил фильм с id = " << oldMovieID;
                        socket->write("{\"type\":\"updateMovie\",\"params\":\"movieSuccessfullyUpdated\"}");
                        socket->waitForBytesWritten(500);
                    }
                    else
                    {
                        qDebug()<<"Клиент " << socketDescriptor << " не смог обновить фильм с id = " << oldMovieID;
                        socket->write("{\"type\":\"updateMovie\",\"params\":\"movieUnsuccessfullyUpdated\"}");
                        socket->waitForBytesWritten(500);
                    }
                }
            }
            else if (doc.object().value("additionally").toString() == "poster")
            {
                socket->write("{\"type\":\"updateMovie\",\"params\":\"requestSizeNewPoster\"}");
                socket->waitForBytesWritten(500);
            }
            else if (doc.object().value("additionally").toString() == "scenario")
            {
                socket->write("{\"type\":\"updateMovie\",\"params\":\"requestSizeNewScenario\"}");
                socket->waitForBytesWritten(500);
            }
            else if (doc.object().value("additionally").toString() == "posterAndScenario")
            {
                socket->write("{\"type\":\"updateMovie\",\"params\":\"requestSizeNewPoster\",\"additionally\":\"scenario\"}");
                socket->waitForBytesWritten(500);
            }

        }
        else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "sizeNewPoster"))
        {
            requireSize = doc.object().value("length").toInt();

            newPosterArrives = true;

            if (doc.object().value("additionally").toString() == "scenario") posterAndScenario = true;

            socket->write("{\"type\":\"updateMovie\",\"params\":\"requestNewPoster\"}");
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "sizeNewScenario"))
        {
            requireSize = doc.object().value("length").toInt();

            newScenarioArrives = true;
            socket->write("{\"type\":\"updateMovie\",\"params\":\"requestNewScenario\"}");
            socket->waitForBytesWritten(500);
        }
        else
        {
            //qDebug() << "type: " << doc.object().value("type").toString() << ", params: " << doc.object().value("params").toString();
            complexData = true;
            mySocketReady();
        }
    }
    else if (posterArrives)
    {
        //qDebug() << "Сколько пришло: " << Data.size() << ", сколько должно быть: " << requireSize;
        if (requireSize == Data.size())
        {
            newPoster = Data;

            socket->write("{\"type\":\"addNewMovie\",\"params\":\"findSizeScenario\"}");
            socket->waitForBytesWritten(500);

            posterArrives = false;
        }
        else
        {
            complexData = true;
            mySocketReady();
        }
    }
    else if (scenarioArrives)
    {
        //qDebug() << "Сколько пришло: " << Data.size() << ", сколько должно быть: " << requireSize;
        if (requireSize == Data.size())
        {
            scenarioArrives = false;
            if (db.isOpen())
            {
                QString newMovieID;
                QSqlQuery* queryNewMovieID = new QSqlQuery(db);
                if (queryNewMovieID->exec("SELECT MAX(MovieID) FROM Movie"))
                {
                    queryNewMovieID->first();
                    newMovieID = QString::number(queryNewMovieID->value(0).toInt() + 1);
                }

                QSqlQuery* query = new QSqlQuery(db);
                query->prepare("INSERT INTO Movie (MovieID, StudioName, DirectorID, ProtagonistID, Title, ReleaseDate, BoxOffice, Budget, Poster, Scenario) "
                               "VALUES (:MovieID, :StudioName, :DirectorID, :ProtagonistID, :Title, :ReleaseDate, :BoxOffice, :Budget, :Poster, :Scenario)");
                query->bindValue(":MovieID", newMovieID);
                query->bindValue(":StudioName", newStudioName);
                query->bindValue(":DirectorID", newDirectorID);
                query->bindValue(":ProtagonistID", newProtagonistID);
                query->bindValue(":Title", newTitle);
                query->bindValue(":ReleaseDate", newReleaseDate);
                query->bindValue(":BoxOffice", newBoxOffice);
                query->bindValue(":Budget", newBudget);
                query->bindValue(":Poster", newPoster);
                query->bindValue(":Scenario", Data);

                if(!query->exec())
                {
                    qDebug() << "Запрос на добавление фильма составлен неверно!";
                }
                else
                {
                    qDebug()<<"Клиент " << socketDescriptor << " добавил новый фильм с id = " << newMovieID;
                    socket->write("{\"type\":\"addNewMovie\",\"params\":\"movieAddedSuccessfully\"}");
                    socket->waitForBytesWritten(500);
                }
            }
        }
        else
        {
            complexData = true;
            mySocketReady();
        }
    }
    else if (actorPortraitArrives)
    {        
        //qDebug() << "Сколько пришло: " << Data.size() << ", сколько должно быть: " << requireSize;
        if (requireSize == Data.size())
        {
            actorPortraitArrives = false;

            if (db.isOpen())
            {
                QString newActorID;
                QSqlQuery* queryNewActorID = new QSqlQuery(db);
                if (queryNewActorID->exec("SELECT MAX(ActorID) FROM Actor"))
                {
                    queryNewActorID->first();
                    newActorID = QString::number(queryNewActorID->value(0).toInt() + 1);
                }

                QSqlQuery* query = new QSqlQuery(db);
                query->prepare("INSERT INTO Actor (ActorID, FirstName, LastName, DateOfBirth, ActorPortrait) "
                               "VALUES (:ActorID, :FirstName, :LastName, :DateOfBirth, :ActorPortrait)");
                query->bindValue(":ActorID", newActorID);
                query->bindValue(":FirstName", newFirstNameActor);
                query->bindValue(":LastName", newLastNameActor);
                query->bindValue(":DateOfBirth", newDateOfBirthActor);
                query->bindValue(":ActorPortrait", Data);

                if(!query->exec())
                {
                    qDebug() << "Запрос на добавление актёра составлен неверно!";
                }
                else
                {
                    qDebug()<<"Клиент " << socketDescriptor << " добавил нового актёра с id = " << newActorID;
                    socket->write("{\"type\":\"addNewActor\",\"params\":\"actorAddedSuccessfully\"}");
                    socket->waitForBytesWritten(500);
                }
            }
            else
            {
                socket->write("{\"type\":\"connect\", \"status\":\"no\"}");
                qDebug()<<"Соединение с БД НЕ установлено";
            }
        }
        else
        {
            complexData = true;
            mySocketReady();
        }
    }
    else if (newPosterArrives)
    {
        //qDebug() << "Сколько пришло: " << Data.size() << ", сколько должно быть: " << requireSize;
        if (requireSize == Data.size())
        {
            newPoster = Data;

            if (!posterAndScenario)
            {
                QSqlQuery* query = new QSqlQuery(db);
                query->prepare("UPDATE Movie SET MovieID=?, Title=?, ReleaseDate=?, BoxOffice=?, Budget=?, ProtagonistID=?, DirectorID=?, StudioName=?, Poster=? WHERE MovieID=?");
                query->bindValue(0, newMovieID);
                query->bindValue(1, newTitle);
                query->bindValue(2, newReleaseDate);
                query->bindValue(3, newBoxOffice);
                query->bindValue(4, newBudget);
                query->bindValue(5, newProtagonistID);
                query->bindValue(6, newDirectorID);
                query->bindValue(7, newStudioName);
                query->bindValue(8, newPoster);
                query->bindValue(9, oldMovieID);

                if(!query->exec())
                {
                    qDebug() << "Запрос на обновление фильма составлен неверно!";
                }
                else
                {
                    if (checkMovieID(newMovieID))
                    {
                        qDebug()<<"Клиент " << socketDescriptor << " обновил фильм с id = " << oldMovieID;
                        socket->write("{\"type\":\"updateMovie\",\"params\":\"movieSuccessfullyUpdated\"}");
                        socket->waitForBytesWritten(500);
                    }
                    else
                    {
                        qDebug()<<"Клиент " << socketDescriptor << " не смог обновить фильм с id = " << oldMovieID;
                        socket->write("{\"type\":\"updateMovie\",\"params\":\"movieUnsuccessfullyUpdated\"}");
                        socket->waitForBytesWritten(500);
                    }
                }
            }
            else
            {
                socket->write("{\"type\":\"updateMovie\",\"params\":\"requestSizeNewScenario\"}");
                socket->waitForBytesWritten(500);
            }

            newPosterArrives = false;
        }
        else
        {
            complexData = true;
            mySocketReady();
        }
    }
    else if (newScenarioArrives)
    {
        //qDebug() << "Сколько пришло: " << Data.size() << ", сколько должно быть: " << requireSize;
        if (requireSize == Data.size())
        {
            if (!posterAndScenario)
            {
                QSqlQuery* query = new QSqlQuery(db);
                query->prepare("UPDATE Movie SET MovieID=?, Title=?, ReleaseDate=?, BoxOffice=?, Budget=?, ProtagonistID=?, DirectorID=?, StudioName=?, Scenario=? WHERE MovieID=?");
                query->bindValue(0, newMovieID);
                query->bindValue(1, newTitle);
                query->bindValue(2, newReleaseDate);
                query->bindValue(3, newBoxOffice);
                query->bindValue(4, newBudget);
                query->bindValue(5, newProtagonistID);
                query->bindValue(6, newDirectorID);
                query->bindValue(7, newStudioName);
                query->bindValue(8, Data);
                query->bindValue(9, oldMovieID);

                if(!query->exec())
                {
                    qDebug() << "Запрос на обновление фильма составлен неверно!";
                }
                else
                {
                    qDebug()<<"Клиент " << socketDescriptor << " обновил фильм с id = " << oldMovieID;
                    socket->write("{\"type\":\"updateMovie\",\"params\":\"movieSuccessfullyUpdated\"}");
                    socket->waitForBytesWritten(500);
                }
            }
            else
            {
                posterAndScenario = false;

                QSqlQuery* query = new QSqlQuery(db);
                query->prepare("UPDATE Movie SET MovieID=?, Title=?, ReleaseDate=?, BoxOffice=?, Budget=?, ProtagonistID=?, DirectorID=?, StudioName=?, Poster=?, Scenario=? WHERE MovieID=?");
                query->bindValue(0, newMovieID);
                query->bindValue(1, newTitle);
                query->bindValue(2, newReleaseDate);
                query->bindValue(3, newBoxOffice);
                query->bindValue(4, newBudget);
                query->bindValue(5, newProtagonistID);
                query->bindValue(6, newDirectorID);
                query->bindValue(7, newStudioName);
                query->bindValue(8, newPoster);
                query->bindValue(9, Data);
                query->bindValue(10, oldMovieID);

                if(!query->exec())
                {
                    qDebug() << "Запрос на обновление фильма составлен неверно!";
                }
                else
                {
                    if (checkMovieID(newMovieID))
                    {
                        qDebug()<<"Клиент " << socketDescriptor << " обновил фильм с id = " << oldMovieID;
                        socket->write("{\"type\":\"updateMovie\",\"params\":\"movieSuccessfullyUpdated\"}");
                        socket->waitForBytesWritten(500);
                    }
                    else
                    {
                        qDebug()<<"Клиент " << socketDescriptor << " не смог обновить фильм с id = " << oldMovieID;
                        socket->write("{\"type\":\"updateMovie\",\"params\":\"movieUnsuccessfullyUpdated\"}");
                        socket->waitForBytesWritten(500);
                    }
                }
            }

            newScenarioArrives = false;
        }
        else
        {
            complexData = true;
            mySocketReady();
        }
    }
    else
    {
        socket->write("{\"type\":\"resultSelect\", \"status\":\"no\"}");
        qDebug()<<"Соединение с БД НЕ установлено!";
    }
}

void socketThread::socketDisc()
{
    qDebug()<<"Клиент " << socketDescriptor << " отключился";
    socket->deleteLater();
    db.close();
    quit();
}


