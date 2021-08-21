#ifndef OUTPUTACTOR_H
#define OUTPUTACTOR_H

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
class outputActor;
}

class outputActor : public QWidget
{
    Q_OBJECT

public:
    explicit outputActor(QWidget *parent = nullptr);
    ~outputActor();

    QStandardItemModel* actor = new QStandardItemModel(nullptr);
    QStandardItemModel* movies = new QStandardItemModel(nullptr);

    bool flagContextMenu;
    QString globalID;

public slots:
    void acceptActor(QJsonArray);
    void acceptActorPortrait(QByteArray actorPortrait, QJsonArray docArr);
    void customMenuReq(QPoint);
    void deleteActor();

private slots:
    void on_tableView_clicked(const QModelIndex &index);

signals:
    void sendTakeData(QList <QString>, QList <QString>, QList <QString>, QList <QString>);
    void requestPhotoActor(QString actorID);
    void sendDeleteActorSignal(QString);

private:
    Ui::outputActor *ui;
};

#endif // OUTPUTACTOR_H
