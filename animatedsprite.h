#pragma once
#include "sprite.h"
#include <QPair>

enum class MoveDir { None, Up, Left, Down, Right };

class GameScene;

class AnimatedSprite : public Sprite
{
public:
    explicit AnimatedSprite(SpriteType type, TileData t, GameScene* parent);
    ~AnimatedSprite() {}

    virtual void setMoveDir(MoveDir dir) { _nextDir = dir; }
    MoveDir getMoveDir() { return _currentDir; }
    void setScene(GameScene* scene) { _scene = scene; }
    void setSpriteByFrame(unsigned int frame);

    virtual void action(bool isGameReplayed) = 0;

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
    QString _animPath = ":/sprites/";

    std::unordered_map<MoveDir, std::vector<QImage>> _animation{};
};

