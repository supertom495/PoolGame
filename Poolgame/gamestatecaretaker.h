#ifndef GAMESTATECARETAKER_H
#define GAMESTATECARETAKER_H

#include "gamestatememento.h"

class GameStateCaretaker
{
public:
    GameStateCaretaker(std::vector<GameStateMemento*>* mementos);
    /**
     * @brief popMemento - a memento is poped so the game can use it to revert
     * @return
     */
    GameStateMemento* popMemento();
    /**
     * @brief addMemento - the new state been adding to game
     * @param memento
     */
    void addMemento(GameStateMemento* memento);

private:
    std::vector<GameStateMemento*>* m_mementos;
};

#endif // GAMESTATECARETAKER_H
