#include "gamescene.h"

#include <QFile>
#include <QTextStream>
#include <QtGlobal>
#include <QKeyEvent>

#include "utils.h"
#include "mainwindow.h"


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
    : QGraphicsScene(parent) , _viewWidth(viewWidth), _wallBrush(Qt::red)
{
    setBackgroundBrush(Qt::magenta);
       
    loadImages();

    try {
        loadFromMap(mapPath);
    }
    catch (std::runtime_error& e) {
        throw std::runtime_error("Filed to load map '" + mapPath.toStdString() + "' " + e.what());
    }

   
    int interval = 150 / _tileWidth; //Adjust the speed according to tile size

    _playerTimer = new QTimer(this);
    connect(_playerTimer, SIGNAL(timeout()), this, SLOT(playerHandler()));
    _playerTimer->start(interval);

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


//void GameScene::addToScene(Sprite* sprite, int ci, int li)
//{
//    addItem(sprite);
//    _map[ci][li] = sprite;
// 
//    //_map[li][ci] = sprite;
//}

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

void GameScene::addWall(int ci, int li)
{
    auto tmp = addSprite(SpriteType::Wall, ci, li);
    tmp->setBrush(_wallBrush);
    tmp->setPen(_wallPen);
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
        width = std::stoi(tks[0].toStdString());
        height = std::stoi(tks[1].toStdString());
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
            addWall(0, ci);
            addWall(height+1, ci);
        }
        for (int li = 1; li < height + 1; ++li) { //Left and right border
            addWall(li, 0);
            addWall(li, width+1);
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
                    //addSprite(SpriteType::Ball, ci, li)->setZValue(1);
                    /*auto tmp  = new Sprite(SpriteType::Empty, t);
                    addItem(tmp);
                    _map[ci][li] = tmp;
                    tmp->setImage(new QPixmap)*/
                    addSprite(SpriteType::Ball, ci, li);
                    break;
                case 'X': //Wall
                    addWall(ci, li);
                    break;
                case 'S': //Start(player)
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
        _player->setMoveDir(MoveDir::Up);
    } else if (event->key() == Qt::Key_A) {
        _player->setMoveDir(MoveDir::Left);
    } else if (event->key() == Qt::Key_S) {
        _player->setMoveDir(MoveDir::Down);
    } else if (event->key() == Qt::Key_D) {
        _player->setMoveDir(MoveDir::Right);
    }
}