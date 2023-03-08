#include "sprite.h"

#include "utils.h"
#include <QKeyEvent>

Sprite::Sprite(SpriteType type, QRect rect, QObject *parent)
    : QObject(parent), QGraphicsItem(),
    _type(type), _pos(rect.x(), rect.y()), _tileDim(rect.width(), rect.height()),
    _pen(Qt::magenta, 2, Qt::SolidLine)
{
    init();
}

Sprite::Sprite(SpriteType type, QRect rect, QPen pen, QBrush brush, QObject* parent)
    :QObject(parent), QGraphicsItem(),
    _type(type), _pos(rect.x(), rect.y()), _tileDim(rect.width(), rect.height()),
    _pen(pen), _brush(brush)
{
    init();
}

Sprite::~Sprite()
{
    delete _spriteImage;
}

void Sprite::init()
{
    setPos(_pos);
}

QRectF Sprite::boundingRect() const
{
    return QRect(0, 0, _tileDim.width(), _tileDim.height());
}


