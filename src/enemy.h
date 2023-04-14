/** @file enemy.h
 *  @author Vadim Goncearenco <xgonce00@stud.fit.vutbr.cz>
 *  @brief File with enemy class declaration.
 *  @details This file contatins declaration for enemy class.
 *  Enemies move in random directions and kill player on touch.
 */

#ifndef ENEMY_H
#define ENEMY_H

#include "animatedsprite.h"

#include <random>
#include <iostream>

/**
 * @brief Class for enemy.
 * @details This class is used for enemy movement and animation.
 */
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

#endif // ENEMY_H