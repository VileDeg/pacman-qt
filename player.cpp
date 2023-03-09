#include "player.h"

#include <QKeyEvent>
#include <QTimer>
#include <QPixmap>

#include "utils.h"
#include "gamescene.h"

Player::Player(QRect rect, QObject *parent)
    : Sprite(SpriteType::Player, rect, parent)
{
    /*_sheetOffset = {260, 323};
    QString sheetPath = ":/img/pacman_hd.png";
    _spriteImage = new QPixmap(sheetPath); 
    if (_spriteImage->isNull()) {
        errpr("failed to open pixmap:" << sheetPath);
    }*/

    loadAnimationFrames();
    _spriteImage = &_animation[MoveDir::Right][0];

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(animationHandler()));
    timer->start(_animInterval);
}

Player::~Player() {}

void Player::loadAnimationFrame(MoveDir dir, QString path)
{
    static QString animPath = ":/sprites/player/";
    //QPixmap tmp(animPath+path);
    //if (tmp.isNull()) {
    //    //errpr("failed to open pixmap:" << path);
    //    throw std::runtime_error("failed to open pixmap" + path.toStdString());
    //}
    _animation[dir].push_back(loadPixmap(animPath + path));
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

void Player::animationHandler()
{
    ++_frameNumber;
    if (_frameNumber >= _animation[_currentDir].size()) {
        _frameNumber = 0;
    }
    /*if (_currentDir == MoveDir::None) {
        pr("Frame: " << _frameNumber);
    }*/

    _spriteImage = &_animation[_currentDir][_frameNumber];

    update(boundingRect());
}


void Player::action()
{
    //v2pr(pos());
    //pr("Next: " << int(_nextDir) << " Current: " << int(_currentDir));

    QPoint plPos(pos().toPoint()); // Player pos local to scene
    //QPoint scPos(_scene->sceneRect().topLeft().toPoint()); // Scene pos global
    //QPoint mapPos = plPos - scPos; // Player pos global
    QPoint mapPos = plPos;
    QPoint mapPosTile = { mapPos.x() / _tileDim.width(), mapPos.y() / _tileDim.height() }; //Tile in which the player is 
    QPoint mapPosTileRem = { mapPos.x() % _tileDim.width(), mapPos.y() % _tileDim.height() };

    if (mapPosTileRem.isNull()) {
        _tilePos = mapPosTile; // Update the index of current tile

        _scene->interactAt(mapPosTile.y(), mapPosTile.x());
    }

    int tpx = _tilePos.x();
    int tpy = _tilePos.y();
    bool upFree = _scene->canMoveTo(tpy - 1, tpx);
    bool leftFree = _scene->canMoveTo(tpy, tpx - 1);
    bool downFree = _scene->canMoveTo(tpy + 1, tpx);
    bool rightFree = _scene->canMoveTo(tpy, tpx + 1);

    switch (_nextDir) {
        case MoveDir::Up:
            if (mapPosTileRem.x() == 0 && upFree) {
                _currentDir = _nextDir;
            }
            break;
        case MoveDir::Left:
            if (mapPosTileRem.y() == 0 && leftFree) {
                _currentDir = _nextDir;
            }
            break;
        case MoveDir::Down:
            if (mapPosTileRem.x() == 0 && downFree) {
                _currentDir = _nextDir;
            }
            break;
        case MoveDir::Right:
            if (mapPosTileRem.y() == 0 && rightFree) {
                _currentDir = _nextDir;
            }
            break;
        default:
            _currentDir = _nextDir;
            break;
    }

    switch (_currentDir) {
        case MoveDir::None:
            break;
        case MoveDir::Up:
            if (mapPosTileRem.y() == 0 && !upFree) {
                _nextDir = _currentDir = MoveDir::None; // Stop
            } else {
                moveUp();
            }
            break;
        case MoveDir::Left:
            if (mapPosTileRem.x() == 0 && !leftFree) {
                _nextDir = _currentDir = MoveDir::None; // Stop
            } else {
                moveLeft();
            }
            break;
        case MoveDir::Down:
            if (mapPosTileRem.y() == 0 && !downFree) {
                _nextDir = _currentDir = MoveDir::None; // Stop
            } else {
                moveDown();
            }
            break;
        case MoveDir::Right:
            if (mapPosTileRem.x() == 0 && !rightFree) {
                _nextDir = _currentDir = MoveDir::None; // Stop
            } else {
                moveRight();
            }
            break;
        default:
            break;
    }
}
