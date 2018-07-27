#pragma once

#include "ball.h"

/**
 * @brief The BallDecorator class
 * This acts as a Decorator. Any methods not overriden can simply forward the requests
 *  onto the Class this is decorated
 */
class BallDecorator : public Ball {
protected:
    Ball* m_subBall;
public:
    BallDecorator(Ball* b) : m_subBall(b) {}
    virtual ~BallDecorator() { delete m_subBall; }
    // mess of forwarded requests
    // is this the downside of a decorator..?
    virtual void render(QPainter &painter, const QVector2D& offset) override { m_subBall->render(painter, offset); }
    virtual void translate(QVector2D vec) override { m_subBall->translate(vec); }
    virtual QVector2D getVelocity() const override{ return m_subBall->getVelocity(); }
    virtual void setVelocity(QVector2D v) override { m_subBall->setVelocity(v); }
    virtual void changeVelocity(const QVector2D& delta) override { m_subBall->changeVelocity(delta); }
    virtual void multiplyVelocity(const QVector2D& vel) override { m_subBall->multiplyVelocity(vel); }

    virtual double getMass() const override { return m_subBall->getMass(); }
    virtual double getRadius() const override { return m_subBall->getRadius(); }
    virtual QVector2D getPosition() const override { return m_subBall->getPosition(); }
    virtual void setPosition(QVector2D p) override { m_subBall->setPosition(p); }
    virtual bool applyBreak(const QVector2D& q, std::vector<Ball*>& b) override { return m_subBall->applyBreak(q,b); }
};

/**
 * @brief The CueBall class
 * This handles some mouse interactions, and can control the position/velocity of the ball
 * The ball will only be able to be controlled if the mouse click&drag event originated at
 * the position of the cue ball.
 */
class CueBall : public BallDecorator {
protected:
    // keep track of where the mouse click started at
    QVector2D m_startMousePos;
    // and the end
    QVector2D m_endMousePos;
    // whether the drag visualiser is drawn
    bool isDrawing = false;
    //wheter the stick is hitting
    bool isHitting = false;
    //times of hiting need to be drawn
    int hitting_count = 8;
    //the new Velocity
    QVector2D resultingVel;

public:
    CueBall(Ball* b) : BallDecorator(b) {}
    ~CueBall() {}

    /**
     * @brief render - draw this ball and the drag indicator if applicable
     * @param painter - the brush to use to draw
     * @param offset - where our pos is relative to
     */
    void render(QPainter &painter, const QVector2D &offset) override;

    /**
     * @brief setStartMousePos - update where the start of the mouse drag is.
     *      Chooses not to draw IF the click is not within bounds
     * @param p - the mouse click pos, relative to the QWidget
     */
    void setStartMousePos(QVector2D p);

    /**
     * @brief setEndMousePos - update where the current end of the mouse drag is.
     *      Used when the mouse is moved, i.e. not released, but dragged.
     * @param p
     */
    void setEndMousePos(QVector2D p);

    /**
     * @brief setReleasePos - update where the end of the mouse drag is, & release the click
     *  This will update the ball velocity if drawn.
     * @param p
     */
    void setReleasePos(QVector2D p);

    /**
     * @brief setIsHitting - update where the stick is hitting the ball
     *  This will allow the time to drawing the stick.
     * @param state
     */
    void setIsHitting(bool state){isHitting = state;}

    // whether we can consider this ball as having stopped.
    inline bool isSubBallMoving() const {
        if(!m_subBall) return false;

        try {return m_subBall->getVelocity().length() > MovementEpsilon; } catch (const std::exception& e) { printf("%s", e.what());}
        //return m_subBall->getVelocity().length() > MovementEpsilon;
    }

    Ball* copy() override;
};

class BallSparkleDecorator : public BallDecorator {
protected:
    // our particle that is drawn
    struct Sparkle {
        Sparkle(QPointF pos, double opacity = 1.0)
            :pos(pos),opacity(opacity){}
        // absolute position
        QPointF pos;
        double opacity = 1.0;
        static constexpr double width = 5.0;
        static constexpr double height = 5.0;
    };

    // how fast the opacity is faded per drawn frame.
    // yes, this is frame dependent.
    static constexpr double fadeRate = 0.01;
    // our particle effects that will be drawn per frame
    std::vector<Sparkle> m_sparklePositions;
public:
    BallSparkleDecorator(Ball* b) : BallDecorator(b) {}

    /**
     * @brief render - draw the underlying ball and also the sparkles
     * @param painter - the brush to use to draw
     * @param offset - the offset that this ball is from the origin
     */
    void render(QPainter &painter, const QVector2D &offset) override;
    Ball* copy() override;

};

class BallSmashDecorator : public BallDecorator {
protected:
    struct Crumb {
        Crumb(QPointF pos,double width,double height, QVector2D dir, double opacity = 1.0)
            :pos(pos),width(width),height(height),dir(dir),opacity(opacity){}
        // absolute position (from origin)
        QPointF pos;
        // width of the particle rect
        double width = 5.0;
        // height of the particle rect
        double height = 5.0;
        // particle tween direction
        QVector2D dir;
        double opacity = 1.0;
    };
    // how often they fade per frame
    static constexpr double fadeRate = 0.01;
    // how often they move away
    static constexpr double moveRate = 0.3;
    std::vector<Crumb> m_crumbs;

    void addCrumbs(QPointF cPos) {
        size_t numAdding = rand() % 10;
        for (size_t i = 0; i < numAdding; ++i) {
            double width = (rand()%100)/20.0;
            double height = (rand()%100)/20.0;
            QVector2D dir(rand()%10-5, rand()%10-5);
            m_crumbs.push_back(Crumb(cPos, width, height, dir));
        }
    }
public:
    BallSmashDecorator(Ball* b) : BallDecorator(b) {}

    /**
     * @brief changeVelocity - set the velocity of the ball, as well as generate particles (if applicable)
     * @param delta - the change in velocity
     */
    virtual void changeVelocity(const QVector2D& delta) override;

    /**
     * @brief multiplyVelocity - mul the velocity, as well as generate particles, if direction changes.
     * @param vel
     */
    virtual void multiplyVelocity(const QVector2D& vel) override {
        m_subBall->multiplyVelocity(vel);
        if (vel.x() < 0 || vel.y() < 0) addCrumbs(m_subBall->getPosition().toPointF());
    }

    /**
     * @brief render - draw the ball, the smash particles, as well as update the particle effects positions
     *  yes. we animate in the render function! ):<
     * @param painter - the brush to use to draw
     * @param offset - the offset from the window that this ball's pos is.
     */
    virtual void render(QPainter &painter, const QVector2D &offset) override;

    Ball* copy() override;

};
