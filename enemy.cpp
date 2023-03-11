#include "enemy.h"
#include "utils.h"
#include "gamescene.h"

#include <QRandomGenerator>

void Enemy::action()
{
    QPoint p(pos().toPoint()); // Player p local to scene
    QPoint tPos = { p.x() / _t.width, p.y() / _t.width }; //Tile in which the player is 
    QPoint rem = { p.x() % _t.width, p.y() % _t.width };

    if (rem.isNull()) {
        _t.x = tPos.x(); // Update the index of current tile
        _t.y = tPos.y();

        setRandomNextDir();
    }

    processMovement(rem);
}


void Enemy::setRandomNextDir()
{
    QVector<MoveDir> aroundFree{};
    int tpx = _t.x;
    int tpy =_t.y;

    bool upFree = _scene->canMoveTo(tpx, tpy + 1);
    bool leftFree = _scene->canMoveTo(tpx-1, tpy);
    bool downFree = _scene->canMoveTo(tpx, tpy-1);
    bool rightFree = _scene->canMoveTo(tpx+1, tpy);

    if (upFree) {
        aroundFree.push_back(MoveDir::Up);
    }
    if (leftFree) {
        aroundFree.push_back(MoveDir::Left);
    }
    if (downFree) {
        aroundFree.push_back(MoveDir::Down);
    }
    if (rightFree) {
        aroundFree.push_back(MoveDir::Right);
    }
    if (aroundFree.isEmpty()) {
        return;
    }
    _nextDir = aroundFree.at(QRandomGenerator::global()->generate() % aroundFree.size());
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
