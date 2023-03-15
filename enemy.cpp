#include "enemy.h"
#include "utils.h"
#include "gamescene.h"


Enemy::Enemy(TileData t, size_t seed, GameScene* parent)
    : AnimatedSprite(SpriteType::Enemy, t, parent),
    _seed(seed), _generator(_seed)
{
    initAnimation(":/sprites/enemy/", 100);
}

void Enemy::action()
{
    updatePosition();

    if (_remPixels.isNull()) {
        setRandomNextDir();
    }
    scanAround();
    processMovement();
}


void Enemy::setRandomNextDir()
{
    QVector<MoveDir> dirs{};
    int tpx = _t.x;
    int tpy =_t.y;

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
    //_nextDir = dirs.at(QRandomGenerator::global()->generate() % dirs.size());
    _nextDir = dirs.at(_generator.generate() % dirs.size());
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
