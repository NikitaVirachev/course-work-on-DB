#ifndef OUTPUTPROTAGONIST_H
#define OUTPUTPROTAGONIST_H

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
class outputProtagonist;
}

class outputProtagonist : public QWidget
{
    Q_OBJECT

public:
    explicit outputProtagonist(QWidget *parent = nullptr);
    ~outputProtagonist();

    QStandardItemModel* protagonist = new QStandardItemModel(nullptr);

    bool flagContextMenu;
    QString globalID;

public slots:
    void acceptProtagonist(QJsonArray);
    //void customMenuReq(QPoint);
    //void deleteDirector();

private:
    Ui::outputProtagonist *ui;
};

#endif // OUTPUTPROTAGONIST_H
