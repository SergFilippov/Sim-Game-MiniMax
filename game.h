#ifndef GAME_H
#define GAME_H

#include <QPair>
#include <QMap>
#include <QStack>
#include <QVector>


typedef QPair<int, int> Line;

class Game
{
public:
    enum Player {
        P_NONE = 0,
        P_PLAYER1,
        P_PLAYER2
    };

    enum Enemy {
        EN_HUMAN,
        EN_COMPUTER
    };

    Game();
    ~Game();

    void startNew(int numberOfPoints, Enemy enemy, int AiLevel);
    void makeMove(int start, int end);
    Player getActivePlayer() const;
    bool isOver() const;
    bool isActive() const;
    Player getWinner() const;
    Player getLineType(int start, int  end);
    int getNumberOfPoints();
    void undoMove();
    Enemy getEnemy();
    void makeAiMove();
    QVector<int> getTriangleVertices() const;

private:
    static const int INFINITY_VAL = 16000;
    static const int NOT_INITIALIZATE = -100000;

    void changeActivePlayer();
    int getEvaluation();
    int runMinMax(int deph, Game::Player player, int alpha, int beta);

    int AiLevel_;
    int numberOfPoints_;
    bool isActive_;
    Player activePlayer_;
    Enemy enemy_;

    QMap<Line, Player> gameState_;
    QStack<Line> moves_;
};

#endif // GAME_H
