#include "paint.h"
#include "ui_paint.h"

Paint::Paint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Paint)
{
    ui->setupUi(this);

    scene = new paintScene();       // Инициализируем графическую сцену
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену
    /*
    QMenuBar* menuBar = new QMenuBar();
    QMenu *fileMenu = new QMenu("Меню");
    menuBar->addMenu(fileMenu);
    fileMenu->addAction("Сохранить");
    fileMenu->addAction("Палитра");
    fileMenu->addAction("Толщина");
    */
    //this->layout()->setMenuBar(menuBar);
    createActions();
    createMenus();
}

Paint::~Paint()
{
    delete ui;
}

void Paint::createActions()
{
    exitAct = new QAction(tr("&Закрыть"), this);
    exitAct->setShortcut(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    penColorAct = new QAction(tr("&Цвет кисти..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    penWidthAct = new QAction(tr("&Размер кисти..."), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    saveAsActs = new QAction(tr("&Сохранить"), this);
    connect(saveAsActs, SIGNAL(triggered()), this, SLOT(save()));
}

void Paint::createMenus()
{
    optionMenu = new QMenu(tr("&Опции"), this);
    optionMenu->addAction(saveAsActs);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(exitAct);
    this->layout()->setMenuBar(optionMenu);
}

void Paint::acceptImage(QPixmap img)
{
    image = img;
    scene->addPixmap(image);
}


void Paint::resizeEvent(QResizeEvent *event)
{
    //QRectF bounds = ui->graphicsView->scene()->sceneRect();
    //ui->graphicsView->fitInView(bounds, Qt::KeepAspectRatioByExpanding);
    //ui->graphicsView->centerOn(bounds.center());

    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
}


void Paint::showEvent(QShowEvent *)
{
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
}

void Paint::save()
{

}

void Paint::penColor()
{

}

void Paint::penWidth()
{

}

