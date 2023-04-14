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
