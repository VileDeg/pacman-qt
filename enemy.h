#pragma once

#include "animatedsprite.h"
#include <QRandomGenerator>
#include <iostream>

class Enemy : public AnimatedSprite
{
public:
    explicit Enemy(TileData t, size_t seed, GameScene* parent);
    ~Enemy() {}

    void action(bool isGameReplayed) override;
    void onTileOverlap() override;

    

    void setRandomNextDir();
    
private:
    void loadAnimationFrames() override;

private:
    quint64 _seed;
    QRandomGenerator _generator;
};

