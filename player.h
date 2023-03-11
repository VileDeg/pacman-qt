#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"

#include <unordered_map>

class Player : public AnimatedSprite
{
public:
    explicit Player(TileData t, GameScene* parent)
        :AnimatedSprite(SpriteType::Player, t, parent)
    { initAnimation(":/sprites/player/", 25); }
        
    ~Player() {}

    void action() override;
    void setPathTo(QPointF mousePos);
    void setMoveDir(MoveDir dir) override { 
        
        AnimatedSprite::setMoveDir(dir); 
    }
    std::vector<QPoint> _path;

    QPoint getTilePos() {return QPoint(_t.x, _t.y);}
    void setDirTo(QPoint to) { 
        if (to - getTilePos() == QPoint(0, -1)) setMoveDir(MoveDir::Up);
        else if (to - getTilePos() == QPoint(-1, 0)) setMoveDir(MoveDir::Left);
        else if (to - getTilePos() == QPoint(0, 1)) setMoveDir(MoveDir::Down);
        else if (to - getTilePos() == QPoint(1, 0)) setMoveDir(MoveDir::Right);
        else assert(false);
    }
    QPoint _target{-1,-1};
private:
    
    
    
    /*void onTileOverlap() override;
    void onAction() override;*/

    void loadAnimationFrames() override;
};

#endif // PLAYER_H
