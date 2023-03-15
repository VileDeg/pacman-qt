#pragma once
#include "sprite.h"
#include <QPair>

enum class MoveDir { None, Up, Left, Down, Right };

class GameScene;

class AnimatedSprite : public Sprite
{
    Q_OBJECT
private slots:
    void animationHandler();
public:
    explicit AnimatedSprite(SpriteType type, TileData t, GameScene* parent);
    ~AnimatedSprite() {}

    virtual void setMoveDir(MoveDir dir) { _nextDir = dir; }
    void setScene(GameScene* scene) { _scene = scene; }

    virtual void action() = 0;
    QVector<QPair<MoveDir, size_t>> _moveSeq{};
    size_t _moveSeqIndex = 0;
protected:
    void moveUp() { setPos(x(), y() - 1); }
    void moveLeft() { setPos(x() - 1, y()); }
    void moveDown() { setPos(x(), y() + 1); }
    void moveRight() { setPos(x() + 1, y()); }

    void loadAnimationFrame(MoveDir dir, QString path);
    virtual void loadAnimationFrames() = 0;
    void initAnimation(QString path, int interval);
    void processMovement(QPoint rem);
    void scanAround();

    GameScene* _scene;
    MoveDir _currentDir = MoveDir::None;
    MoveDir _nextDir = MoveDir::None;
    bool _aroundFree[4]; //WASD
private:
    int _frameNumber = 0;
    QString _animPath = ":/sprites/";
    

    std::unordered_map<MoveDir, std::vector<QImage>> _animation{};
};

