#include "game.h"
#include <QDebug>

Game::Game()
{
    isActive_ = false;
}

void Game::startNew(int numberOfPoints, Enemy enemy, int AiLevel)
{
    numberOfPoints_ = numberOfPoints;
    enemy_ = enemy;
    AiLevel_ = AiLevel;

    gameState_.clear();
    moves_.clear();

    activePlayer_ = P_PLAYER1;
    isActive_ = true;


    for (int i = 0; i < numberOfPoints_; i++)
        for (int j = i + 1; j < numberOfPoints_; j++)
            gameState_[Line(i, j)] = P_NONE;
}


void Game::makeMove(int start, int end)
{
    Line line(start, end);
    gameState_[line] = activePlayer_;
    moves_.push_back(line);
    changeActivePlayer();

    if (isOver())
    {
        isActive_ = false;
    }
}

Game::Player Game::getActivePlayer() const
{
    return activePlayer_;
}

bool Game::isActive() const
{
    return isActive_;
}

void Game::changeActivePlayer()
{
    if (activePlayer_ == P_NONE)
        return;
    activePlayer_ = (activePlayer_ == P_PLAYER1) ? P_PLAYER2 : P_PLAYER1;
}

// Возвращает эвристическую оценку состояния игры
int Game::getEvaluation()
{
    int Player1possibleMoves = 0;

    if (isOver())
        switch (getWinner()) {
        case P_PLAYER2:
            return 100;
            break;
        case P_PLAYER1:
            return -100;
            break;
        default:
            return -50;
            break;
        }

    for (int i = 0; i < numberOfPoints_; i++)
        for (int j = i + 1; j < numberOfPoints_; j++)
            if (gameState_[Line(i, j)] == P_NONE)
            {
                bool canPlayer1Move = true;
                for (int k = 0; k < numberOfPoints_; k++)
                    if (k != i && k != j)
                       if (gameState_[(i < k) ? Line(i, k) : Line(k, i)] == gameState_[(j < k) ? Line(j, k) : Line(k, j)])
                       {
                            if (gameState_[(i < k) ? Line(i, k) : Line(k, i)] == P_PLAYER1)
                                canPlayer1Move = false;
                       }

            if (canPlayer1Move)
                Player1possibleMoves++;
            }
    return -Player1possibleMoves;
}

bool Game::isOver() const
{
    // Если некуда ходить
    if (!(gameState_.values().contains(P_NONE)))
        return true;

    Player curState = P_NONE;

    for (int i = 0; i < numberOfPoints_; i++)
        for (int j = i + 1; j < numberOfPoints_; j++)
            for (int k = j + 1; k < numberOfPoints_; k++)
            {
                curState = gameState_[Line(i, j)];
                if (curState == P_NONE)
                    continue;
                // Если образовался треугольник одного цвета
                if ((gameState_[Line(i, k)] == curState) && (gameState_[Line(j, k)] == curState))
                    return true;
           }
    return false;
}

Game::Player Game::getWinner() const
{
    Player curState = P_NONE;

    for (int i = 0; i < numberOfPoints_; i++)
        for (int j = i + 1; j < numberOfPoints_; j++)
            for (int k = j + 1; k < numberOfPoints_; k++)
            {
                curState = gameState_[Line(i, j)];
                if (curState == P_NONE)
                    continue;

                if ((gameState_[Line(i, k)] == curState) && (gameState_[Line(j, k)] == curState))
                {
                    return (curState == P_PLAYER1) ? P_PLAYER2 : P_PLAYER1;
                }
           }

    return P_NONE;
}


void Game::makeAiMove()
{
    runMinMax(0, activePlayer_, -INFINITY_VAL, INFINITY_VAL);
}


int Game::runMinMax(int deph, Player player, int alpha, int beta)
{
    // Если достигли нижнего уровня, получаем оценку
    if (deph >= 2 * AiLevel_)
        return getEvaluation();

    // Если больше некуда ходить, получаем оценку
    if (isOver())
        return getEvaluation();

    int testVal = NOT_INITIALIZATE;
    Line bestMove = Line(0, 0);
    int MinMax = (player == P_PLAYER2) ? -INFINITY_VAL : INFINITY_VAL;

    // Просматриваем все возможные ходы
    for (int i = 0; i < numberOfPoints_; i++)
        for (int j = i + 1; j < numberOfPoints_; j++)
        {
            if (gameState_[Line(i, j)] == P_NONE)
            {
                makeMove(i, j);
                testVal = runMinMax(deph + 1, (player == P_PLAYER1) ? P_PLAYER2 : P_PLAYER1, alpha, beta);
                undoMove();

            // Для компьютера выбираем максимальную оценку, для игрока минимальную
            if ((testVal > MinMax && player == P_PLAYER2) || (testVal <= MinMax && player == P_PLAYER1) || (bestMove == Line(0, 0)))
            {
                MinMax = testVal;
                bestMove = Line(i, j);
            }

            if (activePlayer_ == P_PLAYER2)
                alpha = qMax(alpha, testVal);
            else
                beta = qMin(beta, testVal);

            if (alpha > beta)
               break;
            }
        }

    // Если вернулись в корень, делаем ход
    if (deph == 0 && bestMove != Line(0, 0))
     {
        makeMove(bestMove.first, bestMove.second);
     }

     return MinMax;
}

Game::Player Game::getLineType(int start, int end)
{
    return gameState_[Line(start, end)];
}

int Game::getNumberOfPoints()
{
    return numberOfPoints_;
}

void Game::undoMove()
{
    if (moves_.isEmpty())
        return;

    gameState_[moves_.last()] = P_NONE;
    moves_.pop_back();
    changeActivePlayer();

    isActive_ = true;
}

Game::Enemy Game::getEnemy()
{
    return enemy_;
}

// Возвращает массив точек проигравшей комбинации
QVector<int> Game::getTriangleVertices() const
{
    QVector<int> triangleVertices;

    Player curState = P_NONE;

    for (int i = 0; i < numberOfPoints_; i++)
        for (int j = i + 1; j < numberOfPoints_; j++)
            for (int k = j + 1; k < numberOfPoints_; k++)
            {
                curState = gameState_[Line(i, j)];
                if (curState == P_NONE)
                    continue;

                if ((gameState_[Line(i, k)] == curState) && (gameState_[Line(j, k)] == curState))
                {
                    triangleVertices.push_back(i);
                    triangleVertices.push_back(j);
                    triangleVertices.push_back(k);
                }
           }
    return triangleVertices;
}

Game::~Game()
{

}
