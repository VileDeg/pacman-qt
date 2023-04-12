#pragma once

#include "animatedsprite.h"
#include <QRandomGenerator>

class Enemy : public AnimatedSprite
{
public:
    //size_t seed, 
    explicit Enemy(TileData t, size_t seed, GameScene* parent);
    ~Enemy() {}

    void action(bool isGameReplayed, bool replayForward) override;
    void onTileOverlap() override;
    //void getNextDirReplay() override;
    void onReplayModeSwitch() override {
        //getNextDirReplay();
    }

    void setRandomNextDir();
    
    size_t _seed;
private:
    

    void loadAnimationFrames() override;

    QRandomGenerator _generator;
};

