#ifndef SPRITE_H
#define SPRITE_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QDataStream>

class ISerializable {
public:
    virtual ~ISerializable() {}

    virtual void Serialize(QDataStream& stream) = 0;
    virtual void Deserialize(QDataStream& stream) = 0;
};

struct TileData {
    int x;
    int y;
    int width;
};

enum class SpriteType { None, Background, Empty, Player, Wall, Enemy, Ball, Key, Lock, Door };

QDataStream& operator>>(QDataStream& stream, SpriteType& dir);
QDataStream& operator<<(QDataStream& stream, SpriteType& dir);

class Sprite : public QObject, public QGraphicsItem, public ISerializable
{
public:
    explicit Sprite(SpriteType type, TileData t, QObject* parent = 0);
    ~Sprite() {}

    QPoint getTilePos() { return QPoint(_t.x, _t.y); }
    SpriteType getType() { return _type; }

    void setPen(QPen pen) { _pen = pen; }
    void setBrush(QBrush brush) { _brush = brush; }
    void setImage(QImage* image) { _spriteImage = image; }

    void Serialize(QDataStream& stream);
    void Deserialize(QDataStream& stream);

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QRectF boundingRect() const override;
protected:
    SpriteType _type;
    TileData _t;

    QPen _pen;
    QBrush _brush = Qt::magenta;
    QImage* _spriteImage = nullptr;  
};

#endif // SPRITE_H

