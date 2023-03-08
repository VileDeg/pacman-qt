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
public:
    GameScene(QObject *parent = 0);
    ~GameScene();

    bool canMoveTo(int x, int y);
    void setPlayerMoveDir(MoveDir dir) { _player->setMoveDir(dir); }
    bool loadMap(QString mapPath);
private slots:
    void playerHandler();
private:
    std::unordered_map<QString, int> _intervals{};
    QTimer* _playerTimer;
    QSize _mapSize;
    
    QPoint _tileSize{62,62};
    float _scaleFactor{1.f};
    Sprite*** _map = nullptr;
    Player* _player = nullptr;
};

#endif // GAMESCENE_H
