#include "gamescene.h"

#include <QFile>
#include <QTextStream>
#include <QtGlobal>
#include <QKeyEvent>

#include "utils.h"
#include "mainwindow.h"

#include <deque>

void GameScene::loadImages()
{
    try {
        //_pixmapCache[SpriteType::Empty] = loadPixmap(":/sprites/object/floor.png");
    
        QImage floor = loadPixmap(":/sprites/object/floor.jpg");
        setImageBrightness(floor, 20);
        _pixmapCache[SpriteType::Background] = floor;

        TileData t = {0, 0, _viewWidth};
        Sprite* bg = new Sprite(SpriteType::Empty, t, this);
        bg->setImage(&_pixmapCache[SpriteType::Background]);
        addItem(bg);
        bg->setZValue(-1);
       
        _pixmapCache[SpriteType::Empty] = QImage(); //floorDark
        //bg->setImage(&_pixmapCache[SpriteType::Empty]);
        _pixmapCache[SpriteType::Wall] = loadPixmap(":/sprites/object/wall.png");
        _pixmapCache[SpriteType::Ball] = loadPixmap(":/sprites/object/dot.png");
        _pixmapCache[SpriteType::Key]  = loadPixmap(":/sprites/object/key.png");
        _pixmapCache[SpriteType::Lock] = loadPixmap(":/sprites/object/lock.png");
        QImage door = loadPixmap(":/sprites/object/door.png");
        setImageBrightness(door, 70);
        _pixmapCache[SpriteType::Door] = door;

    } catch (std::runtime_error& e) {
        errpr(e.what());
    }
}

GameScene::GameScene(QString mapPath, int viewWidth, QObject *parent)
    : QGraphicsScene(parent) , _viewWidth(viewWidth)
{
    setBackgroundBrush(Qt::magenta);
       
    loadImages();

    try {
        loadFromMap(mapPath);
    }
    catch (std::runtime_error& e) {
        throw std::runtime_error("Filed to load map '" + mapPath.toStdString() + "' " + e.what());
    }

    //initAstar();
    //findPath(QPoint(1, 9), QPoint(10, 1));
   
    int interval = 150 / _tileWidth; //Adjust the speed according to tile size

    auto playerTimer = new QTimer(this);
    connect(playerTimer, SIGNAL(timeout()), this, SLOT(playerHandler()));
    playerTimer->start(interval);

    auto ghostTimer = new QTimer(this);
    connect(ghostTimer, SIGNAL(timeout()), this, SLOT(enemiesHandler()));
    ghostTimer->start(interval*4);
}

GameScene::~GameScene() 
{
    for (int i = 0; i < _mapSize.width(); ++i) {
        for (int j = 0; j < _mapSize.height(); ++j) {
            delete _map[i][j]; // Delete sprite
        }
        delete[] _map[i]; // Delete row
    }
    delete[] _map; // Delete map

    cleanupAstar();
}

void GameScene::playerHandler()
{
    _player->action(); //Move player
}

void GameScene::enemiesHandler()
{
    //create enemies list
    /*QVector<Enemy*> enemies;
    for (int i = 0; i < _mapSize.height(); ++i) {
        for (int j = 0; j < _mapSize.width(); ++j) {
            if (_map[i][j]->getType() == SpriteType::Enemy) {
                enemies.push_back(dynamic_cast<Enemy*>(_map[i][j]));
                assert(enemies.back());
            }
        }
    }*/

    for (auto enemy : _enemies) {
        enemy->action();
    }
}

bool GameScene::canMoveTo(int x, int y)
{
    if (x < 0 || y < 0 || x >= _mapSize.width() || y >= _mapSize.height())
        return false;
    assert(_map[x][y]);

    switch (_map[x][y]->getType()) {
        case SpriteType::Wall:
        case SpriteType::Lock:
            return false;
    }

    return true;
}

void GameScene::cleanupAstar()
{
    for (int i = 0; i < _mapSize.width(); ++i) {
        delete[] _asMap[i];
    }
    delete[] _asMap;
}

void GameScene::initAstar()
{
    _asMap = new Node*[_mapSize.width()];
    memset(_asMap, 0, sizeof(Node*) * _mapSize.width());
    for (int i = 0; i < _mapSize.width(); ++i) {
        _asMap[i] = new Node[_mapSize.height()];
        memset(_asMap[i], 0, sizeof(Node) * _mapSize.height());

        for (int j = 0; j < _mapSize.height(); ++j) {
            _asMap[i][j] = {};
            _asMap[i][j].x = i;
            _asMap[i][j].y = j;
            _asMap[i][j].isWall = _map[i][j]->getType() == SpriteType::Wall;
            _asMap[i][j].nbs.resize(4);
        }
    }

    int dimx = _mapSize.width();
    int dimy = _mapSize.height();
    for (std::size_t r = 0; r < _mapSize.width(); ++r) {
        for (std::size_t c = 0; c < _mapSize.height(); ++c) {
            Node& curr = _asMap[r][c];

            if (r > 0) {
                /*if (c > 0) {
                    curr.nbs[0] = &_asMap[r - 1][c - 1];
                }*/
                curr.nbs[0] = &_asMap[r - 1][c];
                /*if (c < dimy - 1) {
                    curr.nbs[2] = &_asMap[r - 1][c + 1];
                }*/
            }

            if (c > 0) {
                curr.nbs[1] = &_asMap[r][c - 1];
            }
            if (c < dimy - 1) {
                curr.nbs[2] = &_asMap[r][c + 1];
            }

            if (r < dimx - 1) {
                /*if (c > 0) {
                    curr.nbs[5] = &_asMap[r + 1][c - 1];
                }*/
                curr.nbs[3] = &_asMap[r + 1][c];
                /*if (c < dimy-1) {
                    curr.nbs[7] = &_asMap[r + 1][c + 1];
                }*/
            }
        }
    }
    _astarInitialized = true;
    //printAstar();
}

void GameScene::printAstar()
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

std::vector<QPoint> GameScene::findPath(QPoint start, QPoint end)
{
    if (_astarInitialized) {
        cleanupAstar();
    }
    initAstar();
    static int cnt = 0;

    ++cnt;

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

void GameScene::playerInteract(int x, int y)
{
    Sprite* target = _map[x][y];
    assert(target);
    
    switch (target->getType())
    {
    case SpriteType::Ball:
        makeEmptyAt(x, y);
        _playerScore += _ballPoints;
        break;
    case SpriteType::Key:
        makeEmptyAt(x, y); //Key -> Empty
        makeEmptyAt(_doorPos.x(), _doorPos.y()); //Lock -> Empty
        addSprite(SpriteType::Door, _doorPos.x(), _doorPos.y()); //Empty -> Door
        break;
    case SpriteType::Door:
        makeEmptyAt(x, y);
        gameEnd(true, _playerScore);
        break;

    default:
        break;
    }
}

void GameScene::collideWithEnemy(QPoint playerPos, bool * died)
{
    //Check for collision with enemies
    for (auto enemy : _enemies) {

        float dst = QVector2D(enemy->pos()).distanceToPoint(QVector2D(playerPos));
        if (dst < _tileWidth/2) {
            gameEnd(false, _playerScore);
            *died = true;
            break; //Without break will crash!
        }
    }
}

void GameScene::moveSprite(int fromx, int fromy, int tox, int toy)
{
    assert(_map[fromx][fromy]);
    assert(_map[tox][toy]);

    auto tmp = _map[fromx][fromy];
    _map[fromx][fromy] = _map[tox][toy];
    _map[tox][toy] = tmp;

}

Sprite* GameScene::addSprite(SpriteType type, int ci, int li)
{
    TileData t{ ci, li, _tileWidth };
    Sprite* tmp = new Sprite(type, t);
    addItem(tmp);
    _map[ci][li] = tmp;
    tmp->setImage(&_pixmapCache[type]);
    return tmp;
}

void GameScene::makeEmptyAt(int x, int y)
{
    auto sprite = _map[x][y];
    assert(sprite != nullptr);
    removeItem(sprite);
    delete sprite;
    addSprite(SpriteType::Empty, x, y);
}

void GameScene::loadFromMap(QString mapPath)
{
    QFile f(mapPath);
    if (!f.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Could not open file");
    }

    QTextStream in(&f);

    QString dimLine = in.readLine();
    QStringList tks = dimLine.split(" ");
    if (tks.size() != 2) {
        throw std::runtime_error("Invalid map dimensions(too many values)");
    }
    int width{}, height{};
    try {
        height = std::stoi(tks[0].toStdString());
        width = std::stoi(tks[1].toStdString());
    }
    catch (std::exception) {
        throw std::runtime_error("Invalid map dimensions(failed to convert)");
    }
    vpr(width); vpr(height);
    int wFull = width + 2;
    int hFull = height + 2;

    _tileWidth = (float)_viewWidth / std::max(wFull, hFull);

    { //Allocate map array
        _mapSize = { wFull, hFull };
        _map = new Sprite **[_mapSize.width()];
        for (int i = 0; i < _mapSize.width(); i++) {
            _map[i] = new Sprite *[_mapSize.height()];
            for (int j = 0; j < _mapSize.height(); j++)
                _map[i][j] = nullptr;
        }

        
    }
    
    { //Draw borders
        for (int ci = 0; ci < width + 2; ++ci) { //Top and bottom border
            addSprite(SpriteType::Wall, ci, 0);
            addSprite(SpriteType::Wall, ci, height+1);
        }
        for (int li = 1; li < height + 1; ++li) { //Left and right border
            addSprite(SpriteType::Wall, 0, li);
            addSprite(SpriteType::Wall, width+1, li);
        }
    }

    bool targetInMap = false;
    bool playerInMap = false;
    bool keyInMap = false;
    for (int li = 1; li < _mapSize.height()-1; ++li) { //Lines
        if (in.atEnd()) {
            throw std::runtime_error("Too few lines in map");
        }

        QString line = in.readLine();
        if (line.size() != width) {
            throw std::runtime_error("Too many characters in line");
        }

        for (int ci = 1; ci < _mapSize.width()-1; ++ci) { //Columns

            char cu = line[ci-1].unicode();

            Enemy* enemy = nullptr;
            Sprite* tmp = nullptr;
            TileData t{ci, li, _tileWidth};

            switch (cu)
            {
                case 'T': //Target(door)
                    _doorPos = { ci, li };
                    targetInMap = true;
                    pr("Door pos: " << ci << " " << li);
                    break;
                case 'K': //Key
                    addSprite(SpriteType::Key, ci, li);
                    keyInMap = true;
                    break;
                case 'G': //Ghost
                    addSprite(SpriteType::Empty, ci, li);

                    enemy = new Enemy(t, this);
                    enemy->setZValue(2);
                    //addToScene(enemy, ci, li);
                    addItem(enemy);
                    _enemies.append(enemy);
                    break;
                case '.': //Empty
                    addSprite(SpriteType::Ball, ci, li);
                    break;
                case 'X': //Wall
                    addSprite(SpriteType::Wall, ci, li);
                    break;
                case 'S': //Start(player)
                    pr("Player pos: " << ci << " " << li);
                    _player = new Player(t, this);
                    _player->setScene(this);
                    _player->setZValue(2);
                    addItem(_player);
                    _map[ci][li] = _player;
                    //addToScene(_player, ci, li);
                    playerInMap = true;
                    break;
                default:
                    throw std::runtime_error("Invalid symbol '" + cu + std::string("' in map"));
            }
        }
    }

    if (!targetInMap || !playerInMap) {
        std::string errmsg = "";
        errmsg += !targetInMap ? "target not found in map" : "";
        errmsg += !playerInMap ? "player not found in map" : "";
        throw std::runtime_error(errmsg);
    }
    if (keyInMap) { //If there is key in map, door is closed by default
        addSprite(SpriteType::Lock, _doorPos.x(), _doorPos.y());
    } else { //Else it is open
        addSprite(SpriteType::Door, _doorPos.x(), _doorPos.y());
    }

    f.close();
}

void GameScene::onKeyPress(QKeyEvent* event)
{
    if (!_player) return;
    if (event->key() == Qt::Key_W) {
        _player->_target = {-1,-1}; //Clear mouse target
        _player->setMoveDir(MoveDir::Up);
    } else if (event->key() == Qt::Key_A) {
        _player->_target = {-1,-1}; //Clear mouse target
        _player->setMoveDir(MoveDir::Left);
    } else if (event->key() == Qt::Key_S) {
        _player->_target = {-1,-1}; //Clear mouse target
        _player->setMoveDir(MoveDir::Down);
    } else if (event->key() == Qt::Key_D) {
        _player->_target = {-1,-1}; //Clear mouse target
        _player->setMoveDir(MoveDir::Right);
    }
}

void GameScene::onMousePress(QMouseEvent* event, QPointF localPos)
{
    if (!_player) return;

    if (event->button() == Qt::LeftButton) {
        QPoint tPos(localPos.x() / _tileWidth, localPos.y() / _tileWidth);
        _player->_path = findPath(_player->getTilePos(), tPos);
        _player->_path.erase(_player->_path.begin()); //Remove starting point
        _player->setPathTo(localPos);
    }
}
