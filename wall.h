#pragma once
#include "sprite.h"

class Wall : public Sprite
{
public:
    explicit Wall(QRect rect, QPen pen, QBrush brush, QObject *parent = 0);
    ~Wall();

private:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};

