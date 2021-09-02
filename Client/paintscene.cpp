#include "paintscene.h"

paintScene::paintScene(QObject *parent) : QGraphicsScene(parent)
{
    myPenWidth = 1;
    myPenColor = Qt::black;
}

paintScene::~paintScene()
{

}

QPixmap paintScene::saveImage()
{

}

void paintScene::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void paintScene::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // При нажатии кнопки мыши отрисовываем эллипс
    addEllipse(event->scenePos().x() - 5,
               event->scenePos().y() - 5,
               myPenWidth,
               myPenWidth,
               QPen(myPenWidth),
               QBrush(myPenColor));
    // Сохраняем координаты точки нажатия
    previousPoint = event->scenePos();
}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // Отрисовываем линии с использованием предыдущей координаты
    addLine(previousPoint.x(),
            previousPoint.y(),
            event->scenePos().x(),
            event->scenePos().y(),
            QPen(myPenColor,myPenWidth,Qt::SolidLine,Qt::RoundCap));
    // Обновляем данные о предыдущей координате
    previousPoint = event->scenePos();
}
