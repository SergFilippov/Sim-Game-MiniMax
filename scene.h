#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QPointF>
#include <QVector>
#include "game.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene(Game *game);
    ~Scene();
    void reset();
    void undoMove();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);

private:    
    void makeMove();
    void drawLine(int startIndex, int endIndex, Qt::GlobalColor color, int width = 4);
    void reDrawLines();
    void drawPoints();
    void computePointsCoords();
    void drawTriangle();

    Game *game_;
    QVector<QPoint> pointsCoords_;
    QPointF lineStartPosition_;
    QPointF lineFinishPosition_;
    QGraphicsLineItem *line_;
    QVector<QGraphicsEllipseItem *> points_;
    QVector<QGraphicsLineItem *> lines_;
};

#endif // SCENE_H
