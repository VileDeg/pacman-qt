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
        ASSERT(false);
    }
}

void Player::action()
{
    _tileOverlapped = false;
    bool died = false;
    _scene->collideWithEnemy(pos().toPoint(), &died);
    if (died) {
        return;
    }

    updatePosition();

    if (_remPixels.isNull()) {
        _tileOverlapped = true;

        bool win = false;
        _scene->playerInteract(_t.x, _t.y, &win);
        if (win) {
            return;
        }

        scanAround();

        if (_goToMouse) { //Move to mouse click
            if (!_path.empty()) {
                auto next = _path.front();
                setDirTo(next);
                _path.erase(_path.begin());
            } else { // Target reached
                _nextDir = MoveDir::None;
                _goToMouse = false;
            }
        }

        if (_scene->_toBeRecorded) { // Record next dir for replay
            if (!_moveSeq.empty() && _nextDir == _currentDir) { // If the next dir is the same as the current dir, just increment the count
                _moveSeq.back().second += 1;
            } else {
                _moveSeq.push_back({ _nextDir, 0 });
            }
        }

        if (_scene->_replay) {
            if (_moveSeqIndex < _moveSeq.size()) {
                _nextDir = _moveSeq[_moveSeqIndex].first;
                if (_moveSeq[_moveSeqIndex].second > 0) {
                    _moveSeq[_moveSeqIndex].second -= 1;
                } else {
                    ++_moveSeqIndex;
                }
            } else {
                _nextDir = MoveDir::None;
                PRINF("MoveSeq empty");
                //ASSERT(false);
            }
        }
    }

    processMovement();
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