#include "gamestatecaretaker.h"

GameStateCaretaker::GameStateCaretaker(std::vector<GameStateMemento *>* mementos): m_mementos(mementos){}

GameStateMemento* GameStateCaretaker::popMemento(){
    GameStateMemento* memento = m_mementos->back();
    if(m_mementos->size() > 1){
        m_mementos->pop_back();
    }
    return memento;
}

void GameStateCaretaker::addMemento(GameStateMemento* memento){
    m_mementos->push_back(memento);
}
