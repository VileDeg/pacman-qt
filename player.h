#ifndef PLAYER_H
#define PLAYER_H

#include "sprite.h"

#include <unordered_map>

class GameScene;

enum class MoveDir { None, Up, Left, Down, Right };

class Player : public Sprite
{
    Q_OBJECT
    
private slots:
    void animationHandler();
public:
    explicit Player(QRect rect, QObject *parent = 0);
    ~Player();

    void action();
    
    void setMoveDir(MoveDir dir) { _nextDir = dir; }
    void setScene(GameScene* scene) { _scene = scene; }
private:
    void moveUp() { setPos(x(), y() - 1); } 
    void moveLeft() { setPos(x() - 1, y()); }
    void moveDown() { setPos(x(), y() + 1); }
    void moveRight() { setPos(x() + 1, y()); }
    void loadAnimationFrame(MoveDir dir, QString path);
    void loadAnimationFrames();
private:
    GameScene* _scene;
    MoveDir _currentDir = MoveDir::None;
    MoveDir _nextDir = MoveDir::None;

    int _animInterval = 20; //Ms
    int _frameNumber = 0;

    std::unordered_map<MoveDir, std::vector<QPixmap>> _animation{};
};

#endif // PLAYER_H
