#include "wall.h"

Wall::Wall(QRect rect, QPen pen, QBrush brush, QObject* parent) 
    : Sprite(SpriteType::Wall, rect, pen, brush, parent)
{
}

Wall::~Wall()
{
}

void Wall::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QRect r = boundingRect().toRect();

    painter->setBrush(_brush);
    painter->drawRect(r);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}
