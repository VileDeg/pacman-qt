#ifndef SPRITE_H
#define SPRITE_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QVector2D>

enum class SpriteType { None, Empty, Player, Wall, Enemy, Item };

class Sprite : public QObject, public QGraphicsItem
{
public:
    explicit Sprite(SpriteType type, QRect rect, QObject *parent = 0);
    explicit Sprite(SpriteType type, QRect rect, QPen pen, QBrush brush, QObject* parent = 0);
    ~Sprite();

    SpriteType getType() { return _type; }
    void setPen(QPen pen) { _pen = pen; }
    void setBrush(QBrush brush) { _brush = brush; }
protected:

    QRectF boundingRect() const override;
protected:
    SpriteType _type;
    QPoint _tilePos; //Current tile index in map
    QPoint _pos;
    QPoint _sheetOffset; //Offset in spriteSheet
    QPen _pen;
    QBrush _brush = Qt::magenta;

    QPixmap* _spriteImage = nullptr;  
    int _frameNumber{};   

    QSize _tileDim; //Frame dimensions
    
    const int _maxFrame{2};
private:
    void init();
};

#endif // SPRITE_H

