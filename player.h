#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"

#include <unordered_map>
#include <iostream>
#include "utils.h"

class Player : public AnimatedSprite
{
    Q_OBJECT
signals:
    //void tileOverlapped(int x, int y);
    void tileOverlapped();


public:
    explicit Player(TileData t, GameScene* parent)
        :AnimatedSprite(SpriteType::Player, t, parent)
    { 
        initAnimation(":/sprites/player/"); 
    }
        
    ~Player() {}

    void action(bool isGameReplayed) override;
    void onTileOverlap() override;

    void SaveToStream(QDataStream& stream) override;

    void LoadFromStream(QDataStream& stream) override;

    void setMoveDir(MoveDir dir) override { 
        _goToMouse = false;
        AnimatedSprite::setMoveDir(dir); 
    }

    void setMouseClickPath(std::vector<QPoint> path) {
        _path = path; 
        _goToMouse = true;
    }
    bool getTileOverlapped() { return _tileOverlapped; }
    //void storeNextDir();

    QPoint getTilePos() {return QPoint(_t.x, _t.y);}
private:
    void loadAnimationFrames() override;
    void setDirTo(QPoint to);
    
    QVector<QPair<MoveDir, int>> _moveSeq{};
    int _moveSeqIndex = 0;
    bool _tileOverlapped = false;
    bool _goToMouse = false;
    std::vector<QPoint> _path;
};

#endif // PLAYER_H
