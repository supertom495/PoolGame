#include "balldecorator.h"
#include <iostream>

void CueBall::render(QPainter &painter, const QVector2D &offset) {
    m_subBall->render(painter, offset);
    // stop drawing the line if we're moving at all
    if (isSubBallMoving()) isDrawing = false;
    if (isDrawing) {
        painter.setBrush(Qt::blue);

        double diameter = 2 * this->getRadius();
        QVector2D normal = (m_startMousePos - m_endMousePos).normalized();

        QVector2D startPos = ((m_startMousePos + m_endMousePos) / 2);

        int stickLength = 300;

        painter.drawEllipse((startPos + (normal * 5) - (normal * diameter)).toPointF(), 4,4);

        painter.setPen(QPen(QColor("#DEB808"), 8));

        painter.drawLine((startPos - (normal * diameter)) .toPointF(),
                         (startPos - (normal * stickLength) - (normal * diameter)).toPointF());
        //painter.drawLine(m_startMousePos.toPointF(), m_endMousePos.toPointF());
        painter.setPen(QPen());
        hitting_count = 8;
    }
    else if (isHitting) {

        if (hitting_count == 0){
            //isHitting = false;
            m_subBall->changeVelocity(resultingVel);
            //return;
        }

        if (hitting_count == -8){
            isHitting = false;
            return;
        }

        painter.setBrush(Qt::blue);

        double diameter = 2 * this->getRadius();
        QVector2D normal = (m_startMousePos - m_endMousePos).normalized();

        QVector2D startPos = ((m_startMousePos + m_endMousePos) / 2);

        QVector2D refine = (m_startMousePos - startPos)/8 * (8 - hitting_count);

        if (hitting_count < 0){
            refine = (m_startMousePos - startPos);
        }

        int stickLength = 300;

        painter.drawEllipse((startPos + (normal * 5) - (normal * diameter) + refine).toPointF(), 4,4);

        painter.setPen(QPen(QColor("#DEB808"), 8));

        painter.drawLine((startPos - (normal * diameter) + refine) .toPointF(),
                         (startPos - (normal * stickLength) - (normal * diameter) + refine).toPointF());
        //painter.drawLine(m_startMousePos.toPointF(), m_endMousePos.toPointF());
        painter.setPen(QPen());

        hitting_count--;

    }

}

void CueBall::setStartMousePos(QVector2D p) {
    // don't allow movement if moving
    if (isSubBallMoving()) {
        // can't be too sure
        isDrawing = false;
        return;
    }

    m_startMousePos = p;
    m_endMousePos = p;

    // note this treats it as absolute distance
    // check whether the click isn't on the ball
    if (m_startMousePos.distanceToPoint(m_subBall->getPosition()) > m_subBall->getRadius()) return;

    isDrawing = true;
}

void CueBall::setEndMousePos(QVector2D p) {
    if (isSubBallMoving()) {
        return;
    }
    m_endMousePos = p;
}

void CueBall::setReleasePos(QVector2D p) {
    // only draw & move if we're allowing the draw action to go ahead
    if (isDrawing) {
        // velocity is the vector that the mouse drew
        m_endMousePos = p;
        this->resultingVel = - m_endMousePos + m_startMousePos;
        isDrawing = false;
        // update ball vel
        isHitting = true;
        //m_subBall->changeVelocity(resultingVel);
    }
}

Ball* CueBall::copy(){
    CueBall* ball = new CueBall(*this);
    ball->m_subBall = m_subBall->copy();
    return ball;
}


void BallSparkleDecorator::render(QPainter &painter, const QVector2D &offset) {
    // draw the sub-ball
    m_subBall->render(painter, offset);

    // 1/10 chance to make a new sparkle (must not be moving)
    if (rand() % 10 == 0 && m_subBall->getVelocity().length() >= MovementEpsilon) {
        m_sparklePositions.push_back(Sparkle(m_subBall->getPosition().toPointF()));
    }

    // render all of our sparkles
    for (size_t i = 0; i < (size_t) m_sparklePositions.size(); ++i) {
        Sparkle& s = m_sparklePositions.at(i);
        painter.setBrush(QBrush(QColor("yellow")));
        painter.setOpacity(s.opacity);
        // 5x5 mini rect randomly oscillating
        QRectF r(offset.x() + s.pos.x() + (rand()%6)-3,
                 offset.y() + s.pos.y() + (rand()%6)-3, s.width , s.height);
        painter.drawRect(r);

        s.opacity -= fadeRate;
        // remove when non-visible
        if (s.opacity <= 0) {
            m_sparklePositions.erase(m_sparklePositions.begin() + i);
            // so we don't skip positions
            --i;
        }
        painter.setOpacity(1);
    }
}

Ball* BallSparkleDecorator::copy(){
    BallSparkleDecorator* ball = new BallSparkleDecorator(*this);
    ball->m_subBall = m_subBall->copy();
    return ball;
}

void BallSmashDecorator::changeVelocity(const QVector2D &delta) {
    // whenever our velocity changes a lot, we should add some particles
    QVector2D preVel = m_subBall->getVelocity();
    m_subBall->changeVelocity(delta);
    double lenChange = fabs((preVel - m_subBall->getVelocity()).length());
    if (lenChange > 3.0) addCrumbs(m_subBall->getPosition().toPointF());
}

void BallSmashDecorator::render(QPainter &painter, const QVector2D &offset) {
    m_subBall->render(painter, offset);

    // animating in render!? yeah, this should be refactored..?
    // perhaps this may be a good idea to mention in the report, and
    // refactor for assgn3 so that the animation functions are now member functions of balls
    // this enables the simplification of the Game::animate fn, as well as physics defining decorators

    // draw the crummies
    for (size_t i = 0; i < (size_t) m_crumbs.size(); ++i) {
        // move the crummie
        Crumb& c = m_crumbs.at(i);
        c.pos += (c.dir*moveRate).toPointF();

        painter.setBrush(QBrush(QColor("gray")));
        painter.setOpacity(c.opacity);
        // our lil crumb object
        QRectF r(offset.x() + c.pos.x(),
                 offset.y() + c.pos.y(), c.width, c.height);
        painter.drawRect(r);

        c.opacity -= fadeRate;
        // remove when non-visible particles
        if (c.opacity <= 0) {
            m_crumbs.erase(m_crumbs.begin() + i);
            // so we don't skip positions
            --i;
        }
        // reset to opaque
        painter.setOpacity(1);
    }
}

Ball* BallSmashDecorator::copy(){
    BallSmashDecorator* ball = new BallSmashDecorator(*this);
    ball->m_subBall = m_subBall->copy();
    return ball;
}
