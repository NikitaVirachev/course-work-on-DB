#ifndef PAINT_H
#define PAINT_H

#include <QWidget>
#include <QResizeEvent>
#include <QMenuBar>


#include <paintscene.h>

namespace Ui {
class Paint;
}

class Paint : public QWidget
{
    Q_OBJECT

public:
    explicit Paint(QWidget *parent = 0);
    ~Paint();

private:
    Ui::Paint *ui;
    paintScene *scene;  // Объявляем кастомную графическую сцену
    QPixmap image;

    // Tie user actions to functions
    void createActions();
    void createMenus();
    // Menu widgets
    QMenu *optionMenu;
    // All Actions
    QAction *saveAsActs;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *exitAct;

private:
    /* Переопределяем событие изменения размера окна
     * для пересчёта размеров графической сцены
     * */
    void resizeEvent(QResizeEvent * event);
    void showEvent(QShowEvent *);

private slots:
    // Possible events
    void save();
    void penColor();
    void penWidth();

public slots:
    void acceptImage(QPixmap);
};

#endif // PAINT_H
