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
    void tileOverlapped();

public:
    explicit Player(TileData t, GameScene* parent);
    ~Player() {}

    void action(bool isGameReplayed) override;
    void onTileOverlap() override;
    void setMoveDir(MoveDir dir) override;

    void setMouseClickPath(std::vector<QPoint> path);
    bool getTileOverlapped() { return _tileOverlapped; }

    QPoint getTilePos() {return QPoint(_t.x, _t.y);}
private:
    void loadAnimationFrames() override;

    void setDirTo(QPoint to);

private:        
    bool _tileOverlapped = false;
    bool _goToMouse = false;
    std::vector<QPoint> _path;
};

#endif // PLAYER_H
