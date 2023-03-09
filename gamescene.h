#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>

#include <unordered_map>

#include "player.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT
signals:
    void playerWin();
public:
    GameScene(int viewWidth, QObject *parent = 0);
    ~GameScene();
    
    void interactAt(int x, int y);
    int getScore() { return _playerScore; }
    bool canMoveTo(int x, int y);
    void setPlayerMoveDir(MoveDir dir) { _player->setMoveDir(dir); }
    bool loadMap(QString mapPath);
private slots:
    void playerHandler();
private:
    Sprite* addSprite(SpriteType type, int li, int ci);
    void removeSprite(int li, int ci);
    QRect tileRect(int li, int ci);
    
    void addWall(int li, int ci);
private:
    std::unordered_map<QString, int> _intervals{};
    QTimer* _playerTimer;
    QSize _mapSize; //In tiles
    QPen _wallPen;
    QBrush _wallBrush;
    int _playerScore = 0;
    int _ballPoints = 10;
    bool _keyFound = false;
    QPoint _doorPos;
    std::unordered_map<SpriteType, QPixmap> _pixmapCache{};
    int _viewWidth;
    int _maxTilesInRow{ 10 };
    int _tileWidth; //Will be set according on map dimensions
    float _scaleFactor{1.f};
    Sprite*** _map = nullptr;
    Player* _player = nullptr;
};

#endif // GAMESCENE_H
