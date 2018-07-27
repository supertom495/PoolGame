#include "table.h"
#include "ball.h"
#include <iostream>

void StageOneTable::render(QPainter &painter, const QVector2D& offset) {
    // our table colour
    painter.setBrush(m_brush);
    // draw table
    painter.drawRect(offset.x(), offset.y(), this->getWidth(), this->getHeight());
}
Table* StageOneTable::copy(){
    return new StageOneTable(*this);
}

void StageTwoTable::render(QPainter &painter, const QVector2D& offset) {
    // our table colour
    painter.setBrush(m_brush);
    // draw table
    painter.drawRect(offset.x() , offset.y(), this->getWidth(), this->getHeight());

    // render the pockets relative to this table
    for (Pocket* p : m_pockets) {
        p->render(painter, offset);
    }
}

StageTwoTable::~StageTwoTable() {
    for (Pocket* p : m_pockets) delete p;
}

bool StageTwoTable::sinks(Ball *b) {
    QVector2D absPos = b->getPosition();
    double radius = b->getRadius();
    // check whether any pockets consumes this ball
    for (Pocket* p : m_pockets) {
        // you sunk my scrabbleship
        if (p->contains(absPos, radius)) {
            p->incrementSunk();
            return true;
        }
    }
    return false;
}

Table* StageTwoTable::copy(){
    StageTwoTable* table = new StageTwoTable(*this);
    std::vector<Pocket*> pockets = std::vector<Pocket*>();
    for(size_t i = 0; i < m_pockets.size(); i++){
        pockets.push_back(m_pockets.at(i)->copy());
    }
    table->setPockets(pockets);

    return table;
}
