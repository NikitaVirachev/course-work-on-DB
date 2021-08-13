#ifndef UPDATESTUDIO_H
#define UPDATESTUDIO_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class updateStudio;
}

class updateStudio : public QWidget
{
    Q_OBJECT

public:
    explicit updateStudio(QWidget *parent = nullptr);
    ~updateStudio();

    QList<QString> localListStudioName;

public slots:
    void acceptStudioName(QList <QString>);

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

signals:
    void sendUpdateStudio(QString, QString);

private:
    Ui::updateStudio *ui;
};

#endif // UPDATESTUDIO_H
