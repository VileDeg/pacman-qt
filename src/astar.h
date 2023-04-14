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
    int x = 0;
    int y = 0;
    int g = 0; /**< Distance(in tiles) from start. */
    int h = 0; /**< Heuristic(real distance to target). */
    int f = 0; /**< Sum of 'g' and 'h'. */
    bool isWall = false; /**< Is the node walkable? */
    Node* parent = nullptr; /**< Node from which this node was opened. */
    std::vector<Node*> nbs = {}; /**< Neighbours of this node. */

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

    /**
     * @brief Find path from player to mouse click position.
     * 
     * @param start Player position.
     * @param end Mouse click position.
     * @return std::vector<QPoint> 
     */
    std::vector<QPoint> findPath(QPoint start, QPoint end);

    void print();
private:
    void init();
    void cleanup();

private:
    bool _initialized = true;

    GameScene* _scene = nullptr;
    Node** _asMap = nullptr; /**< Map of nodes */
    QSize _mapSize = {0,0}; /**< Map size (in tiles) */
};

#endif // ASTAR_H