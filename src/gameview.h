/** @file gameview.h
 *  @author Vadim Goncearenco <xgonce00@stud.fit.vutbr.cz>
 *  @brief File with game view class declaration.
 *  @details This file contains declaration for game view class.
 *  It is a small wrapper around QGraphicsView, which only determines some appearance settings.
 */

#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>

class GameView : public QGraphicsView
{
public:
    GameView(QWidget *parent = 0);

    ~GameView();
private:
    void Init();
};

#endif // GAMEVIEW_H
