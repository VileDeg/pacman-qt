/** @file sprite.h
 *  @author Vadim Goncearenco <xgonce00@stud.fit.vutbr.cz>
 *  @brief File with declration of sprite class, serializable interface and tile data struct.
 *  @details This file contains declration of sprite class serializable interface and tile data struct.
 */

#ifndef SPRITE_H
#define SPRITE_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QDataStream>

/**
 * @brief ISerializable interface.
 * @details Abstract base class for serializable classes.
 */
class ISerializable {
public:
    virtual ~ISerializable() {}

    virtual void Serialize(QDataStream& stream) = 0;
    virtual void Deserialize(QDataStream& stream) = 0;
};

/**
 * @brief Tile data struct.
 * @details Contains tile coordinates and tile width.
 */
struct TileData {
    int x = 0;
    int y = 0;
    int width = 50; /**< Width in pixels */
};

/**
 * @brief Enumeration of possible sprite types.
 */
enum class SpriteType { None, Background, Empty, Player, Wall, Enemy, Ball, Key, Lock, Door };

QDataStream& operator>>(QDataStream& stream, SpriteType& dir);
QDataStream& operator<<(QDataStream& stream, SpriteType& dir);

/**
 * @brief Base class for all sprites.
 * @details Serves as base class to all sprites, even background.
 * Handles painting of the sprite image and serialization of sprite data for replay.
 */
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
    SpriteType _type = SpriteType::None;
    TileData _t = {};

    QPen _pen;
    QBrush _brush = Qt::magenta;
    QImage* _spriteImage = nullptr;
};

#endif // SPRITE_H

