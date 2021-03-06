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

bool socketThread::checkInformationOnFilm(QString movieID, QString title, QString releaseDate, QString boxOffice, QString budget)
{
    QSqlQuery* query = new QSqlQuery(db);
    if (query->exec("SELECT Title, ReleaseDate, BoxOffice, Budget FROM Movie WHERE MovieID = " + movieID))
    {
        query->first();
        if ((query->value(0).toString() == title) && (query->value(1).toString() == releaseDate) && (query->value(2).toString() == boxOffice) && (query->value(3).toString() == budget))
        {
            return true;
        }
        return false;
    }
}

bool socketThread::checkDirectorID(QString directorID)
{
    QList <QString> listDirectorID;

    QSqlQuery* queryAllDirectorID = new QSqlQuery(db);
    if (queryAllDirectorID->exec("SELECT DirectorID FROM Director"))
    {
        while (queryAllDirectorID->next())
        {
            listDirectorID.append(queryAllDirectorID->value(0).toString());
        }
    }

    foreach( QString value, listDirectorID )
    {
        if (value == directorID)
        {
            return true;
        }
    }

    return false;
}

bool socketThread::checkInformationDirector(QString directorID, QString firstName, QString lastName, QString dateOfBirtday)
{
    QSqlQuery* query = new QSqlQuery(db);
    if (query->exec("SELECT FirstName, LastName, DateOfBirth FROM Director WHERE DirectorID = " + directorID))
    {
        query->first();
        if ((query->value(0).toString() == firstName) && (query->value(1).toString() == lastName) && (query->value(2).toString() == dateOfBirtday))
        {
            return true;
        }
        return false;
    }
}

bool socketThread::checkStudioName(QString studioName)
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

    foreach( QString value, listStudioName )
    {
        if (value == studioName)
        {
            return true;
        }
    }

    return false;
}

bool socketThread::checkActorID(QString actorID)
{
    QList <QString> listActorID;

    QSqlQuery* queryAllActorID = new QSqlQuery(db);
    if (queryAllActorID->exec("SELECT ActorID FROM Actor"))
    {
        while (queryAllActorID->next())
        {
            listActorID.append(queryAllActorID->value(0).toString());
        }
    }

    foreach( QString value, listActorID )
    {
        if (value == actorID)
        {
            return true;
        }
    }

    return false;
}

bool socketThread::checkInformationActor(QString actorID, QString firstName, QString lastName, QString dateOfBirtday)
{
    QSqlQuery* query = new QSqlQuery(db);
    if (query->exec("SELECT FirstName, LastName, DateOfBirth FROM Actor WHERE ActorID = " + actorID))
    {
        query->first();
        if ((query->value(0).toString() == firstName) && (query->value(1).toString() == lastName) && (query->value(2).toString() == dateOfBirtday))
        {
            return true;
        }
        return false;
    }
}

bool socketThread::checkProtagonistID(QString protagonistID)
{
    QList <QString> listProtagonistID;

    QSqlQuery* queryAllProtagonistID = new QSqlQuery(db);
    if (queryAllProtagonistID->exec("SELECT ProtagonistID FROM Protagonist"))
    {
        while (queryAllProtagonistID->next())
        {
            listProtagonistID.append(queryAllProtagonistID->value(0).toString());
        }
    }

    foreach( QString value, listProtagonistID )
    {
        if (value == protagonistID)
        {
            return true;
        }
    }

    return false;
}

bool socketThread::checkInformationProtagonist(QString protagonistID, QString oldName)
{
    QSqlQuery* query = new QSqlQuery(db);
    if (query->exec("SELECT Name FROM Protagonist WHERE ProtagonistID = " + protagonistID))
    {
        query->first();
        if (query->value(0).toString() == oldName)
        {
            return true;
        }
        return false;
    }
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
    //qDebug() << "Data: " << Data;
    doc = QJsonDocument::fromJson(Data, &docError);
    //qDebug() << "doc: " << doc;
    if ((docError.errorString()=="no error occurred") && (!posterArrives) && (!actorPortraitArrives) && (!scenarioArrives))
    {        
        if ((doc.object().value("type").toString() == "connection") && (doc.object().value("params").toString() == "connectionInformation"))
        {
            qDebug()<< "Клиент " << socketDescriptor<<" запрашивает подключение к серверу БД";
            QString connectionName = QString::number(socketDescriptor);
            db = QSqlDatabase::addDatabase("QODBC", connectionName);
            db.setDatabaseName("DRIVER={SQL Server};"
                               "SERVER="+doc.object().value("adressDB").toString()+";"
                               "DATABASE="+doc.object().value("nameDB").toString()+";");
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

            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[]}");
            }
            else
            {
                itog.append("]}");
            }

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
            if (checkMovieID(doc.object().value("id").toString()))
            {
                if (checkInformationOnFilm(doc.object().value("id").toString(), doc.object().value("title").toString(),
                                           doc.object().value("releaseDate").toString(), doc.object().value("boxOffice").toString(),
                                           doc.object().value("budget").toString()))
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
                            if (directorID != "0")
                            {
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
                                itog.append("{\"directorID\":\"NULL\",\"firstName\":\"NULL\",\"lastName\":\"NULL\",\"dateOfBirth\":\"NULL\"}");
                                itog.append("],\"resultStudioName\":\"");
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
                            if (studioName != "")
                            {
                                itog.append(studioName + "\",\"resultProtagonist\":[");
                            }
                            else
                            {
                                itog.append("NULL\",\"resultProtagonist\":[");
                            }
                        }
                        else
                        {
                            //ошибка
                            qDebug() << "Ошибка в получении названия студии: " << queryStudioName->lastError().text();
                        }

                        QSqlQuery* queryProtagonist = new QSqlQuery(db);
                        QString protagonistID;
                        if (queryProtagonist->exec("SELECT ProtagonistID FROM Movie WHERE MovieID = " + movieID))
                        {
                            queryProtagonist->first();
                            protagonistID = queryProtagonist->value(0).toString();
                            if (protagonistID != "0")
                            {
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
                                itog.append("{\"protagonistID\":\"NULL\",\"name\":\"NULL\"}");
                                itog.append("],\"resultActor\":[");
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
                            if ((actorID != "") && (actorID != "0"))
                            {
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
                                itog.append("{\"actorID\":\"NULL\",\"firstName\":\"NULL\",\"lastName\":\"NULL\",\"dateOfBirth\":\"NULL\"}");
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
                else
                {
                    qDebug()<<"Клиент " << socketDescriptor << " запрашивает информацию о несуществующем фильме с id = " << doc.object().value("id").toString();
                    socket->write("{\"type\":\"selectSelectInformationOnFilm\",\"params\":\"fail\"}");
                    socket->waitForBytesWritten(500);
                }
            }
            else
            {
                qDebug()<<"Клиент " << socketDescriptor << " запрашивает информацию о несуществующем фильме с id = " << doc.object().value("id").toString();
                socket->write("{\"type\":\"selectSelectInformationOnFilm\",\"params\":\"fail\"}");
                socket->waitForBytesWritten(500);
            }

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

                    if (baPoster != "")
                    {
                        socket->write("{\"type\":\"afterSelectInformationOnFilm\",\"params\":\"size\",\"length\":"+QByteArray::number(baPoster.size())+"}");
                        socket->waitForBytesWritten(500);
                    }
                    else
                    {
                        QString str = "NULL";
                        socket->write("{\"type\":\"afterSelectInformationOnFilm\",\"params\":\"size\",\"length\":"+QByteArray::number(str.size())+"}");
                        socket->waitForBytesWritten(500);
                    }
                }
            }

        }
        else if (doc.object().value("type").toString() == "selectPoster")
        {
            if (baPoster != "")
            {
                socket->write(baPoster);
                socket->waitForBytesWritten(500);
            }
            else
            {
                socket->write("NULL");
                socket->waitForBytesWritten(500);
            }
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

                    if (baActorPortrait != "")
                    {
                        socket->write("{\"type\":\"selectActorPortrait\",\"params\":\"size\",\"length\":"+QByteArray::number(baActorPortrait.size())+"}");
                        socket->waitForBytesWritten(500);
                    }
                    else
                    {
                        QString str = "NULL";
                        socket->write("{\"type\":\"selectActorPortrait\",\"params\":\"size\",\"length\":"+QByteArray::number(str.size())+"}");
                        socket->waitForBytesWritten(500);
                    }
                }
            }
        }
        else if (doc.object().value("type").toString() == "selectActorPortrait")
        {
            if (baActorPortrait != "")
            {
                socket->write(baActorPortrait);
            }
            else
            {
                socket->write("NULL");
            }

            //qDebug() << "Размер ответного сообщения: " << socket->bytesToWrite();
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "selectScenario") && (doc.object().value("params").toString() == "selectSize"))
        {
            QSqlQuery* queryScenario = new QSqlQuery(db);
            if (queryScenario->exec("SELECT Scenario FROM Movie WHERE MovieID = " + movieID))
            {
                queryScenario->first();
                baScenario = queryScenario->value(0).toByteArray();

                if (baScenario != "")
                {
                    socket->write("{\"type\":\"selectScenario\",\"params\":\"size\",\"length\":"+QByteArray::number(baScenario.size())+"}");
                    socket->waitForBytesWritten(500);
                }
                else
                {
                    QString str = "NULL";
                    socket->write("{\"type\":\"selectScenario\",\"params\":\"size\",\"length\":"+QByteArray::number(str.size())+"}");
                    socket->waitForBytesWritten(500);
                }
            }
        }
        else if (doc.object().value("type").toString() == "selectScenario")
        {
            if (baScenario != "")
            {
                socket->write(baScenario);
                socket->waitForBytesWritten(500);
            }
            else
            {
                socket->write("NULL");
                socket->waitForBytesWritten(500);
            }
        }
        else if ((doc.object().value("type").toString() == "addNewMovie") && (doc.object().value("params").toString() == "selectAllID"))
        {
            itog = "{\"type\":\"addNewMovie\",\"params\":\"resultSelectAllID\",\"allMovieID\":[";

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

            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[],\"allProtagonistID\":[");
            }
            else
            {
                itog.append("],\"allProtagonistID\":[");
            }

            QSqlQuery* queryAllProtagonistID = new QSqlQuery(db);
            if (queryAllProtagonistID->exec("SELECT ProtagonistID FROM Protagonist"))
            {
                while (queryAllProtagonistID->next())
                {
                    itog.append("{\"ProtagonistID\":\""+queryAllProtagonistID->value(0).toString()+
                                "\"},");
                }
                itog.remove(itog.length()-1,1);
            }

            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[],\"allDirectorID\":[");
            }
            else
            {
                itog.append("],\"allDirectorID\":[");
            }

            QSqlQuery* queryAllDirectorID = new QSqlQuery(db);
            if (queryAllDirectorID->exec("SELECT DirectorID FROM Director"))
            {
                while (queryAllDirectorID->next())
                {
                    itog.append("{\"DirectorID\":\""+queryAllDirectorID->value(0).toString()+
                                "\"},");
                }
                itog.remove(itog.length()-1,1);
            }

            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[],\"allStudioName\":[");
            }
            else
            {
                itog.append("],\"allStudioName\":[");
            }

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

            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[]}");
            }
            else
            {
                itog.append("]}");
            }

            socket->write(itog);
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

            qDebug()<< "Клиент " << socketDescriptor<<" запрашивает обновление информации о фильмах";
            downloadInformationAboutFilms();

            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[]}");
            }
            else
            {
                itog.append("]}");
            }

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
            QSqlQuery* query = new QSqlQuery(db);
            if (doc.object().value("dateOfBirth").toString() == "")
            {
                query->prepare("INSERT INTO Director (FirstName, LastName, DateOfBirth) "
                               "VALUES (:FirstName, :LastName, NULL)");
            }
            else
            {
                query->prepare("INSERT INTO Director (FirstName, LastName, DateOfBirth) "
                               "VALUES (:FirstName, :LastName, :DateOfBirth)");
            }
            if (doc.object().value("firstName").toString() != "")
            {
                query->bindValue(":FirstName", doc.object().value("firstName").toString());
            }
            else
            {
                query->bindValue(":FirstName", QVariant (QVariant :: String));
            }
            if (doc.object().value("lastName").toString() != "")
            {
                query->bindValue(":LastName", doc.object().value("lastName").toString());
            }
            else
            {
                query->bindValue(":LastName", QVariant (QVariant :: String));
            }
            if (doc.object().value("dateOfBirth").toString() != "")
            {
                query->bindValue(":DateOfBirth", doc.object().value("dateOfBirth").toString());
            }

            if(!query->exec())
            {
                qDebug() << "Запрос на добавление режиссёра составлен неверно!";
            }
            else
            {
                qDebug()<<"Клиент " << socketDescriptor << " добавил нового режиссёра";
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

            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[]}");
            }
            else
            {
                itog.append("]}");
            }

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
                        qDebug()<<"Клиент " << socketDescriptor << " добавил новую киностудию";
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
        else if ((doc.object().value("type").toString() == "addNewProtagonist") && (doc.object().value("params").toString() == "selectAllActorID"))
        {
            itog = "{\"type\":\"addNewProtagonist\",\"params\":\"allActorID\",\"result\":[";

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

            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[]}");
            }
            else
            {
                itog.append("]}");
            }

            socket->write(itog);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "addNewProtagonist") && (doc.object().value("params").toString() == "data"))
        {
            QSqlQuery* query = new QSqlQuery(db);
            query->prepare("INSERT INTO Protagonist (ActorID, Name) "
                           "VALUES (:ActorID, :Name)");
            if (doc.object().value("actorID").toString() == "")
            {
                query->bindValue(":ActorID", QVariant (QVariant :: String));
            }
            else
            {
                query->bindValue(":ActorID", doc.object().value("actorID").toString());
            }
            if (doc.object().value("name").toString() == "")
            {
                query->bindValue(":Name", QVariant (QVariant :: String));
            }
            else
            {
                query->bindValue(":Name", doc.object().value("name").toString());
            }

            if(!query->exec())
            {
                qDebug() << "Запрос на добавление режиссёра составлен неверно!";
            }
            else
            {
                qDebug()<<"Клиент " << socketDescriptor << " добавил нового главного героя";
                socket->write("{\"type\":\"addNewProtagonist\",\"params\":\"protagonistAddedSuccessfully\"}");
                socket->waitForBytesWritten(500);
            }
        }
        else if ((doc.object().value("type").toString() == "selectAllMovieID") && (doc.object().value("params").toString() == "findAllMovieID"))
        {
            itog = "{\"type\":\"selectAllMovieID\",\"params\":\"itog\",\"result\":[";

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

            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[]}");
            }
            else
            {
                itog.append("]}");
            }

            socket->write(itog);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "findMovieInformation"))
        {
            if (db.transaction())
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
            else
            {
                qDebug() << "Failed to start transaction mode";
            }
        }
        else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "requestPosterSize"))
        {
            QSqlQuery* queryPoster = new QSqlQuery(db);
            if (queryPoster->exec("SELECT Poster FROM Movie WHERE MovieID = " + movieID))
            {
                queryPoster->first();
                baPoster = queryPoster->value(0).toByteArray();

                if (baPoster != "")
                {
                    socket->write("{\"type\":\"updateMovie\",\"params\":\"sizePoster\",\"length\":"+QByteArray::number(baPoster.size())+"}");
                    socket->waitForBytesWritten(500);
                }
                else
                {
                    QString str = "NULL";
                    socket->write("{\"type\":\"updateMovie\",\"params\":\"sizePoster\",\"length\":"+QByteArray::number(str.size())+"}");
                    socket->waitForBytesWritten(500);
                }
            }
        }
        else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "selectPoster"))
        {
            if (baPoster != "")
            {
                socket->write(baPoster);
                socket->waitForBytesWritten(500);
            }
            else
            {
                socket->write("NULL");
                socket->waitForBytesWritten(500);
            }
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

                    if (baScenario != "")
                    {
                        socket->write("{\"type\":\"updateMovie\",\"params\":\"sizeScenario\",\"length\":"+QByteArray::number(baScenario.size())+"}");
                        socket->waitForBytesWritten(500);
                    }
                    else
                    {
                        QString str = "NULL";
                        socket->write("{\"type\":\"updateMovie\",\"params\":\"sizeScenario\",\"length\":"+QByteArray::number(str.size())+"}");
                        socket->waitForBytesWritten(500);
                    }
                }
            }
        }
        else if ((doc.object().value("type").toString() == "updateMovie") && (doc.object().value("params").toString() == "selectScenario"))
        {
            if (baScenario != "")
            {
                socket->write(baScenario);
                socket->waitForBytesWritten(500);
            }
            else
            {
                socket->write("NULL");
                socket->waitForBytesWritten(500);
            }
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
            oldMovieID = doc.object().value("oldMovieID").toString();
            newTitle = doc.object().value("title").toString();
            newReleaseDate = doc.object().value("releaseDate").toString();
            newBoxOffice = doc.object().value("boxOffice").toString();
            newBudget = doc.object().value("budget").toString();
            newProtagonistID = doc.object().value("protagonistID").toString();
            newDirectorID = doc.object().value("directorID").toString();
            newStudioName = doc.object().value("studioName").toString();
            oldTitle = doc.object().value("oldTitle").toString();
            oldReleaseDate = doc.object().value("oldReleaseDate").toString();
            oldBoxOffice = doc.object().value("oldBoxOffice").toString();
            oldBudget = doc.object().value("oldBudget").toString();

            if (doc.object().value("additionally").toString() == "basicOnly")
            {
                if (checkInformationOnFilm(oldMovieID, oldTitle,
                                           oldReleaseDate, oldBudget, oldBoxOffice))
                {
                    QSqlQuery* query = new QSqlQuery(db);
                    if (doc.object().value("releaseDate").toString() == "")
                    {
                        query->prepare("UPDATE Movie SET Title=:Title, ReleaseDate=NULL, BoxOffice=:BoxOffice, Budget=:Budget, ProtagonistID=:ProtagonistID, DirectorID=:DirectorID, StudioName=:StudioName WHERE MovieID=:oldMovieID");
                    }
                    else
                    {
                        query->prepare("UPDATE Movie SET Title=:Title, ReleaseDate=:ReleaseDate, BoxOffice=:BoxOffice, Budget=:Budget, ProtagonistID=:ProtagonistID, DirectorID=:DirectorID, StudioName=:StudioName WHERE MovieID=:oldMovieID");
                    }
                    if (newTitle != "")
                    {
                        query->bindValue(":Title", newTitle);
                    }
                    else
                    {
                        query->bindValue(":Title", QVariant (QVariant :: String));
                    }
                    if (doc.object().value("releaseDate").toString() != "")
                    {
                        query->bindValue(":ReleaseDate", newReleaseDate);
                    }
                    if (newBoxOffice == "")
                    {
                        query->bindValue(":BoxOffice", QVariant (QVariant :: Int));
                    }
                    else
                    {
                        query->bindValue(":BoxOffice", newBoxOffice);
                    }
                    if (newBudget == "")
                    {
                        query->bindValue(":Budget", QVariant (QVariant :: Int));
                    }
                    else
                    {
                        query->bindValue(":Budget", newBudget);
                    }
                    if (doc.object().value("protagonistID").toString() == "")
                    {
                        query->bindValue(":ProtagonistID", QVariant (QVariant :: String));
                    }
                    else
                    {
                        query->bindValue(":ProtagonistID", newProtagonistID);
                    }
                    if (doc.object().value("directorID").toString() == "")
                    {
                        query->bindValue(":DirectorID", QVariant (QVariant :: String));
                    }
                    else
                    {
                        query->bindValue(":DirectorID", newDirectorID);
                    }
                    if (doc.object().value("studioName").toString() == "")
                    {
                        query->bindValue(":StudioName", QVariant (QVariant :: String));
                    }
                    else
                    {
                        query->bindValue(":StudioName", newStudioName);
                    }
                    query->bindValue(":oldMovieID", oldMovieID);

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

                    if(!db.commit())
                    {
                        socket->write("{\"type\":\"updateMovie\",\"params\":\"movieUnsuccessfullyUpdated\"}");
                        socket->waitForBytesWritten(500);
                        db.rollback();
                    }
                }
                else
                {
                    socket->write("{\"type\":\"updateMovie\",\"params\":\"movieUnsuccessfullyUpdated\"}");
                    socket->waitForBytesWritten(500);
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
        else if ((doc.object().value("type").toString() == "updateDirector") && (doc.object().value("params").toString() == "findAllDirectorID"))
        {
            itog = "{\"type\":\"updateDirector\",\"params\":\"resultAllDirectorID\",\"result\":[";

            QSqlQuery* queryAllDirectorID = new QSqlQuery(db);
            if (queryAllDirectorID->exec("SELECT DirectorID FROM Director"))
            {
                while (queryAllDirectorID->next())
                {
                    itog.append("{\"DirectorID\":\""+queryAllDirectorID->value(0).toString()+
                                "\"},");
                }
                itog.remove(itog.length()-1,1);
            }

            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[]}");
            }
            else
            {
                itog.append("]}");
            }

            socket->write(itog);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "updateDirector") && (doc.object().value("params").toString() == "findDirectorInformation"))
        {
            directorID = doc.object().value("id").toString();

            itog = "{\"type\":\"updateDirector\",\"params\":\"directorInformation\",";

            QSqlQuery* director = new QSqlQuery(db);
            if (director->exec("SELECT FirstName, LastName, DateOfBirth FROM Director WHERE DirectorID = " + directorID))
            {
                director->first();
                itog.append("\"firstName\":\""+director->value(0).toString()+
                            "\",\"lastName\":\""+director->value(1).toString()+
                            "\",\"dateOfBirth\":\""+director->value(2).toString()+
                            "\"");
            }
            else
            {
                qDebug()<<"Запрос на фильмы не выполнен";
            }

            itog.append("}");

            socket->write(itog);
            socket->waitForBytesWritten(500);

            delete director;
        }
        else if ((doc.object().value("type").toString() == "updateDirector") && (doc.object().value("params").toString() == "sendDirectorInformation"))
        {
            oldDirectorID = doc.object().value("oldDirectorID").toString();
            newFirstName = doc.object().value("firstName").toString();
            newLastName = doc.object().value("lastName").toString();
            newDateOfBirth = doc.object().value("dateOfBirth").toString();
            oldFirstName = doc.object().value("oldFirstName").toString();
            oldLastName = doc.object().value("oldLastName").toString();
            oldDateOfBirth = doc.object().value("oldDateOfBirth").toString();

            if (checkInformationDirector(oldDirectorID, oldFirstName, oldLastName, oldDateOfBirth))
            {
                QSqlQuery* query = new QSqlQuery(db);
                if (newDateOfBirth == "")
                {
                    query->prepare("UPDATE Director SET FirstName=:FirstName, LastName=:LastName, DateOfBirth=NULL WHERE DirectorID=:oldDirectorID");
                }
                else
                {
                    query->prepare("UPDATE Director SET FirstName=:FirstName, LastName=:LastName, DateOfBirth=:DateOfBirth WHERE DirectorID=:oldDirectorID");
                }
                if (newFirstName != "")
                {
                    query->bindValue(":FirstName", newFirstName);
                }
                else
                {
                    query->bindValue(":FirstName", QVariant (QVariant :: String));
                }
                if (newLastName != "")
                {
                    query->bindValue(":LastName", newLastName);
                }
                else
                {
                    query->bindValue(":LastName", QVariant (QVariant :: String));
                }
                if (newDateOfBirth != "")
                {
                    query->bindValue(":DateOfBirth", newDateOfBirth);
                }
                query->bindValue(":oldDirectorID", oldDirectorID);

                if(!query->exec())
                {
                    qDebug() << "Запрос на обновление режиссёра составлен неверно!";
                }
                else
                {
                    qDebug()<<"Клиент " << socketDescriptor << " обновил режиссёра с id = " << oldDirectorID;
                    socket->write("{\"type\":\"updateDirector\",\"params\":\"directorSuccessfullyUpdated\"}");
                    socket->waitForBytesWritten(500);
                }
            }
            else
            {
                socket->write("{\"type\":\"updateDirector\",\"params\":\"directorUnsuccessfullyUpdated\"}");
                socket->waitForBytesWritten(500);
            }
        }
        else if ((doc.object().value("type").toString() == "updateStudio") && (doc.object().value("params").toString() == "findAllStudioName"))
        {
            itog = "{\"type\":\"updateStudio\",\"params\":\"resultAllStudioName\",\"result\":[";

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

            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[]}");
            }
            else
            {
                itog.append("]}");
            }

            socket->write(itog);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "updateStudio") && (doc.object().value("params").toString() == "sendStudioInformation"))
        {
            newStudioName = doc.object().value("newDirectorID").toString();
            oldStudioName = doc.object().value("oldDirectorID").toString();

            QSqlQuery* query = new QSqlQuery(db);
            query->prepare("UPDATE Studio SET StudioName=? WHERE StudioName=?");
            query->bindValue(0, newStudioName);
            query->bindValue(1, oldStudioName);

            if(!query->exec())
            {
                qDebug() << "Запрос на обновление киностудии составлен неверно!";
            }
            else
            {
                if (checkStudioName(newStudioName))
                {
                    qDebug()<<"Клиент " << socketDescriptor << " обновил киностудию с названием = " << oldStudioName;
                    socket->write("{\"type\":\"updateStudio\",\"params\":\"studioSuccessfullyUpdated\"}");
                    socket->waitForBytesWritten(500);
                }
                else
                {
                    qDebug()<<"Клиент " << socketDescriptor << " не смог обновить киностудию с названием = " << oldStudioName;
                    socket->write("{\"type\":\"updateStudio\",\"params\":\"studioUnsuccessfullyUpdated\"}");
                    socket->waitForBytesWritten(500);
                }
            }

        }
        else if ((doc.object().value("type").toString() == "updateActor") && (doc.object().value("params").toString() == "findAllActorID"))
        {
            itog = "{\"type\":\"updateActor\",\"params\":\"resultAllActorID\",\"result\":[";

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

            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[]}");
            }
            else
            {
                itog.append("]}");
            }

            socket->write(itog);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "updateActor") && (doc.object().value("params").toString() == "findActorInformation"))
        {
            actorID = doc.object().value("id").toString();

            itog = "{\"type\":\"updateActor\",\"params\":\"actorInformation\",";

            QSqlQuery* actor = new QSqlQuery(db);
            if (actor->exec("SELECT FirstName, LastName, DateOfBirth FROM Actor WHERE ActorID = " + actorID))
            {
                actor->first();
                itog.append("\"firstName\":\""+actor->value(0).toString()+
                            "\",\"lastName\":\""+actor->value(1).toString()+
                            "\",\"dateOfBirth\":\""+actor->value(2).toString()+
                            "\"");
            }
            else
            {
                qDebug()<<"Запрос на фильмы не выполнен";
            }

            itog.append("}");

            socket->write(itog);
            socket->waitForBytesWritten(500);

            delete actor;
        }
        else if ((doc.object().value("type").toString() == "updateActor") && (doc.object().value("params").toString() == "requestPortraitSize"))
        {
            QSqlQuery* queryPortrait = new QSqlQuery(db);
            if (queryPortrait->exec("SELECT ActorPortrait FROM Actor WHERE ActorID = " + actorID))
            {
                queryPortrait->first();
                baActorPortrait = queryPortrait->value(0).toByteArray();

                if (baActorPortrait != "")
                {
                    socket->write("{\"type\":\"updateActor\",\"params\":\"sizePortrait\",\"length\":"+QByteArray::number(baActorPortrait.size())+"}");
                    socket->waitForBytesWritten(500);
                }
                else
                {
                    QString str = "NULL";
                    socket->write("{\"type\":\"updateActor\",\"params\":\"sizePortrait\",\"length\":"+QByteArray::number(str.size())+"}");
                    socket->waitForBytesWritten(500);
                }
            }
        }
        else if ((doc.object().value("type").toString() == "updateActor") && (doc.object().value("params").toString() == "selectPortrait"))
        {
            if (baActorPortrait != "")
            {
                socket->write(baActorPortrait);
                socket->waitForBytesWritten(500);
            }
            else
            {
                socket->write("NULL");
                socket->waitForBytesWritten(500);
            }
        }
        else if ((doc.object().value("type").toString() == "updateActor") && (doc.object().value("params").toString() == "sendActorInformation"))
        {
            oldActorID = doc.object().value("oldActorID").toString();
            newFirstName = doc.object().value("firstName").toString();
            newLastName = doc.object().value("lastName").toString();
            newDateOfBirth = doc.object().value("dateOfBirth").toString();
            oldFirstName = doc.object().value("oldFirstName").toString();
            oldLastName = doc.object().value("oldLastName").toString();
            oldDateOfBirth = doc.object().value("oldDateOfBirth").toString();

            if (checkInformationActor(oldActorID, oldFirstName, oldLastName, oldDateOfBirth))
            {
                if (doc.object().value("additionally").toString() == "basicOnly")
                {
                    QSqlQuery* query = new QSqlQuery(db);
                    if (doc.object().value("dateOfBirth").toString() == "")
                    {
                        query->prepare("UPDATE Actor SET FirstName=:FirstName, LastName=:LastName, DateOfBirth=NULL WHERE ActorID=:oldActorID");
                    }
                    else
                    {
                        query->prepare("UPDATE Actor SET FirstName=:FirstName, LastName=:LastName, DateOfBirth=:DateOfBirth WHERE ActorID=:oldActorID");
                    }
                    if (newFirstName != "")
                    {
                        query->bindValue(":FirstName", newFirstName);
                    }
                    else
                    {
                        query->bindValue(":FirstName", QVariant (QVariant :: String));
                    }
                    if (newLastName != "")
                    {
                        query->bindValue(":LastName", newLastName);
                    }
                    else
                    {
                        query->bindValue(":LastName", QVariant (QVariant :: String));
                    }
                    if (newDateOfBirth != "")
                    {
                        query->bindValue(":DateOfBirth", newDateOfBirth);
                    }
                    query->bindValue(":oldActorID", oldActorID);

                    if(!query->exec())
                    {
                        qDebug() << "Запрос на обновление фильма составлен неверно!";
                    }
                    else
                    {
                        qDebug()<<"Клиент " << socketDescriptor << " обновил актёра с id = " << oldActorID;
                        socket->write("{\"type\":\"updateActor\",\"params\":\"actorSuccessfullyUpdated\"}");
                        socket->waitForBytesWritten(500);
                    }
                }
                else if (doc.object().value("additionally").toString() == "actorPortrait")
                {
                    socket->write("{\"type\":\"updateActor\",\"params\":\"requestSizeNewActorPortrait\"}");
                    socket->waitForBytesWritten(500);
                }
            }
            else
            {
                socket->write("{\"type\":\"updateActor\",\"params\":\"actorUnsuccessfullyUpdated\"}");
                socket->waitForBytesWritten(500);
            }
        }
        else if ((doc.object().value("type").toString() == "updateActor") && (doc.object().value("params").toString() == "sizeNewActorPortrait"))
        {
            requireSize = doc.object().value("length").toInt();

            newActorPortraitArrives = true;

            socket->write("{\"type\":\"updateActor\",\"params\":\"requestNewActorPortrait\"}");
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "updateProtagonist") && (doc.object().value("params").toString() == "findAllProtagonistID"))
        {
            itog = "{\"type\":\"updateProtagonist\",\"params\":\"resultAllProtagonistID\",\"result\":[";

            QSqlQuery* queryAllProtagonistID = new QSqlQuery(db);
            if (queryAllProtagonistID->exec("SELECT ProtagonistID FROM Protagonist"))
            {
                while (queryAllProtagonistID->next())
                {
                    itog.append("{\"ProtagonistID\":\""+queryAllProtagonistID->value(0).toString()+
                                "\"},");
                }
                itog.remove(itog.length()-1,1);
            }

            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[]}");
            }
            else
            {
                itog.append("]}");
            }

            socket->write(itog);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "updateProtagonist") && (doc.object().value("params").toString() == "findProtagonistInformation"))
        {
            protagonistID = doc.object().value("id").toString();

            itog = "{\"type\":\"updateProtagonist\",\"params\":\"protagonistInformation\",";

            QSqlQuery* protagonist = new QSqlQuery(db);
            if (protagonist->exec("SELECT Name, ActorID FROM Protagonist WHERE ProtagonistID = " + protagonistID))
            {
                protagonist->first();
                itog.append("\"name\":\""+protagonist->value(0).toString()+
                            "\",\"actorID\":\""+protagonist->value(1).toString()+
                            "\"");
            }
            else
            {
                qDebug()<<"Запрос на фильмы не выполнен";
            }

            itog.append(",\"allActorID\":[");
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

            itog.append("]}");

            socket->write(itog);
            socket->waitForBytesWritten(500);

            delete protagonist;
        }
        else if ((doc.object().value("type").toString() == "updateProtagonist") && (doc.object().value("params").toString() == "sendProtagonisInformation"))
        {
            if (checkInformationProtagonist(doc.object().value("oldAProtagonistID").toString(), doc.object().value("oldName").toString()))
            {
                QSqlQuery* query = new QSqlQuery(db);
                query->prepare("UPDATE Protagonist SET Name=:Name, ActorID=:ActorID WHERE ProtagonistID=:oldProtagonistID");
                if (doc.object().value("name").toString() == "")
                {
                    query->bindValue(":Name", QVariant (QVariant :: String));
                }
                else
                {
                    query->bindValue(":Name", doc.object().value("name").toString());
                }
                if (doc.object().value("actorID").toString() == "")
                {
                    query->bindValue(":ActorID", QVariant (QVariant :: String));
                }
                else
                {
                    query->bindValue(":ActorID", doc.object().value("actorID").toString());
                }
                query->bindValue(":oldProtagonistID", doc.object().value("oldAProtagonistID").toString());

                if(!query->exec())
                {
                    qDebug() << "Запрос на обновление киностудии составлен неверно!";
                }
                else
                {
                    qDebug()<<"Клиент " << socketDescriptor << " обновил главного героя с id = " << doc.object().value("oldAProtagonistID").toString();
                    socket->write("{\"type\":\"updateProtagonist\",\"params\":\"protagonistSuccessfullyUpdated\"}");
                    socket->waitForBytesWritten(500);
                }
            }
            else
            {
                socket->write("{\"type\":\"updateProtagonist\",\"params\":\"protagonistUnsuccessfullyUpdated\"}");
                socket->waitForBytesWritten(500);
            }


        }
        else if ((doc.object().value("type").toString() == "deleteMovie") && (doc.object().value("params").toString() == "sendID"))
        {
            if (checkMovieID(doc.object().value("id").toString()))
            {
                QSqlQuery* query = new QSqlQuery(db);
                query->prepare("DELETE FROM Movie WHERE MovieID=:MovieID");
                query->bindValue(":MovieID", doc.object().value("id").toString());

                if(!query->exec())
                {
                    qDebug() << "Запрос на удаление фильма составлен неверно!";
                }
                else
                {
                    qDebug()<<"Клиент " << socketDescriptor << " удалил фильм с id = " << doc.object().value("id").toString();
                    socket->write("{\"type\":\"deleteMovie\",\"params\":\"movieSuccessfullyDeleted\"}");
                    socket->waitForBytesWritten(500);
                }
            }
            else
            {
                qDebug()<<"Клиент " << socketDescriptor << " не смог удалить фильм с id = " << doc.object().value("id").toString();
                socket->write("{\"type\":\"deleteMovie\",\"params\":\"movieUnsuccessfullyDeleted\"}");
                socket->waitForBytesWritten(500);
            }
        }
        else if ((doc.object().value("type").toString() == "outputDirector") && (doc.object().value("params").toString() == "findAllDirector"))
        {
            itog = "{\"type\":\"outputDirector\",\"params\":\"resultAllDirector\",\"result\":[";
            QSqlQuery* queryAllDirector = new QSqlQuery(db);
            if (queryAllDirector->exec("SELECT DirectorID, FirstName, LastName, DateOfBirth FROM Director"))
            {
                while (queryAllDirector->next())
                {
                    itog.append("{\"directorID\":\""+queryAllDirector->value(0).toString()+
                                "\",\"firstName\":\""+queryAllDirector->value(1).toString()+
                                "\",\"lastName\":\""+queryAllDirector->value(2).toString()+
                                "\",\"dateOfBirth\":\""+queryAllDirector->value(3).toString()+
                                "\"},");
                }
                itog.remove(itog.length()-1,1);
            }

            delete queryAllDirector;
            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[]}");
            }
            else
            {
                itog.append("]}");
            }
            socket->write(itog);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "deleteDirector") && (doc.object().value("params").toString() == "sendID"))
        {
            if (checkDirectorID(doc.object().value("id").toString()))
            {
                QSqlQuery* query = new QSqlQuery(db);
                query->prepare("DELETE FROM Director WHERE DirectorID=:DirectorID");
                query->bindValue(":DirectorID", doc.object().value("id").toString());

                if(!query->exec())
                {
                    qDebug() << "Запрос на удаление режиссёра составлен неверно!";
                }
                else
                {
                    qDebug()<<"Клиент " << socketDescriptor << " удалил режиссёра с id = " << doc.object().value("id").toString();
                    socket->write("{\"type\":\"deleteDirector\",\"params\":\"directorSuccessfullyDeleted\"}");
                    socket->waitForBytesWritten(500);
                }
            }
            else
            {
                qDebug()<<"Клиент " << socketDescriptor << " не смог удалить режиссёра с id = " << doc.object().value("id").toString();
                socket->write("{\"type\":\"deleteDirector\",\"params\":\"directorUnsuccessfullyDeleted\"}");
                socket->waitForBytesWritten(500);
            }
        }
        else if ((doc.object().value("type").toString() == "outputStudio") && (doc.object().value("params").toString() == "findAllStudio"))
        {
            itog = "{\"type\":\"outputStudio\",\"params\":\"resultAllStudio\",\"result\":[";
            QSqlQuery* queryAllStudioName = new QSqlQuery(db);
            if (queryAllStudioName->exec("SELECT StudioName FROM Studio"))
            {
                while (queryAllStudioName->next())
                {
                    itog.append("{\"studioName\":\""+queryAllStudioName->value(0).toString()+
                                "\"},");
                }
                itog.remove(itog.length()-1,1);
            }

            delete queryAllStudioName;
            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[]}");
            }
            else
            {
                itog.append("]}");
            }
            socket->write(itog);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "deleteStudio") && (doc.object().value("params").toString() == "sendID"))
        {
            if (checkStudioName(doc.object().value("id").toString()))
            {
                QSqlQuery* query = new QSqlQuery(db);
                query->prepare("DELETE FROM Studio WHERE StudioName=:StudioName");
                query->bindValue(":StudioName", doc.object().value("id").toString());

                if(!query->exec())
                {
                    qDebug() << "Запрос на удаление киностудии составлен неверно!";
                }
                else
                {
                    qDebug()<<"Клиент " << socketDescriptor << " удалил киностудию с названием = " << doc.object().value("id").toString();
                    socket->write("{\"type\":\"deleteStudio\",\"params\":\"studioSuccessfullyDeleted\"}");
                    socket->waitForBytesWritten(500);
                }
            }
            else
            {
                qDebug()<<"Клиент " << socketDescriptor << " не смог удалить киностудию с названием = " << doc.object().value("id").toString();
                socket->write("{\"type\":\"deleteStudio\",\"params\":\"studioUnsuccessfullyDeleted\"}");
                socket->waitForBytesWritten(500);
            }
        }
        else if ((doc.object().value("type").toString() == "outputActor") && (doc.object().value("params").toString() == "findAllActor"))
        {
            itog = "{\"type\":\"outputActor\",\"params\":\"resultAllActor\",\"result\":[";
            QSqlQuery* queryAllActor = new QSqlQuery(db);
            if (queryAllActor->exec("SELECT ActorID, FirstName, LastName, DateOfBirth FROM Actor"))
            {
                while (queryAllActor->next())
                {

                    itog.append("{\"actorID\":\""+queryAllActor->value(0).toString()+
                                "\",\"firstName\":\""+queryAllActor->value(1).toString()+
                                "\",\"lastName\":\""+queryAllActor->value(2).toString()+
                                "\",\"dateOfBirth\":\""+queryAllActor->value(3).toString()+
                                "\"},");
                }
                itog.remove(itog.length()-1,1);
            }

            delete queryAllActor;
            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[]}");
            }
            else
            {
                itog.append("]}");
            }
            qDebug()<<"Клиент " << socketDescriptor << " запрашивает информацию об актёрах";
            socket->write(itog);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "outputActor") && (doc.object().value("params").toString() == "informationAboutFilms"))
        {
            itog = "{\"type\":\"outputActor\",\"params\":\"resultInformationAboutFilms\",\"result\":[";
            QSqlQuery* movies = new QSqlQuery(db);
            movies->prepare("SELECT * FROM MovieByActor(:actorID)");
            movies->bindValue(":actorID", doc.object().value("id").toString());
            if (movies->exec())
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
            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[]}");
            }
            else
            {
                itog.append("]}");
            }
            qDebug()<<"Клиент " << socketDescriptor << " запрашивает информацию о фильмах, связанных с актёром с id = " << doc.object().value("id").toString();
            socket->write(itog);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "outputActor") && (doc.object().value("params").toString() == "requestSizePhotoActor"))
        {
            QSqlQuery* queryActorPortrait = new QSqlQuery(db);
            if (queryActorPortrait->exec("SELECT ActorPortrait FROM Actor WHERE ActorID = " + doc.object().value("id").toString()))
            {
                queryActorPortrait->first();
                baActorPortrait = queryActorPortrait->value(0).toByteArray();

                if (baActorPortrait != "")
                {
                    socket->write("{\"type\":\"outputActor\",\"params\":\"sizePhotoActor\",\"length\":"+QByteArray::number(baActorPortrait.size())+"}");
                    socket->waitForBytesWritten(500);
                }
                else
                {
                    QString str = "NULL";
                    socket->write("{\"type\":\"outputActor\",\"params\":\"sizePhotoActor\",\"length\":"+QByteArray::number(str.size())+"}");
                    socket->waitForBytesWritten(500);
                }
            }

        }
        else if ((doc.object().value("type").toString() == "outputActor") && (doc.object().value("params").toString() == "findPhotoActor"))
        {
            if (baActorPortrait != "")
            {
                socket->write(baActorPortrait);
                socket->waitForBytesWritten(500);
            }
            else
            {
                socket->write("NULL");
                socket->waitForBytesWritten(500);
            }
        }
        else if ((doc.object().value("type").toString() == "deleteActor") && (doc.object().value("params").toString() == "sendID"))
        {
            if (checkActorID(doc.object().value("id").toString()))
            {
                QSqlQuery* query = new QSqlQuery(db);
                query->prepare("DELETE FROM Actor WHERE ActorID=:ActorID");
                query->bindValue(":ActorID", doc.object().value("id").toString());

                if(!query->exec())
                {
                    qDebug() << "Запрос на удаление актёра составлен неверно!";
                }
                else
                {
                    qDebug()<<"Клиент " << socketDescriptor << " удалил актёра с id = " << doc.object().value("id").toString();
                    socket->write("{\"type\":\"deleteActor\",\"params\":\"actorSuccessfullyDeleted\"}");
                    socket->waitForBytesWritten(500);
                }
            }
            else
            {
                qDebug()<<"Клиент " << socketDescriptor << " не смог удалить актёра с id = " << doc.object().value("id").toString();
                socket->write("{\"type\":\"deleteActor\",\"params\":\"actorUnsuccessfullyDeleted\"}");
                socket->waitForBytesWritten(500);
            }
        }
        else if ((doc.object().value("type").toString() == "outputProtagonist") && (doc.object().value("params").toString() == "findAllProtagonist"))
        {
            itog = "{\"type\":\"outputProtagonist\",\"params\":\"resultAllProtagonist\",\"result\":[";

            QSqlQuery* protagonist = new QSqlQuery(db);
            if (protagonist->exec("SELECT ProtagonistID, Name, ActorID FROM Protagonist"))
            {
                while (protagonist->next())
                {

                    itog.append("{\"protagonistID\":\""+protagonist->value(0).toString()+
                                "\",\"name\":\""+protagonist->value(1).toString()+
                                "\",\"actorID\":\""+protagonist->value(2).toString()+
                                "\"},");
                }
                itog.remove(itog.length()-1,1);
            }

            delete protagonist;
            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[],\"resultView\":[");
            }
            else
            {
                itog.append("],\"resultView\":[");
            }

            QSqlQuery* view = new QSqlQuery(db);
            if (view->exec("SELECT * FROM ProtagonistAndActor"))
            {
                while (view->next())
                {

                    itog.append("{\"nameProtagonist\":\""+view->value(0).toString()+
                                "\",\"firstName\":\""+view->value(1).toString()+
                                "\",\"lastName\":\""+view->value(2).toString()+
                                "\"},");
                }
                itog.remove(itog.length()-1,1);
            }

            delete view;
            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[]}");
            }
            else
            {
                itog.append("]}");
            }
            socket->write(itog);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "deleteProtagonist") && (doc.object().value("params").toString() == "sendID"))
        {
            if (checkProtagonistID(doc.object().value("id").toString()))
            {
                QSqlQuery* query = new QSqlQuery(db);
                query->prepare("DELETE FROM Protagonist WHERE ProtagonistID=:ProtagonistID");
                query->bindValue(":ProtagonistID", doc.object().value("id").toString());

                if(!query->exec())
                {
                    qDebug() << "Запрос на удаление главного героя составлен неверно!";
                }
                else
                {
                    qDebug()<<"Клиент " << socketDescriptor << " удалил главного героя с id = " << doc.object().value("id").toString();
                    socket->write("{\"type\":\"deleteProtagonist\",\"params\":\"protagonistSuccessfullyDeleted\"}");
                    socket->waitForBytesWritten(500);
                }
            }
            else
            {
                qDebug()<<"Клиент " << socketDescriptor << " не смог удалить главного героя с id = " << doc.object().value("id").toString();
                socket->write("{\"type\":\"deleteProtagonist\",\"params\":\"protagonistUnsuccessfullyDeleted\"}");
                socket->waitForBytesWritten(500);
            }
        }
        else if ((doc.object().value("type").toString() == "storedProcedures") && (doc.object().value("params").toString() == "findStoredProcedures"))
        {
            itog = "{\"type\":\"storedProcedures\",\"params\":\"resultStoredProcedures\",\"resultProcedures1\":[";
            QSqlQuery* query1 = new QSqlQuery(db);
            if (query1->exec("EXEC MostProductiveYear"))
            {
                while (query1->next())
                {
                    itog.append("{\"releaseDate\":\""+query1->value(0).toString()+
                                "\"},");
                }
                itog.remove(itog.length()-1,1);
            }

            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[],\"resultProcedures2\":[");
            }
            else
            {
                itog.append("],\"resultProcedures2\":[");
            }

            QSqlQuery* query2 = new QSqlQuery(db);
            if (query2->exec("EXEC InformationAboutMovies"))
            {
                query2->first();
                itog.append("{\"averageBudget\":\""+query2->value(0).toString()+
                            "\",\"averageBoxOffice\":\""+query2->value(1).toString()+
                            "\"}");
            }
            itog.append("]}");

            qDebug()<<"Клиент " << socketDescriptor << " воспользовался процедурами";
            socket->write(itog);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "report") && (doc.object().value("params").toString() == "requestReport"))
        {
            QString str;
            str.append("<html><head></head><body><center>"+QString("Таблица фильмов"));
            str.append("<table border=1><tr>");
            str.append("<th>"+QString("Номер фильма")+"</th>");
            str.append("<th>"+QString("Название фильма")+"</th>");
            str.append("<th>"+QString("Дата выхода")+"</th>");
            str.append("<th>"+QString("Кассовые сборы")+"</th>");
            str.append("<th>"+QString("Бюджет")+"</th>");
            str.append("<th>"+QString("Номер главного героя")+"</th>");
            str.append("<th>"+QString("Номер режиссёра")+"</th>");
            str.append("<th>"+QString("Название киностудии")+"</th></tr>");

            QSqlQuery* queryMovies = new QSqlQuery(db);
            queryMovies->exec("SELECT MovieID, Title, ReleaseDate, BoxOffice, Budget, ProtagonistID, DirectorID, StudioName FROM Movie");

            while(queryMovies->next())
            {
                str.append("<tr><td>");
                str.append(queryMovies->value(0).toString());
                str.append("</td><td>");
                str.append(queryMovies->value(1).toString());
                str.append("</td><td>");
                str.append(queryMovies->value(2).toString());
                str.append("</td><td>");
                str.append(queryMovies->value(3).toString());
                str.append("</td><td>");
                str.append(queryMovies->value(4).toString());
                str.append("</td><td>");
                str.append(queryMovies->value(5).toString());
                str.append("</td><td>");
                str.append(queryMovies->value(6).toString());
                str.append("</td><td>");
                str.append(queryMovies->value(7).toString());
                str.append("</td></tr>");
            }


            str.append("</table></center><p><br></p><center>"+QString("Таблица режиссёров"));
            str.append("<table border=1><tr>");
            str.append("<th>"+QString("Номер режиссёра")+"</th>");
            str.append("<th>"+QString("Имя")+"</th>");
            str.append("<th>"+QString("Фамилия")+"</th>");
            str.append("<th>"+QString("Дата рождения")+"</th></tr>");

            QSqlQuery* queryDirector = new QSqlQuery(db);
            queryDirector->exec("SELECT * FROM Director");

            while(queryDirector->next())
            {
                str.append("<tr><td>");
                str.append(queryDirector->value(0).toString());
                str.append("</td><td>");
                str.append(queryDirector->value(1).toString());
                str.append("</td><td>");
                str.append(queryDirector->value(2).toString());
                str.append("</td><td>");
                str.append(queryDirector->value(3).toString());
                str.append("</td></tr>");
            }

            str.append("</table></center><p><br></p><center>"+QString("Таблица актёров"));
            str.append("<table border=1><tr>");
            str.append("<th>"+QString("Номер актёра")+"</th>");
            str.append("<th>"+QString("Имя")+"</th>");
            str.append("<th>"+QString("Фамилия")+"</th>");
            str.append("<th>"+QString("Дата рождения")+"</th></tr>");

            QSqlQuery* queryActor = new QSqlQuery(db);
            queryActor->exec("SELECT ActorID, FirstName, LastName, DateOfBirth FROM Actor");

            while(queryActor->next())
            {
                str.append("<tr><td>");
                str.append(queryActor->value(0).toString());
                str.append("</td><td>");
                str.append(queryActor->value(1).toString());
                str.append("</td><td>");
                str.append(queryActor->value(2).toString());
                str.append("</td><td>");
                str.append(queryActor->value(3).toString());
                str.append("</td></tr>");
            }

            str.append("</table></center><p><br></p><center>"+QString("Таблица главных героев"));
            str.append("<table border=1><tr>");
            str.append("<th>"+QString("Номер главного героя")+"</th>");
            str.append("<th>"+QString("Имя")+"</th>");
            str.append("<th>"+QString("номер актёра")+"</th></tr>");

            QSqlQuery* queryProtagonist = new QSqlQuery(db);
            queryProtagonist->exec("SELECT * FROM Protagonist");

            while(queryProtagonist->next())
            {
                str.append("<tr><td>");
                str.append(queryProtagonist->value(0).toString());
                str.append("</td><td>");
                str.append(queryProtagonist->value(1).toString());
                str.append("</td><td>");
                str.append(queryProtagonist->value(2).toString());
                str.append("</td></tr>");
            }

            str.append("</table></center><p><br></p><center>"+QString("Таблица киностудий"));
            str.append("<table border=1><tr>");
            str.append("<th>"+QString("Название киностудии")+"</th></tr>");

            QSqlQuery* queryStudio = new QSqlQuery(db);
            queryStudio->exec("SELECT * FROM Studio");

            while(queryStudio->next())
            {
                str.append("<tr><td>");
                str.append(queryStudio->value(0).toString());
                str.append("</td></tr>");
            }

            str.append("</table></center></body></html>");

            /*QAxObject* word = new QAxObject("Word.Application",this);
            word->setProperty("DisplayAlert", false);
            word->setProperty("Visible",true);
            QAxObject* doc = word->querySubObject("Documents");
            doc->dynamicCall("Open(QVariant)",file);*/

            itog = "{\"type\":\"report\",\"params\":\"PDFreport\",\"report\":\""+str.toUtf8()+"\"}";
            qDebug() << itog;
            socket->write(itog);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "report") && (doc.object().value("params").toString() == "requestExcelReport"))
        {
            itog = "{\"type\":\"report\",\"params\":\"itogExcel\",\"result\":[";

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

            if (itog.lastIndexOf(":") == (itog.length()-1))
            {
                itog.append("[]}");
            }
            else
            {
                itog.append("]}");
            }

            socket->write(itog);
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
                QSqlQuery* query = new QSqlQuery(db);
                if (newReleaseDate == "")
                {
                    query->prepare("INSERT INTO Movie (StudioName, DirectorID, ProtagonistID, Title, ReleaseDate, BoxOffice, Budget, Poster, Scenario) "
                                   "VALUES (:StudioName, :DirectorID, :ProtagonistID, :Title, NULL, :BoxOffice, :Budget, :Poster, :Scenario)");
                }
                else
                {
                    query->prepare("INSERT INTO Movie (StudioName, DirectorID, ProtagonistID, Title, ReleaseDate, BoxOffice, Budget, Poster, Scenario) "
                                   "VALUES (:StudioName, :DirectorID, :ProtagonistID, :Title, :ReleaseDate, :BoxOffice, :Budget, :Poster, :Scenario)");
                }

                if (newStudioName == "")
                {
                    query->bindValue(":StudioName", QVariant (QVariant :: String));
                }
                else
                {
                    query->bindValue(":StudioName", newStudioName);
                }
                if (newDirectorID == "")
                {
                    query->bindValue(":DirectorID", QVariant (QVariant :: String));
                }
                else
                {
                    query->bindValue(":DirectorID", newDirectorID);
                }
                if (newProtagonistID == "")
                {
                    query->bindValue(":ProtagonistID", QVariant (QVariant :: String));
                }
                else
                {
                    query->bindValue(":ProtagonistID", newProtagonistID);
                }
                if (newTitle != "")
                {
                    query->bindValue(":Title", newTitle);
                }
                else
                {
                    query->bindValue(":Title", QVariant (QVariant :: String));
                }
                if (newReleaseDate != "")
                {
                    query->bindValue(":ReleaseDate", newReleaseDate);
                }
                if (newBoxOffice == "")
                {
                    query->bindValue(":BoxOffice", QVariant (QVariant :: Int));
                }
                else
                {
                    query->bindValue(":BoxOffice", newBoxOffice);
                }
                if (newBudget == "")
                {
                    query->bindValue(":Budget", QVariant (QVariant :: Int));
                }
                else
                {
                    query->bindValue(":Budget", newBudget);
                }
                if (newPoster != "NULL")
                {
                    query->bindValue(":Poster", newPoster);
                }
                else
                {
                    query->bindValue(":Poster", QVariant (QVariant :: ByteArray));
                }
                if (Data != "NULL")
                {
                    query->bindValue(":Scenario", Data);
                }
                else
                {
                    query->bindValue(":Scenario", QVariant (QVariant :: ByteArray));
                }

                if(!query->exec())
                {
                    qDebug() << "Запрос на добавление фильма составлен неверно!";
                }
                else
                {
                    qDebug()<<"Клиент " << socketDescriptor << " добавил новый фильм";
                    socket->write("{\"type\":\"addNewMovie\",\"params\":\"movieAddedSuccessfully\"}");
                    socket->waitForBytesWritten(500);
                }

                QSqlQuery* endTransact = new QSqlQuery(db);
                endTransact->prepare("COMMIT TRANSACTION");
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
                QSqlQuery* query = new QSqlQuery(db);
                if (newDateOfBirthActor == "")
                {
                    query->prepare("INSERT INTO Actor (FirstName, LastName, DateOfBirth, ActorPortrait) "
                                   "VALUES (:FirstName, :LastName, NULL, :ActorPortrait)");
                }
                else
                {
                    query->prepare("INSERT INTO Actor (FirstName, LastName, DateOfBirth, ActorPortrait) "
                                   "VALUES (:FirstName, :LastName, :DateOfBirth, :ActorPortrait)");
                }
                if (newFirstNameActor != "")
                {
                    query->bindValue(":FirstName", newFirstNameActor);
                }
                else
                {
                    query->bindValue(":FirstName", QVariant (QVariant :: String));
                }
                if (newLastNameActor != "")
                {
                    query->bindValue(":LastName", newLastNameActor);
                }
                else
                {
                    query->bindValue(":LastName", QVariant (QVariant :: String));
                }
                if (newDateOfBirthActor != "")
                {
                    query->bindValue(":DateOfBirth", newDateOfBirthActor);
                }
                if (Data != "NULL")
                {
                    query->bindValue(":ActorPortrait", Data);
                }
                else
                {
                    query->bindValue(":ActorPortrait", QVariant (QVariant :: ByteArray));
                }

                if(!query->exec())
                {
                    qDebug() << "Запрос на добавление актёра составлен неверно!";
                }
                else
                {
                    qDebug()<<"Клиент " << socketDescriptor << " добавил нового актёра";
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
            //qDebug() << newPoster;

            if (!posterAndScenario)
            {
                if (checkInformationOnFilm(oldMovieID, oldTitle,
                                           oldReleaseDate, oldBudget, oldBoxOffice))
                {
                    QSqlQuery* query = new QSqlQuery(db);
                    if (newReleaseDate == "")
                    {
                        query->prepare("UPDATE Movie SET Title=:Title, ReleaseDate=NULL, BoxOffice=:BoxOffice, Budget=:Budget, ProtagonistID=:ProtagonistID, DirectorID=:DirectorID, StudioName=:StudioName, Poster=:Poster WHERE MovieID=:oldMovieID");
                    }
                    else
                    {
                        query->prepare("UPDATE Movie SET Title=:Title, ReleaseDate=:ReleaseDate, BoxOffice=:BoxOffice, Budget=:Budget, ProtagonistID=:ProtagonistID, DirectorID=:DirectorID, StudioName=:StudioName, Poster=:Poster WHERE MovieID=:oldMovieID");
                    }
                    if (newTitle != "")
                    {
                        query->bindValue(":Title", newTitle);
                    }
                    else
                    {
                        query->bindValue(":Title", QVariant (QVariant :: String));
                    }
                    if (newReleaseDate != "")
                    {
                        query->bindValue(":ReleaseDate", newReleaseDate);
                    }
                    if (newBoxOffice == "")
                    {
                        query->bindValue(":BoxOffice", QVariant (QVariant :: Int));
                    }
                    else
                    {
                        query->bindValue(":BoxOffice", newBoxOffice);
                    }
                    if (newBudget == "")
                    {
                        query->bindValue(":Budget", QVariant (QVariant :: Int));
                    }
                    else
                    {
                        query->bindValue(":Budget", newBudget);
                    }
                    if (newProtagonistID == "")
                    {
                        query->bindValue(":ProtagonistID", QVariant (QVariant :: String));
                    }
                    else
                    {
                        query->bindValue(":ProtagonistID", newProtagonistID);
                    }
                    if (newDirectorID == "")
                    {
                        query->bindValue(":DirectorID", QVariant (QVariant :: String));
                    }
                    else
                    {
                        query->bindValue(":DirectorID", newDirectorID);
                    }
                    if (newStudioName == "")
                    {
                        query->bindValue(":StudioName", QVariant (QVariant :: String));
                    }
                    else
                    {
                        query->bindValue(":StudioName", newStudioName);
                    }
                    if (newPoster != "NULL")
                    {
                        query->bindValue(":Poster", newPoster);
                    }
                    else
                    {
                        query->bindValue(":Poster", QVariant (QVariant :: ByteArray));
                    }
                    query->bindValue(":oldMovieID", oldMovieID);

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

                    if(!db.commit())
                    {
                        socket->write("{\"type\":\"updateMovie\",\"params\":\"movieUnsuccessfullyUpdated\"}");
                        socket->waitForBytesWritten(500);
                    }
                }
                else
                {
                    socket->write("{\"type\":\"updateMovie\",\"params\":\"movieUnsuccessfullyUpdated\"}");
                    socket->waitForBytesWritten(500);
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
                if (checkInformationOnFilm(oldMovieID, oldTitle,
                                           oldReleaseDate, oldBudget, oldBoxOffice))
                {
                    QSqlQuery* query = new QSqlQuery(db);
                    if (newReleaseDate == "")
                    {
                        query->prepare("UPDATE Movie SET Title=:Title, ReleaseDate=NULL, BoxOffice=:BoxOffice, Budget=:Budget, ProtagonistID=:ProtagonistID, DirectorID=:DirectorID, StudioName=:StudioName, Scenario=:Scenario WHERE MovieID=:oldMovieID");
                    }
                    else
                    {
                        query->prepare("UPDATE Movie SET Title=:Title, ReleaseDate=:ReleaseDate, BoxOffice=:BoxOffice, Budget=:Budget, ProtagonistID=:ProtagonistID, DirectorID=:DirectorID, StudioName=:StudioName, Scenario=:Scenario WHERE MovieID=:oldMovieID");
                    }
                    if (newTitle != "")
                    {
                        query->bindValue(":Title", newTitle);
                    }
                    else
                    {
                        query->bindValue(":Title", QVariant (QVariant :: String));
                    }
                    if (newReleaseDate != "")
                    {
                        query->bindValue(":ReleaseDate", newReleaseDate);
                    }
                    if (newBoxOffice == "")
                    {
                        query->bindValue(":BoxOffice", QVariant (QVariant :: Int));
                    }
                    else
                    {
                        query->bindValue(":BoxOffice", newBoxOffice);
                    }
                    if (newBudget == "")
                    {
                        query->bindValue(":Budget", QVariant (QVariant :: Int));
                    }
                    else
                    {
                        query->bindValue(":Budget", newBudget);
                    }
                    if (newProtagonistID == "")
                    {
                        query->bindValue(":ProtagonistID", QVariant (QVariant :: String));
                    }
                    else
                    {
                        query->bindValue(":ProtagonistID", newProtagonistID);
                    }
                    if (newDirectorID == "")
                    {
                        query->bindValue(":DirectorID", QVariant (QVariant :: String));
                    }
                    else
                    {
                        query->bindValue(":DirectorID", newDirectorID);
                    }
                    if (newStudioName == "")
                    {
                        query->bindValue(":StudioName", QVariant (QVariant :: String));
                    }
                    else
                    {
                        query->bindValue(":StudioName", newStudioName);
                    }
                    if (Data != "NULL")
                    {
                        query->bindValue(":Scenario", Data);
                    }
                    else
                    {
                        query->bindValue(":Scenario", QVariant (QVariant :: ByteArray));
                    }
                    query->bindValue(":oldMovieID", oldMovieID);

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

                    if(!db.commit())
                    {
                        socket->write("{\"type\":\"updateMovie\",\"params\":\"movieUnsuccessfullyUpdated\"}");
                        socket->waitForBytesWritten(500);
                    }
                }
                else
                {
                    socket->write("{\"type\":\"updateMovie\",\"params\":\"movieUnsuccessfullyUpdated\"}");
                    socket->waitForBytesWritten(500);
                }
            }
            else
            {
                posterAndScenario = false;

                QSqlQuery* query = new QSqlQuery(db);
                if (newReleaseDate == "")
                {
                    query->prepare("UPDATE Movie SET Title=:Title, ReleaseDate=NULL, BoxOffice=:BoxOffice, Budget=:Budget, ProtagonistID=:ProtagonistID, DirectorID=:DirectorID, StudioName=:StudioName, Poster=:Poster, Scenario=:Scenario WHERE MovieID=:oldMovieID");
                }
                else
                {
                    query->prepare("UPDATE Movie SET Title=:Title, ReleaseDate=:ReleaseDate, BoxOffice=:BoxOffice, Budget=:Budget, ProtagonistID=:ProtagonistID, DirectorID=:DirectorID, StudioName=:StudioName, Poster=:Poster, Scenario=:Scenario WHERE MovieID=:oldMovieID");
                }
                if (newTitle != "")
                {
                    query->bindValue(":Title", newTitle);
                }
                else
                {
                    query->bindValue(":Title", QVariant (QVariant :: String));
                }
                if (newReleaseDate != "")
                {
                    query->bindValue(":ReleaseDate", newReleaseDate);
                }
                if (newBoxOffice == "")
                {
                    query->bindValue(":BoxOffice", QVariant (QVariant :: Int));
                }
                else
                {
                    query->bindValue(":BoxOffice", newBoxOffice);
                }
                if (newBudget == "")
                {
                    query->bindValue(":Budget", QVariant (QVariant :: Int));
                }
                else
                {
                    query->bindValue(":Budget", newBudget);
                }
                if (newProtagonistID == "")
                {
                    query->bindValue(":ProtagonistID", QVariant (QVariant :: String));
                }
                else
                {
                    query->bindValue(":ProtagonistID", newProtagonistID);
                }
                if (newDirectorID == "")
                {
                    query->bindValue(":DirectorID", QVariant (QVariant :: String));
                }
                else
                {
                    query->bindValue(":DirectorID", newDirectorID);
                }
                if (newStudioName == "")
                {
                    query->bindValue(":StudioName", QVariant (QVariant :: String));
                }
                else
                {
                    query->bindValue(":StudioName", newStudioName);
                }
                if (newPoster != "NULL")
                {
                    query->bindValue(":Poster", newPoster);
                }
                else
                {
                    query->bindValue(":Poster", QVariant (QVariant :: ByteArray));
                }
                if (Data != "NULL")
                {
                    query->bindValue(":Scenario", Data);
                }
                else
                {
                    query->bindValue(":Scenario", QVariant (QVariant :: ByteArray));
                }
                query->bindValue(":oldMovieID", oldMovieID);

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

                if(!db.commit())
                {
                    socket->write("{\"type\":\"updateMovie\",\"params\":\"movieUnsuccessfullyUpdated\"}");
                    socket->waitForBytesWritten(500);
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
    else if (newActorPortraitArrives)
    {
        //qDebug() << "Сколько пришло: " << Data.size() << ", сколько должно быть: " << requireSize;
        if (requireSize == Data.size())
        {
            QSqlQuery* query = new QSqlQuery(db);
            if (newDateOfBirth == "")
            {
                query->prepare("UPDATE Actor SET FirstName=:FirstName, LastName=:LastName, DateOfBirth=NULL, ActorPortrait=:ActorPortrait WHERE ActorID=:oldActorID");
            }
            else
            {
                query->prepare("UPDATE Actor SET FirstName=:FirstName, LastName=:LastName, DateOfBirth=:DateOfBirth, ActorPortrait=:ActorPortrait WHERE ActorID=:oldActorID");
            }

            if (newFirstName != "")
            {
                query->bindValue(":FirstName", newFirstName);
            }
            else
            {
                query->bindValue(":FirstName", QVariant (QVariant :: String));
            }
            if (newLastName != "")
            {
                query->bindValue(":LastName", newLastName);
            }
            else
            {
                query->bindValue(":LastName", QVariant (QVariant :: String));
            }
            if (newDateOfBirth != "")
            {
                query->bindValue(":DateOfBirth", newDateOfBirth);
            }
            if (Data != "NULL")
            {
                query->bindValue(":ActorPortrait", Data);
            }
            else
            {
                query->bindValue(":ActorPortrait", QVariant (QVariant :: ByteArray));
            }
            query->bindValue(":oldActorID", oldActorID);

            if(!query->exec())
            {
                qDebug() << "Запрос на обновление фильма составлен неверно!";
            }
            else
            {
                qDebug()<<"Клиент " << socketDescriptor << " обновил актёра с id = " << oldActorID;
                socket->write("{\"type\":\"updateActor\",\"params\":\"actorSuccessfullyUpdated\"}");
                socket->waitForBytesWritten(500);
            }

            newActorPortraitArrives = false;
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


