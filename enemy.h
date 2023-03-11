#pragma once

#include "animatedsprite.h"

class Enemy : public AnimatedSprite
{
public:
    explicit Enemy(TileData t, GameScene* parent)
        : AnimatedSprite(SpriteType::Enemy, t, parent)
    { initAnimation(":/sprites/enemy/", 100); }

    ~Enemy() {}

    void action() override;
private:
    void setRandomNextDir();

    void loadAnimationFrames() override;
};

