#ifndef PLAYER_H
#define PLAYER_H

#include "sprite.h"

class GameScene;

enum class MoveDir { None, Up, Left, Down, Right };

class Player : public Sprite
{
public:
    explicit Player(QRect rect, QObject *parent = 0);
    ~Player();

    void action();

    void setMoveDir(MoveDir dir) { _nextDir = dir; }
    void setScene(GameScene* scene) { _scene = scene; }
private:
    void moveUp();
    void moveLeft();
    void moveDown();
    void moveRight();

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
private:
    GameScene* _scene;
    MoveDir _currentDir = MoveDir::None;
    MoveDir _nextDir = MoveDir::None;
};

#endif // PLAYER_H
