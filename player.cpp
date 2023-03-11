#include "player.h"

#include <QKeyEvent>
#include <QTimer>
#include <QPixmap>

#include "utils.h"
#include "gamescene.h"
#include <map>

void Player::action()
{
    bool died = false;
    _scene->collideWithEnemy(pos().toPoint(), &died);
    if (died) {
        return;
    }

    QPoint p(pos().toPoint()); // Player p local to scene
    QPoint tPos = { p.x() / _t.width, p.y() / _t.width }; //Tile in which the player is 
    QPoint rem = { p.x() % _t.width, p.y() % _t.width };

    if (rem.isNull()) {
        _t.x = tPos.x(); // Update the index of current tile
        _t.y = tPos.y();

        _scene->playerInteract(_t.x, _t.y);
    }

    processMovement(rem);

    if (_target == QPoint{ -1, -1 }) {
        return;
    }

    p = pos().toPoint(); // Player p local to scene
    tPos = { p.x() / _t.width, p.y() / _t.width }; //Tile in which the player is 
    rem = { p.x() % _t.width, p.y() % _t.width };

    if (!rem.isNull()) {
        return;
    }

    _t.x = tPos.x(); // Update the index of current tile
    _t.y = tPos.y();

    if (!_path.empty()) {
        auto next = _path.front();
        setDirTo(next);
        _path.erase(_path.begin());
    } else {
        _nextDir = MoveDir::None;
        _target = { -1,-1 };
    }

    /*std::map<MoveDir, int> dirMap{
        { MoveDir::Up, 0 },
        { MoveDir::Left, 0 },
        { MoveDir::Down, 0 },
        { MoveDir::Right, 0 }
    };

    
    QPoint tdiff = _target - tPos;
    if (tdiff.isNull()) {
        _nextDir = MoveDir::None;
        _target = { -1, -1 };
        return;
    }
    pr("Target: " << _target);
    pr("Player: " << tPos);
    pr("Diff: " << tdiff);
    if (tdiff.x() > 0) {
        dirMap[MoveDir::Right] = tdiff.x();
    } 
    if (tdiff.x() < 0) {
        dirMap[MoveDir::Left] = -tdiff.x();
    } 
    if (tdiff.y() > 0) {
        dirMap[MoveDir::Down] = tdiff.y();
    } 
    if (tdiff.y() < 0) {
        dirMap[MoveDir::Up] = -tdiff.y();
    }

    bool end = false;
    while(!end) {
        auto it = std::max_element(dirMap.begin(), dirMap.end(),
            [](const auto& a, const auto& b) {
                return a.second < b.second;
            });
        _nextDir = it->first;
        dirMap.erase(it);
        if (!_aroundFree[static_cast<int>(_nextDir) - 1]) {
            if (!dirMap.empty()) {
                continue;
            }
            _nextDir = MoveDir::None;
            end = true;
        } else {
            end = true;
        }
    }*/
}

void Player::setPathTo(QPointF mousePos)
{
    int tx = mousePos.x() / _t.width;
    int ty = mousePos.y() / _t.width;
    _target = { tx,ty };
}

void Player::loadAnimationFrames()
{
    try {
        loadAnimationFrame(MoveDir::None, "a1.png");

        loadAnimationFrame(MoveDir::Up, "a1.png");
        loadAnimationFrame(MoveDir::Up, "b2.png");
        loadAnimationFrame(MoveDir::Up, "b3.png");
        loadAnimationFrame(MoveDir::Up, "b4.png");
        loadAnimationFrame(MoveDir::Up, "b5.png");
        loadAnimationFrame(MoveDir::Up, "b6.png");
        loadAnimationFrame(MoveDir::Up, "b5.png");
        loadAnimationFrame(MoveDir::Up, "b4.png");
        loadAnimationFrame(MoveDir::Up, "b3.png");
        loadAnimationFrame(MoveDir::Up, "b2.png");

        loadAnimationFrame(MoveDir::Left, "a1.png");
        loadAnimationFrame(MoveDir::Left, "c2.png");
        loadAnimationFrame(MoveDir::Left, "c3.png");
        loadAnimationFrame(MoveDir::Left, "c4.png");
        loadAnimationFrame(MoveDir::Left, "c5.png");
        loadAnimationFrame(MoveDir::Left, "c6.png");
        loadAnimationFrame(MoveDir::Left, "c5.png");
        loadAnimationFrame(MoveDir::Left, "c4.png");
        loadAnimationFrame(MoveDir::Left, "c3.png");
        loadAnimationFrame(MoveDir::Left, "c2.png");

        loadAnimationFrame(MoveDir::Down, "a1.png");
        loadAnimationFrame(MoveDir::Down, "d2.png");
        loadAnimationFrame(MoveDir::Down, "d3.png");
        loadAnimationFrame(MoveDir::Down, "d4.png");
        loadAnimationFrame(MoveDir::Down, "d5.png");
        loadAnimationFrame(MoveDir::Down, "d6.png");
        loadAnimationFrame(MoveDir::Down, "d5.png");
        loadAnimationFrame(MoveDir::Down, "d4.png");
        loadAnimationFrame(MoveDir::Down, "d3.png");
        loadAnimationFrame(MoveDir::Down, "d2.png");

        loadAnimationFrame(MoveDir::Right, "a1.png");
        loadAnimationFrame(MoveDir::Right, "a2.png");
        loadAnimationFrame(MoveDir::Right, "a3.png");
        loadAnimationFrame(MoveDir::Right, "a4.png");
        loadAnimationFrame(MoveDir::Right, "a5.png");
        loadAnimationFrame(MoveDir::Right, "a6.png");
        loadAnimationFrame(MoveDir::Right, "a5.png");
        loadAnimationFrame(MoveDir::Right, "a4.png");
        loadAnimationFrame(MoveDir::Right, "a3.png");
        loadAnimationFrame(MoveDir::Right, "a2.png");
    }
    catch (std::runtime_error& e) {
        errpr(e.what());
    }
}