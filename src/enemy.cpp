#include "enemy.h"
#include "utils.h"
#include "gamescene.h"

Enemy::Enemy(TileData t, size_t seed, GameScene* parent)
    : AnimatedSprite(SpriteType::Enemy, t, parent),
    _seed(seed), _generator(_seed)
{
    initAnimation(":/sprites/enemy/");
}

void Enemy::action(bool isGameReplayed)
{
    updatePosition();

    if (_remPixels.isNull()) {
        onTileOverlap();
    } 

    scanAround();
    processMovement();
}

void Enemy::onTileOverlap()
{
    setRandomNextDir();
}

void Enemy::setRandomNextDir()
{
    QVector<MoveDir> dirs{};

    if (_aroundFree[0]) {
        dirs.push_back(MoveDir::Up);
    }
    if (_aroundFree[1]) {
        dirs.push_back(MoveDir::Left);
    }
    if (_aroundFree[2]) {
        dirs.push_back(MoveDir::Down);
    }
    if (_aroundFree[3]) {
        dirs.push_back(MoveDir::Right);
    }
    if (dirs.isEmpty()) {
        return;
    }
    std::uniform_int_distribution<int> dist(0, dirs.size()-1);
    int index = dist(_generator);

    _nextDir = dirs.at(index);
}

void Enemy::loadAnimationFrames()
{
    try {
        loadAnimationFrame(MoveDir::None, "reddown1.png");

        loadAnimationFrame(MoveDir::Up, "redup1.png");
        loadAnimationFrame(MoveDir::Up, "redup2.png");

        loadAnimationFrame(MoveDir::Left, "redleft1.png");
        loadAnimationFrame(MoveDir::Left, "redleft2.png");

        loadAnimationFrame(MoveDir::Down, "reddown1.png");
        loadAnimationFrame(MoveDir::Down, "reddown2.png");

        loadAnimationFrame(MoveDir::Right, "redright1.png");
        loadAnimationFrame(MoveDir::Right, "redright2.png");
    }
    catch (std::runtime_error& e) {
        errpr(e.what());
    }
}
