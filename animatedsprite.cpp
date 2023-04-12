#include "animatedsprite.h"
#include "utils.h"
#include "gamescene.h"

AnimatedSprite::AnimatedSprite(SpriteType type, TileData t, GameScene* parent)
    : Sprite(type, t, parent), _scene(parent)
{}

void AnimatedSprite::setSpriteByFrame(unsigned int frame, bool replay, bool replayForward)
{
    MoveDir dir = _currentDir;
    if (replay && !replayForward) {
        if (_nextDir == MoveDir::Up) {
            dir = MoveDir::Down;
        } else if (_nextDir == MoveDir::Left) {
            dir = MoveDir::Right;
        } else if (_nextDir == MoveDir::Down) {
            dir = MoveDir::Up;
        } else if (_nextDir == MoveDir::Right) {
            dir = MoveDir::Left;
        }
    }
    _spriteImage = &_animation[dir][frame % _animation[dir].size()];

    update(boundingRect());
}

void AnimatedSprite::replayNextDir(bool forward)
{
    if (forward && _moveSeqIndex < _moveSeq.size() && _moveSeqIndex >= 0) { // If there are still moves to replay
        _nextDir = _moveSeq[_moveSeqIndex].first;
        if (dynamic_cast<Player*>(this)) {
            std::cout << "LastMove: " << dir_to_str(_currentDir) << ", " << _moveCounter << ", " << _moveSeqIndex << std::endl;
        }
        if (_moveCounter > 0) { // If the next move is the same as last, decrement the count
            _moveCounter -= 1; //_nextDir == _currentDir && 
        } else { // Otherwise, move to the next move
            ++_moveSeqIndex;
            if (_moveSeqIndex < _moveSeq.size()) {
                _moveCounter = _moveSeq[_moveSeqIndex].second;
            }
        }
    } else if (!forward && _moveSeqIndex < _moveSeq.size() && _moveSeqIndex >= 0) {
        _nextDir = _moveSeq[_moveSeqIndex].first;
        if (dynamic_cast<Player*>(this)) {
            std::cout << "LastMove: " << dir_to_str(_currentDir) << ", " << _moveCounter << ", " << _moveSeqIndex << std::endl;
        }
        if (_moveCounter < _moveSeq[_moveSeqIndex].second) { // If the next move is the same as last, decrement the count
            _moveCounter += 1; //_nextDir == _currentDir && 
        } else { // Otherwise, move to the next move
            --_moveSeqIndex;
            _moveCounter = 0;
        }
        reverseNextDir();
    } else { // No more moves to replay
        _nextDir = MoveDir::None;
        PRINF("MoveSeq empty");
    }

    if (dynamic_cast<Player*>(this)) {
        /*std::cout << "After: " << std::endl;
        for (auto& m : _moveSeq) {
            std::cout << "(" << static_cast<int>(m.first) << " : " << m.second << "), ";
        }
        std::cout << std::endl;*/
        std::cout << "Dir: " << dir_to_str(_nextDir) << "\tIndex: " << _moveSeqIndex << std::endl;
    }   
}

void AnimatedSprite::initAnimation(QString path)
{
    _animPath = path;
    
    loadAnimationFrames();
    _spriteImage = &_animation[MoveDir::Right][0];
    ASSERT(_spriteImage != nullptr);
}

void AnimatedSprite::loadAnimationFrame(MoveDir dir, QString path)
{
    auto tmp = loadPixmap(_animPath + path);
    _animation[dir].push_back(tmp);
}

void AnimatedSprite::scanAround()
{
    _aroundFree[0] = _scene->canMoveTo(_t.x, _t.y-1); //Up
    _aroundFree[1] = _scene->canMoveTo(_t.x-1, _t.y); //Left
    _aroundFree[2] = _scene->canMoveTo(_t.x, _t.y+1); //Down
    _aroundFree[3] = _scene->canMoveTo(_t.x+1, _t.y); //Right
}

void AnimatedSprite::updatePosition()
{
    QPoint p(pos().toPoint()); // Position local to scene
    _t.x = p.x() / _t.width;
    _t.y = p.y() / _t.width;
    _remPixels = { p.x() % _t.width, p.y() % _t.width };
}

void AnimatedSprite::processMovement()
{
    switch (_nextDir) {
        case MoveDir::Up:
            if (_remPixels.x() == 0 && _aroundFree[0]) {
                _currentDir = _nextDir;
            }
            break;
        case MoveDir::Left:
            if (_remPixels.y() == 0 && _aroundFree[1]) {
                _currentDir = _nextDir;
            }
            break;
        case MoveDir::Down:
            if (_remPixels.x() == 0 && _aroundFree[2]) {
                _currentDir = _nextDir;
            }
            break;
        case MoveDir::Right:
            if (_remPixels.y() == 0 && _aroundFree[3]) {
                _currentDir = _nextDir;
            }
            break;
        default:
            _currentDir = _nextDir;
            break;
    }

    auto doStop = [&]() { _nextDir = _currentDir = MoveDir::None; };

    switch (_currentDir) {
        case MoveDir::None:
            break;
        case MoveDir::Up:
            if (_remPixels.y() == 0 && !_aroundFree[0]) {
                doStop();
            } else {
                moveUp();
            }
            break;
        case MoveDir::Left:
            if (_remPixels.x() == 0 && !_aroundFree[1]) {
                doStop();
            } else {
                moveLeft();
            }
            break;
        case MoveDir::Down:
            if (_remPixels.y() == 0 && !_aroundFree[2]) {
                doStop();
            } else {
                moveDown();
            }
            break;
        case MoveDir::Right:
            if (_remPixels.x() == 0 && !_aroundFree[3]) {
                doStop();
            } else {
                moveRight();
            }
            break;
        default:
            break;
    }
}
