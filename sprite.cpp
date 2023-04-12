#include "sprite.h"

#include "utils.h"
#include <QKeyEvent>

Sprite::Sprite(SpriteType type, TileData t, QObject* parent)
    : QObject(parent), QGraphicsItem(),
    _type(type), _t(t),
    _pen(Qt::magenta, 2, Qt::SolidLine)
{
    setPos(t.x * t.width, t.y * t.width);
}

QRectF Sprite::boundingRect() const
{
    return QRect(0, 0, _t.width, _t.width);
}

void Sprite::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QRect r = boundingRect().toRect();

    painter->drawImage(r, *_spriteImage); //, source

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Sprite::Serialize(QDataStream& stream)
{
    /*if (_type == SpriteType::Wall) {
        return;
    }*/

    stream << _type;
    stream << pos();
    
    int t = 0;
    if (_type == SpriteType::Player) {
        pr("SER player: " << pos().x() << " " << pos().y());
    } else if (_type == SpriteType::Enemy) {
        pr("SER enemy: " << pos().x() << " " << pos().y());
    }
}

void Sprite::Deserialize(QDataStream& stream)
{
    stream >> _type;
    
    QPointF pos;
    stream >> pos;
    setPos(pos);

    if (_type == SpriteType::Player) {
        pr("DES player: " << pos.x() << " " << pos.y());
    } else if (_type == SpriteType::Enemy) {
        pr("DES enemy: " << pos.x() << " " << pos.y());
    }
}