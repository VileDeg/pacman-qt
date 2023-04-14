#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>

#include <unordered_map>

#include "player.h"
#include "enemy.h"
#include "astar.h"
#include "replay.h"

class GameScene : public QGraphicsScene, public ISerializable
{
    Q_OBJECT

private:
    struct GameTimers {
        QTimer* player, *playerAnim, *enemies, *enemiesAnim;

        void stopAll() {
            player->stop();
            playerAnim->stop();
            enemies->stop();
            enemiesAnim->stop();
        }
    };

signals:
    void gameStateChanged(GameState ged);

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
    
    GameState getGameState() { return _state; }

    void playerInteract(int x, int y, bool* win);
    void collideWithEnemy(QPoint playerPos, bool* died);
    bool canMoveTo(int x, int y);
    
    void parseMap(QString* inputStr);
    QString getMapString() { return _mapString; }
    void setMapString(QString mapString) { _mapString = mapString; }
    

    void onKeyPress(QKeyEvent* event);
    void onMousePress(QMouseEvent* event, QPointF localPos);

    void Serialize(QDataStream& stream);
    void Deserialize(QDataStream& stream);
private:
    void setPlayerScore(int score);
    Sprite* addSprite(SpriteType type, int li, int ci);
    void makeEmptyAt(int li, int ci);
    
    void playerSendToTile(QPoint tilePos);
    
    void loadFromMap(QString mapPath);
    void endGame(bool win);
    void setAppearence();
private:
    Astar* _astar = nullptr;

    QString _mapString;
    QSize _mapSize; //In tiles

    int _viewWidth;
    int _tileWidth; //Will be set according on map dimensions

    bool _replay = false;
    GameState _state;
    int _ballPoints = 10;

    qint64 _playerAnimFrame = 0;
    qint64 _enemiesAnimFrame = 0;

    bool _keyFound = false;

    GameTimers _timer;
  
    QPoint _tileClicked{ -1,-1 };
    QPoint _doorPos;
  
    QVector<Enemy*> _enemies{};
    std::unordered_map<SpriteType, QImage>& _pixmapCache;

    

    Sprite*** _map = nullptr;
    Player* _player = nullptr;
};

#endif // GAMESCENE_H
