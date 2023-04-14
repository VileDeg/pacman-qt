#include "astar.h"
#include "gamescene.h"

#include <QVector2D>

#include <deque>

Astar::Astar(GameScene* scene, QSize mapSize)
    : _scene(scene), _mapSize(mapSize)
{
    init();
}

Astar::~Astar()
{
    if (_initialized) {
        cleanup();
    }
}

void Astar::init()
{
    _asMap = new Node * [_mapSize.width()];
    memset(_asMap, 0, sizeof(Node*) * _mapSize.width());
    for (int i = 0; i < _mapSize.width(); ++i) {
        _asMap[i] = new Node[_mapSize.height()];
        //memset(_asMap[i], 0, sizeof(Node) * _mapSize.height());

        for (int j = 0; j < _mapSize.height(); ++j) {
            _asMap[i][j] = {};
            _asMap[i][j].x = i;
            _asMap[i][j].y = j;
            _asMap[i][j].isWall = !_scene->canMoveTo(i, j);//_map[i][j]->getType() == SpriteType::Wall;
            _asMap[i][j].nbs.resize(4);
        }
    }

    int dimx = _mapSize.width();
    int dimy = _mapSize.height();
    for (int r = 0; r < _mapSize.width(); ++r) {
        for (int c = 0; c < _mapSize.height(); ++c) {
            Node& curr = _asMap[r][c];

            if (r > 0) {
                curr.nbs[0] = &_asMap[r - 1][c];
            }

            if (c > 0) {
                curr.nbs[1] = &_asMap[r][c - 1];
            }
            if (c < dimy - 1) {
                curr.nbs[2] = &_asMap[r][c + 1];
            }

            if (r < dimx - 1) {
                curr.nbs[3] = &_asMap[r + 1][c];
            }
        }
    }
    _initialized = true;
}

std::vector<QPoint> Astar::findPath(QPoint start, QPoint end)
{
    if (!_scene->canMoveTo(end.x(), end.y())) {
        return {};
    }

    if (_initialized) {
        cleanup();
    }
    init();

    //Find shortest path from start to end in _map using A* algorithm
    std::vector<Node*> open{}, closed{};
    open.push_back(&_asMap[start.x()][start.y()]);

    int iteration = 0;
    int maxIterations = 100;

    while (true) {
        if (iteration > maxIterations) {
            std::cout << "Max iterations reached. Path not found." << std::endl;
            break;
        }

        //Find node with lowest F cost in open. If there are multiple nodes with the same F cost, choose the one with the lowest H cost.
        auto it = std::min_element(open.begin(), open.end(),
            [](const Node* a, const Node* b) {
                if (a->f == b->f) {
                    return a->h < b->h;
                } else {
                    return a->f < b->f;
                }
            }
        );

        if (it == open.end()) { //No path found
            std::cout << "Open list is empty. Path not found. Iteration: " << iteration << std::endl;
            break;
        }
        Node* curr = *it;

        //Remove curr from open
        open.erase(it);
        //Add curr to closed
        closed.push_back(curr);
        //If curr is the end node, we are done
        if (curr->x == end.x() && curr->y == end.y()) {
            std::cout << "Path found in " + std::to_string(iteration) + " iterations\n";
            break;
        }

        //For each neighbor of curr
        for (std::size_t i = 0; i < curr->nbs.size(); ++i) {
            Node* nb = curr->nbs[i];
            if (!nb) {
                continue;
            }
            //If neighbor is not walkable or neighbor is in closed, skip to next neighbor
            if (nb->isWall || std::find(closed.begin(), closed.end(), nb) != closed.end()) {
                continue;
            }

            float diff = 1;
            float dstToNb = curr->g + diff;
            //If new path to neighbor is shorter OR neighbor is not in open
            bool inOpen = std::find(open.begin(), open.end(), nb) != open.end();
            if (dstToNb < nb->g || !inOpen) {
                //Set g to new path
                nb->g = dstToNb;
                //Set h to distance from neighbor to end
                auto dst = QVector2D(nb->x, nb->y).distanceToPoint(QVector2D(end.x(), end.y()));
                nb->h = dst;
                //Set f to g + h
                nb->f = nb->g + nb->h;
                //Set parent to curr
                nb->parent = curr;
                //If neighbor is not in open
                if (!inOpen) {
                    //Add neighbor to open
                    open.push_back(nb);
                }
            }
        }

        ++iteration;
    }

    std::deque<Node*> path{};
    Node* curr = &_asMap[end.x()][end.y()];
    while (curr) {
        path.push_front(curr);
        curr = curr->parent;
    }
    std::vector<QPoint> pathPoints{};
    std::cout << "Path: ";
    for (auto node : path) {
        std::cout << *node;
        pathPoints.push_back(QPoint(node->x, node->y));
    }
    std::cout << std::endl;
    return pathPoints;
}

void Astar::cleanup()
{
    for (int i = 0; i < _mapSize.width(); ++i) {
        delete[] _asMap[i];
    }
    delete[] _asMap;
}

void Astar::print()
{
    for (int i = 0; i < _mapSize.width(); ++i) {
        for (int j = 0; j < _mapSize.height(); ++j) {
            std::cout << _asMap[i][j] << "\n\t";
            for (auto nb : _asMap[i][j].nbs) {
                if (nb) {
                    std::cout << *nb;
                } else {
                    std::cout << "NULL ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}