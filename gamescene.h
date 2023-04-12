#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QDataStream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QTime>
#include <QPair>
#include <QByteArray>
#include <QFile>
#include <unordered_map>
#include <iostream>

#include "player.h"
#include "enemy.h"

struct Node {
    int x;
    int y;
    int g;
    int h;
    int f;
    bool isWall;
    Node* parent;
    std::vector<Node*> nbs;

    friend std::ostream& operator<<(std::ostream& os, const Node& n) {
        os << "(" << n.x << ", " << n.y << ") ";
        return os;
    }
};

class GameScene : public QGraphicsScene
{
    Q_OBJECT
signals:
    void gameEnd(bool win, int score);
    void gamePause(bool pause);
    void playerScoreChanged(int score);
    void playerStepsChanged(int steps);
    
private slots:
    void playerHandler();
    void enemiesHandler();

    void playerAnimHandler();
    void enemiesAnimHandler();

    void onPlayerTileOverlapped();
public slots:
    void ReplayStepForward() {
        if (_isPaused) {
            setGamePaused(false);
        }
        _replayUntilNextTile = true;
    }

public:
    GameScene(QString filePath, int viewWidth, bool replay, QObject *parent = 0);
    ~GameScene();
    
    void playerInteract(int x, int y, bool* win);
    void collideWithEnemy(QPoint playerPos, bool* died);
    void moveSprite(int fromx, int fromy, int tox, int toy);
    int getScore() { return _playerScore; }
    bool canMoveTo(int x, int y);
    std::vector<QPoint> findPath(QPoint start, QPoint end);

    void setGamePaused(bool pause) {
        _isPaused = pause;
        emit gamePause(pause);
    }
    bool getGamePaused() {
        return _isPaused;
    }

    bool ToggleReplayMode();
    
    bool _replayUntilNextTile = false;
    
    bool _replay = false;

    void onKeyPress(QKeyEvent* event);
    void onMousePress(QMouseEvent* event, QPointF localPos);
    void processKey(QKeyEvent* event);
    void processMouse(QMouseEvent* event, QPointF localPos);

    bool _toBeRecorded = true;
private:
    void setPlayerScore(int score);
    Sprite* addSprite(SpriteType type, int li, int ci);
    void makeEmptyAt(int li, int ci);
    
    void playerSendToTile(QPoint tilePos);
    void printAstar();
    void initAstar();
    void cleanupAstar();
    void loadImages();
    void parseMap(QString* inputStr);
    void loadFromMap(QString mapPath);
    void loadFromRecording(QString savePath);
    void endGame(bool win);
private:
    QString _mapFilePath;
    QSize _mapSize; //In tiles
    int _playerScore = 0;
    int _playerSteps = 0;
    int _ballPoints = 10;
    bool _keyFound = false;
    bool _loggingEnabled = true;
    QTimer* _playerTimer;
    QTimer* _enemiesTimer;
    QTimer* _playerAnimTimer;
    QTimer* _enemiesAnimTimer;
    bool _isPaused = false;
    QTimer* _replayStepTimer;

    MoveDir _playerLastDir = MoveDir::None;
    //bool _replayForward = true;
    bool _replayStepByStep = false;

    QString _saveFilePath{ "saves/save.bin" };
    QPoint _tileClicked{ -1,-1 };
    QPoint _doorPos;
    QString _mapString;
    //QByteArray _recBytes;
    Node** _asMap;
    bool _astarInitialized = false;
    QVector<Enemy*> _enemies{};
    std::unordered_map<SpriteType, QImage> _pixmapCache{};
    int _viewWidth;
    int _maxTilesInRow{ 10 };
    int _tileWidth; //Will be set according on map dimensions
    Sprite*** _map = nullptr;
    Player* _player = nullptr;

    //QFile _saveFile;
    //QDataStream _saveStream;
    
    //QTimer* _replayTimer;
};

#endif // GAMESCENE_H
