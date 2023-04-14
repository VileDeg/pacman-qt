/** @file gamescene.h
 *  @author Vadim Goncearenco <xgonce00@stud.fit.vutbr.cz>
 *  @brief File with game scene class declaration.
 *  @details This file contains declaration for game scene class.
 *  Game scene implements core game functionaly and keyboard/mouse input.
 *  It is also responsible for loading and parsing the game map.
 */

#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>

#include <unordered_map>

#include "player.h"
#include "enemy.h"
#include "astar.h"
#include "replay.h"

/**
 * @brief Game scene class.
 * @details This class is responsible for game logic, 
 * keyboard/mouse input and loading and parsing of the game map.
 */
class GameScene : public QGraphicsScene, public ISerializable
{
    Q_OBJECT

private:
    /**
     * @brief Structure with all scene timers.
     * @details This structure contains all timers used for controling game events,
     * such as player movement/animation, enemy movement/animation.
     */
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
    void playerHandler(); /**< Player movement and interaction handler. */
    void enemiesHandler(); /**< Enemies movement and interaction handler. */

    void playerAnimHandler();
    void enemiesAnimHandler();

    void onPlayerTileOverlapped();
public:
    GameScene(QString filePath, int viewWidth, bool replay, 
        std::unordered_map<SpriteType, QImage>& pixmapCache, QObject *parent = 0);
    ~GameScene();
    
    GameState getGameState() { return _state; }

    void playerInteract(int x, int y, bool* win); /**< Player interaction with map. */
    void collideWithEnemy(QPoint playerPos, bool* died);
    bool canMoveTo(int x, int y); /**< Check if player can move to tile. */

    void parseMap(QString* inputStr);
    QString getMapString() { return _mapString; }
    void setMapString(QString mapString) { _mapString = mapString; }

    void onKeyPress(QKeyEvent* event);
    void onMousePress(QMouseEvent* event, QPointF localPos);

    void Serialize(QDataStream& stream);
    void Deserialize(QDataStream& stream);
private:
    void setPlayerScore(int score);
    Sprite* addSprite(SpriteType type, int li, int ci); /**< Add sprite to map. */
    void makeEmptyAt(int li, int ci); /**< Make tile empty. */
     
    void playerSendToTile(QPoint tilePos);
    
    void loadFromMap(QString mapPath);
    void endGame(bool win);
    void setAppearence();
private:
    Astar* _astar = nullptr; /**< Astar object for pathfinding. */

    QString _mapString = ""; /**< Map text contents. */
    QSize _mapSize = {0,0}; /**< Map size (in tiles) */

    int _viewWidth = 0;
    int _tileWidth = 0; /**< Is set according to map dimensions */

    bool _replay = false;
    GameState _state;
    int _ballPoints = 10; /**< Score points for collecting a ball. */

    qint64 _playerAnimFrame = 0;
    qint64 _enemiesAnimFrame = 0;

    bool _keyFound = false;

    GameTimers _timer = {}; /**< Player and enemy timers. */
  
    QPoint _tileClicked{ -1,-1 }; /**< Tile clicked by mouse. */
    QPoint _doorPos; /**< Door position. */
  
    QVector<Enemy*> _enemies{};
    std::unordered_map<SpriteType, QImage>& _pixmapCache; /**< Cache of all loaded images (passed from main window). */

    Sprite*** _map = nullptr; /**< Map of sprites (doesn't contain player and enemies). */
    Player* _player = nullptr;
};

#endif // GAMESCENE_H
