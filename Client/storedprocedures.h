#ifndef STOREDPROCEDURES_H
#define STOREDPROCEDURES_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QStandardItem>

namespace Ui {
class storedProcedures;
}

class storedProcedures : public QWidget
{
    Q_OBJECT

public:
    explicit storedProcedures(QWidget *parent = nullptr);
    ~storedProcedures();

    QStandardItemModel* storedProcedure1 = new QStandardItemModel(nullptr);
    QStandardItemModel* storedProcedure2 = new QStandardItemModel(nullptr);

public slots:
    void accept(QJsonArray, QJsonArray);

private:
    Ui::storedProcedures *ui;
};

#endif // STOREDPROCEDURES_H
