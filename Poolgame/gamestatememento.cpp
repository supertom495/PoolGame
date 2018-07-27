#include "gamestatememento.h"

GameStateMemento::GameStateMemento() {

}

void GameStateMemento::setBalls(std::vector<Ball *> *balls, CueBall* cueBall){
    m_balls = new std::vector<Ball*>();
    for(size_t i = 0; i < balls->size(); i++){
        m_balls->push_back(balls->at(i)->copy());
        if(balls->at(i) == cueBall){
            m_cueBall = (CueBall*)m_balls->at(i);
        }
    }
}

std::vector<Ball*>* GameStateMemento::getBalls() const{
    return m_balls;
}

void GameStateMemento::setTable(Table* table){
    m_table = table->copy();
}

Table* GameStateMemento::getTable() const{
    return m_table;
}

CueBall* GameStateMemento::getCueBall() const{
    return m_cueBall;
}

