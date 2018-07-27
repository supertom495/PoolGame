#include "dialog.h"
#include "ui_dialog.h"
#include <QPainter>
#include <QTimer>
#include <iostream>
#include <QMouseEvent>
#include "utils.h"
#include <cstdlib>

Dialog::Dialog(Game *game, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    m_game(game)
{
    ui->setupUi(this);

    // for animating (i.e. movement, collision) every animFrameMS
    aTimer = new QTimer(this);
    connect(aTimer, SIGNAL(timeout()), this, SLOT(nextAnim()));
    aTimer->start(animFrameMS);

    // for drawing every drawFrameMS milliseconds
    dTimer = new QTimer(this);
    connect(dTimer, SIGNAL(timeout()), this, SLOT(tryRender()));
    dTimer->start(drawFrameMS);


    m_addButton = new QPushButton("Add Ball", this);
    m_replaceButton = new QPushButton("Replace Cue Ball", this);

    //m_button->setGeometry(QRect(QPoint(100, 100),QSize(60, 60)));
    m_addButton->setStyleSheet("background-color:#9dce2c;"
                             "border-radius:7px;"
                             "border:1px solid #83c41a;"
                             "color:#ffffff;"
                             "font-size:15px;"
                             "font-weight:bold;"
                             "padding:4px 24px;"
                             "text-decoration:none;");
    m_addButton->move(0, game->getMinimumHeight());

    m_replaceButton->setStyleSheet("background-color:#9dce2c;"
                             "border-radius:7px;"
                             "border:1px solid #83c41a;"
                             "color:#ffffff;"
                             "font-size:15px;"
                             "font-weight:bold;"
                             "padding:4px 24px;"
                             "text-decoration:none;");
    m_replaceButton->move(m_addButton->geometry().width() + 15, game->getMinimumHeight());
    //m_addButton->geometry().width();

    connect(m_addButton, SIGNAL (clicked()), this, SLOT (handleAddButton()));

    connect(m_replaceButton, SIGNAL (clicked()), this, SLOT (handleReplaceButton()));


    // set the window size to be at least the table size
    this->resize(game->getMinimumWidth(), game->getMinimumHeight()+30);
    this->setStyleSheet("Background-color: #000; ");

    m_invoker->setAddBallCommand(new AddBallCommand(m_game));
    m_invoker->setReplaceCueBallCommand(new ReplaceCueBallCommand(m_game));

}

Dialog::~Dialog()
{
    delete aTimer;
    delete dTimer;
    delete m_game;
    delete ui;
}

void Dialog::tryRender() {
    this->update();
}

void Dialog::nextAnim() {
    m_game->animate(1.0/(double)animFrameMS);
}

void Dialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    m_game->render(painter);
}

void Dialog::handleAddButton()
{
   m_invoker->addBall();
}

void Dialog::handleReplaceButton()
{
   m_invoker->replaceCueBall();
}


void Dialog::mousePressEvent(QMouseEvent* event) {
    QPointF f = event->pos();
    m_game->setStartMousePos(QVector2D(f.x(), f.y()));
}

void Dialog::mouseReleaseEvent(QMouseEvent* event) {
    QPointF f = event->pos();
    m_game->setReleasePos(QVector2D(f.x(), f.y()));
}
void Dialog::mouseMoveEvent(QMouseEvent* event) {
    QPointF f = event->pos();
    m_game->setEndMousePos(QVector2D(f.x(), f.y()));
}

void Dialog::keyPressEvent(QKeyEvent *event){

    if( event->key() == Qt::Key_R)
    {
        m_game->recoveryState( m_game->getCaretaker()->popMemento());
    }
}
