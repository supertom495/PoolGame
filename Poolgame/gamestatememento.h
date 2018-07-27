#ifndef GAMESTATEMEMENTO_H
#define GAMESTATEMEMENTO_H

#include "table.h"
#include "ball.h"
#include "balldecorator.h"

/**
 * @brief The GameStateMemento class - the memento owns all the variable that is in the game
 */
class GameStateMemento
{
public:
    GameStateMemento();

    void setBalls(std::vector<Ball*>* balls, CueBall* cueBall);
    std::vector<Ball*>* getBalls() const;

    void setTable(Table* table);
    Table* getTable() const;

    CueBall* getCueBall() const;


private:
    std::vector<Ball*>* m_balls;
    Table* m_table;
    CueBall* m_cueBall;
};

#endif // GAMESTATEMEMENTO_H
