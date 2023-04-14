/** @file player.cpp
 *  @author Vadim Goncearenco <xgonce00@stud.fit.vutbr.cz>
 *  @brief File with player class definition.
 *  @details This file contains player class definition.
 */

#include "player.h"
#include "utils.h"
#include "gamescene.h"

Player::Player(TileData t, GameScene* parent)
    :AnimatedSprite(SpriteType::Player, t, parent)
{
    initAnimation("sprites/player/");
}

void Player::setMoveDir(MoveDir dir) {
    _goToMouse = false;
    AnimatedSprite::setMoveDir(dir);
}

void Player::setMouseClickPath(std::vector<QPoint> path) {
    _path = path;
    _goToMouse = true;
}

void Player::setDirTo(QPoint to) {
    auto diff = to - getTilePos();
    if (diff == QPoint(0, -1)) {
        _nextDir = MoveDir::Up;
    } else if (diff == QPoint(-1, 0)) {
        _nextDir = MoveDir::Left;
    } else if (diff == QPoint(0, 1)) {
        _nextDir = MoveDir::Down;
    } else if (diff == QPoint(1, 0)) {
        _nextDir = MoveDir::Right;
    } else if (diff == QPoint(0, 0)) {
           _nextDir = MoveDir::None;
    } else {
        ASSERT(false);
    }
}

void Player::action() //TODO: remove arguments
{
    bool overlapPrev = _tileOverlapped;
    _tileOverlapped = false; // Reset tile overlap flag
    bool died = false;
    _scene->collideWithEnemy(pos().toPoint(), &died);
    if (died) { // Player touched an enemy
        return;
    }

    updatePosition(); // Update position attributes based on pixel position

    if (_remPixels.isNull()) { // If the player overlaps the tile
        onTileOverlap();
        if (_tileOverlapped != overlapPrev) {
            emit tileOverlapped();
        }
    }

    processMovement(); // Move the player based on next direction
}

void Player::onTileOverlap()
{
    _tileOverlapped = true;

    bool win = false;
    _scene->playerInteract(_t.x, _t.y, &win);
    if (win) { // Player reached the exit
        return;
    }

    scanAround(); // Check in which directions the player can move

    if (_goToMouse) { //If player direction was set by mouse
        if (!_path.empty()) { // If there are still tiles to move to
            auto next = _path.front();
            setDirTo(next);
            _path.erase(_path.begin());
        } else { // Target reached
            _nextDir = MoveDir::None;
            _goToMouse = false;
        }
    }
}

void Player::loadAnimationFrames()
{
    try {
        loadAnimationFrame(MoveDir::None, "a1.png");

        loadAnimationFrame(MoveDir::Up, "a1.png");
        loadAnimationFrame(MoveDir::Up, "b2.png");
        loadAnimationFrame(MoveDir::Up, "b3.png");
        loadAnimationFrame(MoveDir::Up, "b4.png");
        loadAnimationFrame(MoveDir::Up, "b5.png");
        loadAnimationFrame(MoveDir::Up, "b6.png");
        loadAnimationFrame(MoveDir::Up, "b5.png");
        loadAnimationFrame(MoveDir::Up, "b4.png");
        loadAnimationFrame(MoveDir::Up, "b3.png");
        loadAnimationFrame(MoveDir::Up, "b2.png");

        loadAnimationFrame(MoveDir::Left, "a1.png");
        loadAnimationFrame(MoveDir::Left, "c2.png");
        loadAnimationFrame(MoveDir::Left, "c3.png");
        loadAnimationFrame(MoveDir::Left, "c4.png");
        loadAnimationFrame(MoveDir::Left, "c5.png");
        loadAnimationFrame(MoveDir::Left, "c6.png");
        loadAnimationFrame(MoveDir::Left, "c5.png");
        loadAnimationFrame(MoveDir::Left, "c4.png");
        loadAnimationFrame(MoveDir::Left, "c3.png");
        loadAnimationFrame(MoveDir::Left, "c2.png");

        loadAnimationFrame(MoveDir::Down, "a1.png");
        loadAnimationFrame(MoveDir::Down, "d2.png");
        loadAnimationFrame(MoveDir::Down, "d3.png");
        loadAnimationFrame(MoveDir::Down, "d4.png");
        loadAnimationFrame(MoveDir::Down, "d5.png");
        loadAnimationFrame(MoveDir::Down, "d6.png");
        loadAnimationFrame(MoveDir::Down, "d5.png");
        loadAnimationFrame(MoveDir::Down, "d4.png");
        loadAnimationFrame(MoveDir::Down, "d3.png");
        loadAnimationFrame(MoveDir::Down, "d2.png");

        loadAnimationFrame(MoveDir::Right, "a1.png");
        loadAnimationFrame(MoveDir::Right, "a2.png");
        loadAnimationFrame(MoveDir::Right, "a3.png");
        loadAnimationFrame(MoveDir::Right, "a4.png");
        loadAnimationFrame(MoveDir::Right, "a5.png");
        loadAnimationFrame(MoveDir::Right, "a6.png");
        loadAnimationFrame(MoveDir::Right, "a5.png");
        loadAnimationFrame(MoveDir::Right, "a4.png");
        loadAnimationFrame(MoveDir::Right, "a3.png");
        loadAnimationFrame(MoveDir::Right, "a2.png");
    }
    catch (std::runtime_error& e) {
        errpr(e.what());
    }
}