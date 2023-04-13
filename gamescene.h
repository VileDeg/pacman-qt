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
#include "interfaces.h"

#include "astar.h"
#include "replay.h"

class GameScene : public QGraphicsScene, public ISerializable
{
    Q_OBJECT
signals:
    //void gameEnded(GameEndData ged);

    void gameStateChanged(GameState ged);
    /*void playerScoreChanged(int score);
    void playerStepsChanged(int steps);*/
    
    void errorOccured(QString message, int code);
private slots:
    void playerHandler();
    void enemiesHandler();

    void playerAnimHandler();
    void enemiesAnimHandler();

    void onPlayerTileOverlapped();
public:
    GameScene(QString filePath, int viewWidth, bool replay, 
        std::unordered_map<SpriteType, QImage>& pixmapCache, QObject *parent = 0);
    ~GameScene();
    
    void playerInteract(int x, int y, bool* win);
    void collideWithEnemy(QPoint playerPos, bool* died);
    void moveSprite(int fromx, int fromy, int tox, int toy);
    bool canMoveTo(int x, int y);
    
    bool _replayUntilNextTile = false;
    
    bool _replay = false;
    void parseMap(QString* inputStr);
    QString _mapString;

    void onKeyPress(QKeyEvent* event);
    void onMousePress(QMouseEvent* event, QPointF localPos);

    bool _toBeRecorded = true;

    void Serialize(QDataStream& stream);
    void Deserialize(QDataStream& stream);
private:
    void setPlayerScore(int score);
    Sprite* addSprite(SpriteType type, int li, int ci);
    void makeEmptyAt(int li, int ci);
    
    void playerSendToTile(QPoint tilePos);
    
    void loadImages();
    
    void loadFromMap(QString mapPath);
    //void loadFromRecording(QString savePath);
    void endGame(bool win);
    void setAppearence();
private:
    Astar* _astar = nullptr;
    QString _mapFilePath;
    QSize _mapSize; //In tiles

    /*int _playerScore = 0;
    int _playerSteps = 0;*/
    GameState _state;
    int _ballPoints = 10;

    uint64_t _playerAnimFrame = 0;
    uint64_t _enemiesAnimFrame = 0;
    bool _keyFound = false;

    struct {
        QTimer *player, *playerAnim, *enemies, *enemiesAnim;

        void stopAll() {
            player->stop();
            playerAnim->stop();
            enemies->stop();
            enemiesAnim->stop();
        }
    } _timer;
  
    QPoint _tileClicked{ -1,-1 };
    QPoint _doorPos;
  
    QVector<Enemy*> _enemies{};
    std::unordered_map<SpriteType, QImage>& _pixmapCache;
    int _viewWidth;
    //int _maxTilesInRow{ 10 };
    int _tileWidth; //Will be set according on map dimensions
    Sprite*** _map = nullptr;
    Player* _player = nullptr;
};

#endif // GAMESCENE_H
