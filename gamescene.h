#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QPair>
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
    std::vector<QPoint> findPath(QPoint start, QPoint end);

    void onKeyPress(QKeyEvent* event);
    void onMousePress(QMouseEvent* event, QPointF localPos);
private:
    Sprite* addSprite(SpriteType type, int li, int ci);
    void makeEmptyAt(int li, int ci);
    
    void printAstar();
    void initAstar();
    void cleanupAstar();
    void loadImages();
    void loadFromMap(QString mapPath);
private:
    QSize _mapSize; //In tiles
    int _playerScore = 0;
    int _ballPoints = 10;
    bool _keyFound = false;
    QPoint _doorPos;
    Node** _asMap;
    bool _astarInitialized = false;
    QVector<Enemy*> _enemies{};
    std::unordered_map<SpriteType, QImage> _pixmapCache{};
    int _viewWidth;
    int _maxTilesInRow{ 10 };
    int _tileWidth; //Will be set according on map dimensions
    //float _scaleFactor{1.f};
    Sprite*** _map = nullptr;
    Player* _player = nullptr;
};

#endif // GAMESCENE_H
