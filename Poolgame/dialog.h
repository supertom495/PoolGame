#pragma once
#include <QDialog>
#include <QPushButton>
#include <QCoreApplication>
#include "ball.h"
#include "game.h"
#include "invoker.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(Game* game, QWidget *parent = 0);
    ~Dialog();

protected:
    /**
     * @brief paintEvent - called whenever window repainting is requested
     */
    void paintEvent(QPaintEvent *);
public slots:
    /**
     * @brief nextAnim - move the objects and perform collision events
     */
    void nextAnim();
    /**
     * @brief tryRender - draw the objects to screen
     */
    void tryRender();

    void handleAddButton();
    void handleReplaceButton();


    // qt mouse click
    void mousePressEvent(QMouseEvent* event);
    // qt mouse release
    void mouseReleaseEvent(QMouseEvent* event);
    // qt mouse move
    void mouseMoveEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent *event);

private:
    /**
     * @brief aTimer - timer for calling nextAnim in intervals
     */
    QTimer* aTimer = nullptr;
    /**
     * @brief dTimer - timer for calling tryRender in intervals
     */
    QTimer* dTimer = nullptr;
    /**
     * @brief ui our drawable ui
     */
    Ui::Dialog *ui;
    /**
     * @brief m_game - our game object to be played
     */
    Game* m_game = nullptr;

    /**
     * @brief m_addButton - the button can be push to add a new ball
     */
    QPushButton *m_addButton;

    /**
     * @brief m_replaceButton - the button can be push to replace the cue ball
     */
    QPushButton *m_replaceButton;

    /**
     * @brief m_invoker - an invoker to call the command
     */
    Invoker *m_invoker = new Invoker();


};

