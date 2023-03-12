#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"

#include <unordered_map>

class Player : public AnimatedSprite
{
    Q_OBJECT
signals:
    void tileOverlapped(int x, int y);
public:
    explicit Player(TileData t, GameScene* parent)
        :AnimatedSprite(SpriteType::Player, t, parent)
    { initAnimation(":/sprites/player/", 25); }
        
    ~Player() {}

    void action() override;
    void setMoveDir(MoveDir dir) override { 
        _goToMouse = false;
        AnimatedSprite::setMoveDir(dir); 
    }

    void setMouseClickPath(std::vector<QPoint> path) {
        _path = path; 
        _goToMouse = true;
    }
    bool getTileOverlapped() { return _tileOverlapped; }

    QPoint getTilePos() {return QPoint(_t.x, _t.y);}
private:
    void loadAnimationFrames() override;
    void setDirTo(QPoint to);
    
    
    bool _tileOverlapped = false;
    bool _goToMouse = false;
    std::vector<QPoint> _path;
};

#endif // PLAYER_H
