/** @file player.h
 *  @author Vadim Goncearenco <xgonce00@stud.fit.vutbr.cz>
 *  @brief File with player class declaration.
 *  @details This file contains player class declaration.
 *  Player derives fron animated sprite class, 
 *  and is responsible for player movement and interaction with the game scene.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"
#include "utils.h"

/**
 * @brief Player class.
 * @details Controls player movement and interaction with the game scene.
 */
class Player : public AnimatedSprite
{
    Q_OBJECT
signals:
    void tileOverlapped();

public:
    explicit Player(TileData t, GameScene* parent);
    ~Player() {}

    void action() override;
    void onTileOverlap() override;
    void setMoveDir(MoveDir dir) override;

    void setMouseClickPath(std::vector<QPoint> path); /**< Sets the path to the destination point. */
    bool getTileOverlapped() { return _tileOverlapped; }

    QPoint getTilePos() {return QPoint(_t.x, _t.y);}

private:
    void loadAnimationFrames() override;

    void setDirTo(QPoint to); /**< Sets the next direction based on the position of next tile. */

private:        
    bool _tileOverlapped = false;
    bool _goToMouse = false;

    std::vector<QPoint> _path; /**< Sequence of tile coordinates needed to reach the destination point. */
};

#endif // PLAYER_H
