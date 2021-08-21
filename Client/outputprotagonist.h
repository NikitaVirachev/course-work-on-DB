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
    QStandardItemModel* protagonistAndActor = new QStandardItemModel(nullptr);

    bool flagContextMenu;
    QString globalID;

public slots:
    void acceptProtagonist(QJsonArray, QJsonArray);
    void customMenuReq(QPoint);
    void deleteProtagonist();

signals:
    void sendDeleteProtagonistSignal(QString);

private:
    Ui::outputProtagonist *ui;
};

#endif // OUTPUTPROTAGONIST_H
