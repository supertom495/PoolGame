#pragma once
#include <QJsonObject>

#include "abstractstagefactory.h"
#include "table.h"
#include "ball.h"
#include "balldecorator.h"
#include "gamestatecaretaker.h"

class Game {
    std::vector<Ball*>* m_balls;
    Table* m_table;
    CueBall* m_cueBall = nullptr;

    // screenshake stuff
    QVector2D m_screenshake;
    double m_shakeRadius = 0.0;
    double m_shakeAngle = 0;
    static constexpr double SCREENSHAKEDIST = 10.0;
    GameStateCaretaker* m_careTaker;
    bool m_isSave = false;

    /* increase the amount of screen shake */
    void incrementShake(double amount=SCREENSHAKEDIST) { m_shakeRadius += amount; }

    /**
     * @brief updateShake - update the screenshake radius (make it smaller)
     * @param dt - the timestep change
     */
    void updateShake(double dt);
public:
    ~Game();
    Game(std::vector<Ball*>* balls, Table* table) :
        m_balls(balls), m_table(table), m_careTaker(new GameStateCaretaker(new std::vector<GameStateMemento*>())) {}
    /**
     * @brief Draws all owned objects to the screen (balls and table)
     * @param painter - qtpainter to blit to screen with
     */
    void render(QPainter& painter) const;
    /**
     * @brief Updates the positions of all objects within, based on how much time has changed
     * @param dt - time elapsed since last frame in seconds
     */
    void animate(double dt);

    /* how large the window's width should at least be */
    int getMinimumWidth() const { return m_table->getWidth(); }
    /* how large the window's height should at least be */
    int getMinimumHeight() const { return m_table->getHeight(); }

    /**
     * @brief resolveCollision - modify the ball's velocity if it is colliding with the table
     * @param table - the table to be bounds checked
     * @param ball - the ball to move
     * @return velocity - the change of velocity that a ball underwent
     */
    QVector2D resolveCollision(const Table* table, Ball* ball);
    /**
     * @brief resolveCollision - resolve both ball's velocity whether these balls collide
     * @param ballA - first ball
     * @param ballB - second ball
     * @param pair<deltaVelocityA, deltaVelocityB> - the change of velocities for each ball
     */
    std::pair<QVector2D, QVector2D> resolveCollision(Ball* ballA, Ball* ballB);

    /**
     * @brief isColliding - returns whether two balls are touching each other
     * @param ballA
     * @param ballB
     * @return whether the two balls are touching each other
     */
    bool isColliding(const Ball* ballA, const Ball* ballB) {
        QVector2D collisionVector = ballB->getPosition() - ballA->getPosition();
        return !(collisionVector.length() > ballA->getRadius() + ballB->getRadius());
    }

    /* handle setting the current start click point of the mouse */
    void setStartMousePos(QVector2D p) {
        if (m_cueBall != nullptr) m_cueBall->setStartMousePos(p);
    }

    /* handle setting the current end drag point of the mouse */
    void setEndMousePos(QVector2D p) {
        if (m_cueBall != nullptr) m_cueBall->setEndMousePos(p);
    }

    /* handle setting the release point of the mouse */
    void setReleasePos(QVector2D p) {
        if (m_cueBall != nullptr) m_cueBall->setReleasePos(p);
    }

    /**
     * @brief setCueBall - set the provided ball to be the cueball
     * @param b - this ball has to be some kind of cueball type
     */
    void setCueBall(Ball* b) {
        CueBall* cb = dynamic_cast<CueBall*>(b);
        if (cb == nullptr) throw std::logic_error("attempted to set cue ball to non-cueball type");
        m_cueBall = cb;
    }

    CueBall* getCueBall(){return m_cueBall;}

    /**
     * @brief resetIfCueBall - handle the scenario b as its destroyed. if it is a cueball, the cueball variable is set to null
     * @param b - the ball to test whether it is a cueball or not
     */
    void resetIfCueBall(Ball* b) {
        // if this ball is a cue ball, reset it!
        CueBall* testCue = dynamic_cast<CueBall*>(b);
        //if (testCue) m_cueBall = nullptr;
    }

    GameStateMemento* saveState();
    void recoveryState(GameStateMemento* memento);

    /**
     * @brief addBall - add a new ball with random property
     */
    void addBall();
    /**
     * @brief replaceCueBallCommand - put the cue ball to the middle
     */
    void replaceCueBallCommand();
    /**
     * @brief getCaretaker - caretaker owns the memento
     * @return
     */
    GameStateCaretaker* getCaretaker() const{return m_careTaker;}
    /**
     * @brief setIsSave - if saved then dont need to save again
     * @param isSave - change the save state
     */
    void setIsSave(bool isSave){m_isSave = isSave;}
};
