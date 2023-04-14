/** @file astar.h
 *  @author Vadim Goncearenco <xgonce00@stud.fit.vutbr.cz>
 *  @brief File with A* algorithm class declaration.
 *  @details This file contatins declaration for node struct and A* algorithm class.
 *  This algorithm is used for find the shortest path from player position to mouse click.
 */

#ifndef ASTAR_H
#define ASTAR_H

#include <QSize>
#include <QPoint>

#include <vector>
#include <iostream>

class GameScene;

/**
 * @brief Struct for node in A* algorithm.
 * @details This struct is used for storing information about node in A* algorithm.
 * It contains information about position, g, h, f values, parent node and neighbours.
 */
struct Node {
    int x;
    int y;
    int g; /**< Distance(in tiles) from start. */
    int h; /**< Heuristic(real distance to target). */
    int f; /**< Sum of 'g' and 'h'. */
    bool isWall; /**< Is the node walkable? */
    Node* parent; /**< Node from which this node was opened. */
    std::vector<Node*> nbs;

    Node() : x(0), y(0), g(0), h(0), f(0), isWall(false), parent(nullptr) {}

    friend std::ostream& operator<<(std::ostream& os, const Node& n) {
        os << "(" << n.x << ", " << n.y << ") ";
        return os;
    }
};

/**
 * @brief Class for A* algorithm.
 * @details This class is used for find the shortest path from player position to mouse click.
 * It contains information about map size, scene and map of nodes.
 */
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

#endif // ASTAR_H