#include "sprite.h"

#include "utils.h"
#include <QKeyEvent>

void Sprite::init()
{
    setPos(_pixPos);
}

QRectF Sprite::boundingRect() const
{
    return QRect(0, 0, _t.width, _t.width);
}

void Sprite::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QRect r = boundingRect().toRect();

    //painter->drawPixmap(r, *_spriteImage); //, source
    painter->drawImage(r, *_spriteImage); //, source

    Q_UNUSED(option);
    Q_UNUSED(widget);
}
