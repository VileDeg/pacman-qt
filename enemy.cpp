#include "enemy.h"
#include "utils.h"
#include "gamescene.h"


Enemy::Enemy(TileData t, size_t seed, GameScene* parent)
    : AnimatedSprite(SpriteType::Enemy, t, parent),
    _seed(seed), _generator(_seed)
{
    initAnimation(":/sprites/enemy/");
}

void Enemy::action(bool isGameReplayed, bool replayForward)
{
    updatePosition();

    if (_remPixels.isNull()) {
        onTileOverlap();
    } else if (isGameReplayed && !replayForward) {
        reverseNextDir();
    }

    scanAround();
    processMovement();
}

void Enemy::onTileOverlap()
{
    if (!_scene->_replay) {
        setRandomNextDir();
    } else {
        getNextDirReplay();
    }
}

void Enemy::getNextDirReplay()
{
    if (_scene->getReplayMode()) { // Replay forward
        setRandomNextDir();
        storeNextDir();
    } else { // Replay backward
        if (_moveSeqIndex > 0) {
            _nextDir = _moveSeq[_moveSeqIndex].first;
            static QPair<MoveDir, size_t> lastMove = _moveSeq[_moveSeqIndex];
            if (_nextDir == lastMove.first) { // If the next move is repeated, decrement the count
                lastMove.second -= 1;
            } else { // Otherwise, move to the next move
                --_moveSeqIndex;
                lastMove = _moveSeq[_moveSeqIndex];
            }
            reverseNextDir();
        } else { // No more moves to replay
            _nextDir = MoveDir::None;
            PRINF("MoveSeq empty");
            ASSERTMSG(false, "MoveSeq empty");
        }
    }
}

void Enemy::setRandomNextDir()
{
    QVector<MoveDir> dirs{};
    int tpx = _t.x;
    int tpy = _t.y;

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
