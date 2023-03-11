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

private:
  

    
    /*void onTileOverlap() override;
    void onAction() override;*/

    void loadAnimationFrames() override;
};

#endif // PLAYER_H
