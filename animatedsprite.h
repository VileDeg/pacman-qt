#pragma once
#include "sprite.h"
#include <QPair>

enum class MoveDir { None, Up, Left, Down, Right };

class GameScene;

class AnimatedSprite : public Sprite, public ILoadable
{
    Q_OBJECT
//private slots:
    //void animationHandler();
    //void onReplayModeSwitch();
public:
    explicit AnimatedSprite(SpriteType type, TileData t, GameScene* parent);
    ~AnimatedSprite() {}

    virtual void setMoveDir(MoveDir dir) { _nextDir = dir; }
    MoveDir getMoveDir() { return _currentDir; }
    void setScene(GameScene* scene) { _scene = scene; }
    void setSpriteByFrame(unsigned int frame, bool replay, bool replayForward);

    virtual void action(bool isGameReplayed, bool replayForward) = 0;
    virtual void onTileOverlap() = 0;
    virtual void onReplayModeSwitch() = 0;
    //virtual void getNextDirReplay() = 0;
    void LoadFromStream(QDataStream& stream) override {
        stream >> _moveSeq;
    };

    std::string dir_to_str(MoveDir d) {
        switch (d) {
            case MoveDir::None: return "None";
            case MoveDir::Up: return "Up";
            case MoveDir::Left: return "Left";
            case MoveDir::Down: return "Down";
            case MoveDir::Right: return "Right";
        }
    }

    void replayNextDir(bool forward);

    //QPair<MoveDir, int> _lastMove{ MoveDir::None, 0 };
    int _moveCounter = 0;
    QVector<QPair<MoveDir, int>> _moveSeq{};
    int _moveSeqIndex = 0;
protected:
    void moveUp()    { setPos(x(), y() - 1); }
    void moveLeft()  { setPos(x() - 1, y()); }
    void moveDown()  { setPos(x(), y() + 1); }
    void moveRight() { setPos(x() + 1, y()); }

    void reverseNextDir() {
        if (_nextDir == MoveDir::Up) {
            _nextDir = MoveDir::Down;
        } else if (_nextDir == MoveDir::Left) {
            _nextDir = MoveDir::Right;
        } else if (_nextDir == MoveDir::Down) {
            _nextDir = MoveDir::Up;
        } else if (_nextDir == MoveDir::Right) {
            _nextDir = MoveDir::Left;
        }
    }

    void storeNextDir() {
        if (!_moveSeq.empty() && _nextDir == _currentDir) { // If the next dir is the same as the current dir, just increment the count
            _moveSeq.back().second += 1;
        } else {
            _moveSeq.push_back({ _nextDir, 0 }); // Dir doesn't repeat so count is 0
        }
    }

    void loadAnimationFrame(MoveDir dir, QString path);
    virtual void loadAnimationFrames() = 0;
    void initAnimation(QString path);
    void processMovement();
    void scanAround();
    void updatePosition();

    
    GameScene* _scene;
    MoveDir _currentDir = MoveDir::None;
    MoveDir _nextDir = MoveDir::None;
    QPoint _remPixels{0,0};
    bool _aroundFree[4]{}; //WASD
private:
    //int _frameNumber = 0;
    QString _animPath = ":/sprites/";


    std::unordered_map<MoveDir, std::vector<QImage>> _animation{};
};

