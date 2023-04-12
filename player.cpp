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

void Player::action(bool isGameReplayed) //TODO: remove arguments
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

    if (_scene->_toBeRecorded) { // Record next dir for replay
        if (!_moveSeq.empty() && _moveSeq.back().first == _nextDir) { // If the last move is the same as the current one
            ++_moveSeq.back().second; // Increment the count
        } else { // If the last move is different from the current one
            _moveSeq.push_back({ _nextDir,0 }); // Dir doesn't repeat so count is 0
        }
    } else if (_scene->_replay) { // Replay next dir
        if (_moveSeqIndex < _moveSeq.size() && _moveSeqIndex >= 0) { // If there are still moves to replay
            _nextDir = _moveSeq[_moveSeqIndex].first;
            if (_moveSeq[_moveSeqIndex].second > 0) { // If the move repeats
                --_moveSeq[_moveSeqIndex].second; // Decrement the count
            } else { // If the move doesn't repeat
                ++_moveSeqIndex; // Move to the next move
            }
        } else { // No more moves to replay
            _nextDir = MoveDir::None;
            PRINF("MoveSeq empty");
            //ASSERTMSG(false, "MoveSeq empty");
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

void Player::SaveToStream(QDataStream& stream) {
    stream << _moveSeq;
    std::cout << "SAVE MoveSeq:" << std::endl;
    for (auto& m : _moveSeq) {
        std::cout << "(" << dir_to_str(m.first) << " " << m.second << "), ";
    }
    std::cout << std::endl;
};

void Player::LoadFromStream(QDataStream& stream) {
    stream >> _moveSeq;
    ASSERT(!_moveSeq.empty());
    std::cout << "LOAD MoveSeq:" << std::endl;
    for (auto& m : _moveSeq) {
        std::cout << "(" << dir_to_str(m.first) << " " << m.second << "), ";
    }
    std::cout << std::endl;
};

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