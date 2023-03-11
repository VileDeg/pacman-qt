#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QPair>
#include <unordered_map>

#include "player.h"
#include "enemy.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT
signals:
    //void playerWin();
    void gameEnd(bool win, int score);
//public slots:
//    void setSpriteInMap(Sprite* sprite, int x, int y) { _map[x][y] = sprite; }
private slots:
    void playerHandler();
    void enemiesHandler();
public:
    GameScene(QString mapPath, int viewWidth, QObject *parent = 0);
    ~GameScene();
    
    void playerInteract(int x, int y);
    void collideWithEnemy(QPoint playerPos, bool* died);
    void moveSprite(int fromx, int fromy, int tox, int toy);
    int getScore() { return _playerScore; }
    bool canMoveTo(int x, int y);

    //void setPlayerMoveDir(MoveDir dir) { _player->setMoveDir(dir); }
    void onKeyPress(QKeyEvent* event);

private:
    //void addToScene(Sprite* sprite, int li, int ci);
    Sprite* addSprite(SpriteType type, int li, int ci);
    void makeEmptyAt(int li, int ci);
    
    void addWall(int li, int ci);
    void loadImages();
    void loadFromMap(QString mapPath);
private:
    QTimer* _playerTimer;
    QSize _mapSize; //In tiles
    QPen _wallPen;
    QBrush _wallBrush;
    int _playerScore = 0;
    int _ballPoints = 10;
    bool _keyFound = false;
    QPoint _doorPos;

    QVector<Enemy*> _enemies{};
    std::unordered_map<SpriteType, QImage> _pixmapCache{};
    int _viewWidth;
    int _maxTilesInRow{ 10 };
    int _tileWidth; //Will be set according on map dimensions
    float _scaleFactor{1.f};
    Sprite*** _map = nullptr;
    Player* _player = nullptr;
};

#endif // GAMESCENE_H
