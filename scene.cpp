#include "scene.h"
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QtMath>

Scene::Scene(Game *game)
    : QGraphicsScene()
{
    addPixmap(QPixmap(":/pics/background"))->setZValue(-100);
    game_ = game;
}

Scene::~Scene()
{
    while (!lines_.isEmpty())
    {
        this->removeItem(lines_.last());
        delete lines_.last();
        lines_.pop_back();
    }
    while (!points_.isEmpty())
    {
        this->removeItem(points_.last());
        delete points_.last();
        points_.pop_back();
    }
}

void Scene::reset()
{
    line_ = NULL;

    // Рассчитать координаты точек
    pointsCoords_.clear();
    computePointsCoords();

    // Нарисовать точки и линии
    drawPoints();
    reDrawLines();
}

// Отменяет последний ход
void Scene::undoMove()
{
   if (!game_->isActive())
    return;

    game_->undoMove();
    // Если игра против компьютера - отменить два хода
    if(game_->getEnemy() == Game::EN_COMPUTER)
        game_->undoMove();
    reDrawLines();
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    if (!game_->isActive())
        return;
    if (e->button() != Qt::LeftButton)
        return;

    lineStartPosition_ = e->scenePos();
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    if (!game_->isActive())
        return;

    if (e->buttons() != Qt::LeftButton)
        return;

    if (e->scenePos().x() > width() || e->scenePos().x() < 0)
        return;

    if (e->scenePos().y() > height() || e->scenePos().y() < 0)
        return;

    lineFinishPosition_ = e->scenePos();

    // Удалить предыдушую линию
    if (line_ != NULL)
    {
        removeItem(line_);
        delete line_;
    }

    QColor currentColor =
            (game_->getActivePlayer() == Game::P_PLAYER1) ? Qt::red : Qt::blue;
    // нарисовать новую линию
    line_ = addLine(lineStartPosition_.x(), lineStartPosition_.y(),
                    lineFinishPosition_.x(), lineFinishPosition_.y(), QPen(currentColor, 4));
    }

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    if (!game_->isActive())
        return;

    if (e->button() != Qt::LeftButton)
        return;

    lineFinishPosition_ = e->scenePos();

    if (line_ != NULL)
    {
            removeItem(line_);
            delete line_;
    }
    line_ = NULL;

    makeMove();
}

void Scene::makeMove()
{
    QGraphicsEllipseItem *startPoint =
            dynamic_cast <QGraphicsEllipseItem *> (itemAt(lineStartPosition_, QTransform()));
    QGraphicsEllipseItem *endPoint =
            dynamic_cast <QGraphicsEllipseItem *> (itemAt(lineFinishPosition_, QTransform()));

    if (!(startPoint && endPoint))
        return;

    int first = points_.indexOf(startPoint);
    int second = points_.indexOf(endPoint);

    if (first == second)
        return;

    if (first > second)
    {
        int temp = first;
        first = second;
        second = temp;
    }

    // Если ход возможен - сделать ход
    if (game_->getLineType(first, second) == Game::P_NONE)
      {
        game_->makeMove(first, second);
        reDrawLines();
        if (game_->getEnemy() == Game::EN_COMPUTER && game_->isActive())
        {
            game_->makeAiMove();
            reDrawLines();
        }
    // Если игра окончена - вывести результат
        if (game_->isOver())
        {
            QMessageBox msgBox;
            switch (game_->getWinner())
            {
            case Game::P_NONE:
                msgBox.setText("Ничья!");
                break;
            case Game::P_PLAYER1:
                drawTriangle();
                msgBox.setText("Победили красные!");
                break;
            case Game::P_PLAYER2:
                drawTriangle();
                msgBox.setText("Победили синие!");
                break;
            default:
                break;
            }
            msgBox.exec();
        }
    }
}


void Scene::reDrawLines()
{
    while (!lines_.isEmpty())
    {
        this->removeItem(lines_.last());
        delete lines_.last();
        lines_.pop_back();
    }

    for (int start = 0; start < game_->getNumberOfPoints(); start++)
        for (int end = start + 1; end < game_->getNumberOfPoints(); end++)
        {
            switch (game_->getLineType(start, end))
            {
            case Game::P_PLAYER1 :
                drawLine(start, end, Qt::red);
                break;
            case Game::P_PLAYER2 :
                drawLine(start, end, Qt::blue);
                break;
            default:
                break;
            }
        }
}

void Scene::drawPoints()
{
    while (!points_.isEmpty())
    {
        this->removeItem(points_.last());
        delete points_.last();
        points_.pop_back();
    }

    for (auto pointCoord : pointsCoords_)
    {
        QGraphicsEllipseItem *point =
                addEllipse(pointCoord.x(), pointCoord.y(), 20, 20, QPen(Qt::red, 2), QBrush(Qt::black));
        points_.push_back(point);
    }
}

void Scene::computePointsCoords()
{
    double ungle = 2 * M_PI / game_->getNumberOfPoints();
    for (int i = 0; i < game_->getNumberOfPoints(); i++)
    {
        QPoint point;
        point.setX(150.5 * cos(ungle * i - M_PI / 2) + 170);
        point.setY(150.5 * sin(ungle * i - M_PI / 2) + 170);
        pointsCoords_.push_back(point);
    }
}

void Scene::drawLine(int startIndex, int endIndex, Qt::GlobalColor color, int width)
{
    QPoint start(pointsCoords_[startIndex].x() + 10, pointsCoords_[startIndex].y() + 10);
    QPoint end(pointsCoords_[endIndex].x() + 10, pointsCoords_[endIndex].y() + 10);

    QGraphicsLineItem *line_ = addLine(start.x(), start.y(), end.x(), end.y(), QPen(color, width));
    line_->setZValue(-1);
    lines_.push_back(line_);
}

// Выделить проигравшую комбинацию
void Scene::drawTriangle()
{
    QVector<int> triangle = game_->getTriangleVertices();

    Game::Player winner = game_->getWinner();
    Qt::GlobalColor color = (winner == Game::P_PLAYER1) ? Qt::blue : Qt::red;

    for (int i = 0; i < 3; i++)
        for (int j = i + 1; j < 3; j++)
            drawLine(triangle[i], triangle[j], color, 7);
}
