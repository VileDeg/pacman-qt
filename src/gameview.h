#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>

class GameView : public QGraphicsView
{
public:
    GameView(QWidget *parent = 0);

    ~GameView();
private:
    void Init();
};

#endif // VIEW_H
