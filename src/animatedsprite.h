/** @file animatedsprite.sh
 *  @author Vadim Goncearenco <xgonce00@stud.fit.vutbr.cz>
 *  @brief File with animated sprite class declaration.
 */

#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include "sprite.h"

/**
 * @brief Enumeration of possible directions of movement.
 */
enum class MoveDir { None, Up, Left, Down, Right };

QDataStream& operator>>(QDataStream& stream, MoveDir& dir);
QDataStream& operator<<(QDataStream& stream, MoveDir& dir);

class GameScene;

/**
 * @brief Base class for moving sprites.
 * @details Serves as base class to player and enemy classes.
 * Handles movement and animation.
 */
class AnimatedSprite : public Sprite
{
public:
    explicit AnimatedSprite(SpriteType type, TileData t, GameScene* parent);
    ~AnimatedSprite() {}

    void setScene(GameScene* scene) { _scene = scene; }
    void setSpriteByFrame(unsigned int frame);

    virtual void setMoveDir(MoveDir dir) { _nextDir = dir; }
    virtual void action() = 0;

    void Serialize(QDataStream& stream) override;
    void Deserialize(QDataStream& stream) override;
    
protected:
    virtual void onTileOverlap() = 0;
    virtual void loadAnimationFrames() = 0;

    void moveUp()    { setPos(x(), y() - 1); }
    void moveLeft()  { setPos(x() - 1, y()); }
    void moveDown()  { setPos(x(), y() + 1); }
    void moveRight() { setPos(x() + 1, y()); }
    
    void loadAnimationFrame(MoveDir dir, QString path);
    void initAnimation(QString path);
    void processMovement();
    void scanAround();
    void updatePosition();

    static std::string dir_to_str(MoveDir d);
  
protected:

    GameScene* _scene;
    MoveDir _currentDir = MoveDir::None;
    MoveDir _nextDir = MoveDir::None;
    QPoint _remPixels{0,0};
    bool _aroundFree[4]{}; //WASD
private:
    QString _animPath = "sprites/";

    std::unordered_map<MoveDir, std::vector<QImage>> _animation{};
};

#endif // ANIMATEDSPRITE_H