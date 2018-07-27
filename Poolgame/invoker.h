#ifndef INVOKER_H
#define INVOKER_H
#include "command.h"
class Invoker {
public:
    void setAddBallCommand(Command* addBallComand){m_addBallCommand = addBallComand;}
    void setReplaceCueBallCommand(Command* replaceCueBallCommand){m_ReplaceCueBallCommand = replaceCueBallCommand;}

    void addBall(){m_addBallCommand->execute();}
    void replaceCueBall(){m_ReplaceCueBallCommand->execute();}


private:
    Command* m_addBallCommand;
    Command* m_ReplaceCueBallCommand;
};

#endif // INVOKER_H
