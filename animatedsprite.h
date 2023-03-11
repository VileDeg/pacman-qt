#pragma once
#include "sprite.h"

enum class MoveDir { None, Up, Left, Down, Right };

class GameScene;

class AnimatedSprite : public Sprite
{
    Q_OBJECT
private slots:
    void animationHandler();
public:
    //static_cast<QObject*>(static_cast<QGraphicsScene*>(parent)))
    explicit AnimatedSprite(SpriteType type, TileData t, GameScene* parent);
    ~AnimatedSprite() {}

    virtual void setMoveDir(MoveDir dir) { _nextDir = dir; }
    void setScene(GameScene* scene) { _scene = scene; }

    virtual void action() = 0;

protected:
    void moveUp() { setPos(x(), y() - 1); }
    void moveLeft() { setPos(x() - 1, y()); }
    void moveDown() { setPos(x(), y() + 1); }
    void moveRight() { setPos(x() + 1, y()); }

    /*virtual void onTileOverlap() = 0;
    virtual void onAction() = 0;*/
    void loadAnimationFrame(MoveDir dir, QString path);
    virtual void loadAnimationFrames() = 0;
    void initAnimation(QString path, int interval);
    void processMovement(QPoint rem);

    GameScene* _scene;
    MoveDir _currentDir = MoveDir::None;
    MoveDir _nextDir = MoveDir::None;
    bool _aroundFree[4]; //WASD
    //QPoint _tilePos; //Current tile index in map
private:
    //void setRandomNextDir();
    void scanAround();

    
    //int _animInterval = 20; //Ms
    int _frameNumber = 0;
    QString _animPath = ":/sprites/";
    

    std::unordered_map<MoveDir, std::vector<QImage>> _animation{};
};

