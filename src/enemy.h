#pragma once

#include "animatedsprite.h"

#include <random>
#include <iostream>

class Enemy : public AnimatedSprite
{
public:
    explicit Enemy(TileData t, size_t seed, GameScene* parent);
    ~Enemy() {}

    void action() override;
    void onTileOverlap() override;

    void setRandomNextDir();
    
private:
    void loadAnimationFrames() override;

private:
    size_t _seed;
    std::mt19937 _generator;
};

