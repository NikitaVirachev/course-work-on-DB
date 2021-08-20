#ifndef OUTPUTSTUDIO_H
#define OUTPUTSTUDIO_H

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
class outputStudio;
}

class outputStudio : public QWidget
{
    Q_OBJECT

public:
    explicit outputStudio(QWidget *parent = nullptr);
    ~outputStudio();

    QStandardItemModel* studio = new QStandardItemModel(nullptr);

    bool flagContextMenu;
    QString globalID;

public slots:
    void acceptStudio(QJsonArray);
    //void customMenuReq(QPoint);
    //void deleteStudio();

private:
    Ui::outputStudio *ui;
};

#endif // OUTPUTSTUDIO_H
