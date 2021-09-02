#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QDebug>

class paintScene : public QGraphicsScene
{

    Q_OBJECT

public:
    explicit paintScene(QObject *parent = 0);
    ~paintScene();
    QPixmap saveImage();
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }

private:
    QPointF     previousPoint;      // Координаты предыдущей точки

private:
    // Для рисования используем события мыши
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    int myPenWidth;
    QColor myPenColor;

};

#endif // PAINTSCENE_H
