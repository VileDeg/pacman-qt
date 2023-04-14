#include "sprite.h"
#include "utils.h"

#include <QVector2D>

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
    stream << _type;
    stream << pos();
}

void Sprite::Deserialize(QDataStream& stream)
{
    stream >> _type;
    
    QPointF pos;
    stream >> pos;
    setPos(pos);
}

QDataStream& operator>>(QDataStream& stream, SpriteType& dir)
{
    quint8 value;
    stream >> value;
    dir = static_cast<SpriteType>(value);
    return stream;
}

QDataStream& operator<<(QDataStream& stream, SpriteType& dir)
{
    stream << static_cast<quint8>(dir);
    return stream;
}
