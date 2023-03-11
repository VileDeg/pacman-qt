#include "player.h"

#include <QKeyEvent>
#include <QTimer>
#include <QPixmap>

#include "utils.h"
#include "gamescene.h"
#include <map>

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
        __debugbreak();
        assert(false);
    }

    /*if (diff.x() < 0) {
        _nextDir = MoveDir::Left;
    } else if (diff.x() > 0) {
        _nextDir = MoveDir::Right;
    } else if (diff.y() < 0) {
        _nextDir = MoveDir::Up;
    } else if (diff.y() > 0) {
        _nextDir = MoveDir::Down;
    } else if (diff.isNull()) {
        _nextDir = MoveDir::None;
    } else {
        __debugbreak();
        assert(false);
        _nextDir = _currentDir;
    }*/
}

void Player::action()
{
    _tileOverlapped = false;
    bool died = false;
    _scene->collideWithEnemy(pos().toPoint(), &died);
    if (died) {
        return;
    }

    QPoint p(pos().toPoint()); // Player p local to scene
    QPoint tPos = { p.x() / _t.width, p.y() / _t.width }; //Tile in which the player is 
    QPoint rem = { p.x() % _t.width, p.y() % _t.width };

    if (rem.isNull()) {
        _tileOverlapped = true;
        _oldTPos = { _t.x, _t.y };
        _t.x = tPos.x(); // Update the index of current tile
        _t.y = tPos.y();

        _scene->playerInteract(_t.x, _t.y);

        scanAround();

        if (_goToMouse) { //Move to mouse click
            if (!_path.empty()) {
                auto next = _path.front();
                setDirTo(next);
                _path.erase(_path.begin());
            } else {
                _nextDir = MoveDir::None;
                _goToMouse = false;
            }
        }
    }

    processMovement(rem);
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