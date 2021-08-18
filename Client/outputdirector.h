#ifndef OUTPUTDIRECTOR_H
#define OUTPUTDIRECTOR_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QStandardItem>
#include <QMenu>
#include <QAction>

namespace Ui {
class outputDirector;
}

class outputDirector : public QWidget
{
    Q_OBJECT

public:
    explicit outputDirector(QWidget *parent = nullptr);
    ~outputDirector();

    QStandardItemModel* director = new QStandardItemModel(nullptr);

    bool flagContextMenu;
    QString globalID;

public slots:
    void acceptDirector(QJsonArray);
    void customMenuReq(QPoint);
    void deleteDirector();

signals:
    void sendDeleteDirectorSignal(QString);

private:
    Ui::outputDirector *ui;
};

#endif // OUTPUTDIRECTOR_H
