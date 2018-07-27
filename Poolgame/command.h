#ifndef COMMAND_H
#define COMMAND_H
#include "game.h"

class Command {
public:
    virtual void execute() = 0;
};

class AddBallCommand : public Command {
public:
    AddBallCommand(Game* game) : m_game(game){}

    //call the game to add a ball
    void execute() override{m_game->addBall();}

private:
    Game* m_game;
};

class  ReplaceCueBallCommand : public Command
{
public:
     ReplaceCueBallCommand(Game *game): m_game(game) {}
     //call the game to replace cue ball
     void execute() override{m_game->replaceCueBallCommand();}
private:
     Game* m_game;
};

#endif // COMMAND_H
