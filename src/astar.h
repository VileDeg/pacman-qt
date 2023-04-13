#pragma once

#include <QSize>
#include <QPoint>

#include <vector>
#include <iostream>

class GameScene;

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

class Astar {
public:
    Astar(GameScene* scene, QSize mapSize);
    ~Astar();

    std::vector<QPoint> findPath(QPoint start, QPoint end);

    void print();
private:
    void init();
    void cleanup();

private:
    bool _initialized = true;

    GameScene* _scene;
    Node** _asMap;
    QSize _mapSize; //In tiles
};
