#include "player.h"
#include <QKeyEvent>
#include "utils.h"
#include "gamescene.h"

Player::Player(QRect rect, QObject *parent)
    : Sprite(SpriteType::Player, rect, parent)
{
    _sheetOffset = {260, 323};
    QString sheetPath = ":/img/pacman_hd.png";
    _spriteImage = new QPixmap(sheetPath); 
    if (_spriteImage->isNull()) {
        errpr("failed to open pixmap:" << sheetPath);
    }
    
}

Player::~Player() {}

void Player::action()
{
    ++_frameNumber;
    if (_frameNumber == _maxFrame) {
        _frameNumber = 0;
    }

    QPoint plPos(pos().toPoint()); // Player pos local to scene
    QPoint scPos(_scene->sceneRect().topLeft().toPoint()); // Scene pos global
    QPoint mapPos = plPos - scPos; // Player pos global
    QPoint mapPosTile = { mapPos.x() / _tileDim.width(), mapPos.y() / _tileDim.height() }; //Tile in which the player is 
    QPoint mapPosTileRem = { mapPos.x() % _tileDim.width(), mapPos.y() % _tileDim.height() };

    if (mapPosTileRem.isNull()) {
        _tilePos = mapPosTile; // Update the index of current tile
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
            break;
    }

    switch (_currentDir) {
        case MoveDir::None:
            break;
        case MoveDir::Up:
            if (tpy == 0 && !upFree) {
                _nextDir = _currentDir = MoveDir::None; // Stop
            } else {
                moveUp();
            }
            break;
        case MoveDir::Left:
            if (tpx == 0 && !leftFree) {
                _nextDir = _currentDir = MoveDir::None; // Stop
            } else {
                moveLeft();
            }
            break;
        case MoveDir::Down:
            if (tpy == 0 && !downFree) {
                _nextDir = _currentDir = MoveDir::None; // Stop
            } else {
                moveDown();
            }
            break;
        case MoveDir::Right:
            if (tpx == 0 && !rightFree) {
                _nextDir = _currentDir = MoveDir::None; // Stop
            } else {
                moveRight();
            }
            break;
        default:
                break;
    }
}

void Player::moveUp()
{
    pr("move up");
    setPos(x(), y() - 1);
}
void Player::moveLeft()
{
    pr("move left");
    setPos(x() - 1, y());
}
void Player::moveDown()
{
    pr("move down");
    setPos(x(), y() + 1);
}
void Player::moveRight()
{
    pr("move right");
    setPos(x() + 1, y());
}

void Player::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QRect r = boundingRect().toRect();

    QPoint offset(_sheetOffset.x() + _frameNumber * r.width(), _sheetOffset.y());
    QRect source = { offset.x(), offset.y(), r.width(), r.height() };
    painter->drawPixmap(r, *_spriteImage, source);

    painter->setPen(_pen);
    painter->drawRect(r);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}