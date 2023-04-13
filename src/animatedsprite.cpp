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

void AnimatedSprite::Serialize(QDataStream& stream) {
    Sprite::Serialize(stream);
    stream << _currentDir;
};

void AnimatedSprite::Deserialize(QDataStream& stream) {
    Sprite::Deserialize(stream);
    stream >> _currentDir;
};




std::string AnimatedSprite::dir_to_str(MoveDir d) {
    switch (d) {
        case MoveDir::None: return "None";
        case MoveDir::Up: return "Up";
        case MoveDir::Left: return "Left";
        case MoveDir::Down: return "Down";
        case MoveDir::Right: return "Right";
    }
    return "None";
}



QDataStream& operator>>(QDataStream& stream, MoveDir& dir)
{
    quint8 value;
    stream >> value;
    dir = static_cast<MoveDir>(value);
    return stream;
}

QDataStream& operator<<(QDataStream& stream, MoveDir& dir)
{
    stream << static_cast<quint8>(dir);
    return stream;
}