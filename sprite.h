#ifndef SPRITE_H
#define SPRITE_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QVector2D>
#include <QDataStream>



struct TileData {
    int x;
    int y;
    int width;
};

enum class SpriteType { None, Background, Empty, Player, Wall, Enemy, Ball, Key, Lock, Door };

class Sprite : public QObject, public QGraphicsItem
{
public:
    explicit Sprite(SpriteType type, TileData t, QObject *parent = 0) 
        : QObject(parent), QGraphicsItem(),
        _type(type), _t(t), _pixPos(t.x* t.width, t.y* t.width),
        _pen(Qt::magenta, 2, Qt::SolidLine) 
    { init();}

    ~Sprite() {}

    QPoint getTilePos() { return QPoint(_t.x, _t.y); }
    SpriteType getType() { return _type; }
    void setPen(QPen pen) { _pen = pen; }
    void setBrush(QBrush brush) { _brush = brush; }
    void setImage(QImage* image) { _spriteImage = image; }
protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QRectF boundingRect() const override;
protected:
    SpriteType _type;
    TileData _t;
    QPoint _pixPos; //Position in pixels

    QPen _pen;
    QBrush _brush = Qt::magenta;
    QImage* _spriteImage = nullptr;  
private:
    void init();
};

#endif // SPRITE_H

