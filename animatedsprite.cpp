#include "animatedsprite.h"
#include "utils.h"
#include "gamescene.h"

AnimatedSprite::AnimatedSprite(SpriteType type, TileData t, GameScene* parent)
    : Sprite(type, t, parent), _scene(parent)
{}

void AnimatedSprite::setSpriteByFrame(unsigned int frame)
{
    _spriteImage = &_animation[_currentDir][frame % _animation[_currentDir].size()];

    update(boundingRect());
}

void AnimatedSprite::initAnimation(QString path)
{
    _animPath = path;
    
    loadAnimationFrames();
    _spriteImage = &_animation[MoveDir::Right][0];
    ASSERT(_spriteImage != nullptr);

    /*QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(animationHandler()));
    timer->start(interval);*/
}

void AnimatedSprite::loadAnimationFrame(MoveDir dir, QString path)
{
    auto tmp = loadPixmap(_animPath + path);
    _animation[dir].push_back(tmp);
}

//void AnimatedSprite::animationHandler()
//{
//    ++_frameNumber;
//    if (_frameNumber >= _animation[_currentDir].size()) {
//        _frameNumber = 0;
//    }
//
//    _spriteImage = &_animation[_currentDir][_frameNumber];
//
//    update(boundingRect());
//}

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


