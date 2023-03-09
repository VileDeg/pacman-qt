#include "gamescene.h"

#include <QFile>
#include <QTextStream>
#include <QtGlobal>

#include "utils.h"
#include "mainwindow.h"
#include "wall.h"

GameScene::GameScene(int viewWidth, QObject *parent)
    : QGraphicsScene(parent) , _viewWidth(viewWidth), _wallBrush(Qt::red)
{
    setBackgroundBrush(Qt::black);

    _intervals["player"] = 5;

    _playerTimer = new QTimer(this);
    connect(_playerTimer, SIGNAL(timeout()), this, SLOT(playerHandler()));
    _playerTimer->start(_intervals["player"]);

    try {
        _pixmapCache[SpriteType::Wall] = loadPixmap(":/sprites/object/wall.png");
        _pixmapCache[SpriteType::Ball] = loadPixmap(":/sprites/object/dot.png");
        _pixmapCache[SpriteType::Key]  = loadPixmap(":/sprites/object/key.png");
        _pixmapCache[SpriteType::Lock] = loadPixmap(":/sprites/object/lock.png");
        _pixmapCache[SpriteType::Door] = loadPixmap(":/sprites/object/door.png");
    } catch (std::runtime_error& e) {
        errpr(e.what());
    }
}

GameScene::~GameScene() 
{
    for (int i = 0; i < _mapSize.height(); ++i) {
        for (int j = 0; j < _mapSize.width(); ++j) {
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

void GameScene::interactAt(int x, int y)
{
    auto sprite = _map[x][y];
    assert(sprite);
    
    switch (sprite->getType())
    {
    case SpriteType::Ball:
        _playerScore += _ballPoints;
        removeSprite(x, y);
        break;
    case SpriteType::Key:
        removeSprite(x, y); //Key -> Empty
        removeSprite(_doorPos.x(), _doorPos.y()); //Lock -> Empty
        addSprite(SpriteType::Door, _doorPos.x(), _doorPos.y()); //Empty -> Door
        break;
    case SpriteType::Door:
        playerWin();
        removeSprite(x, y);
        break;
    default:
        break;
    }
}

QRect GameScene::tileRect(int li, int ci)
{
    return QRect(ci * _tileWidth, li * _tileWidth, _tileWidth, _tileWidth);
}

Sprite* GameScene::addSprite(SpriteType type, int li, int ci)
{
    Sprite* tmp = new Sprite(type, tileRect(li, ci));
    addItem(tmp);
    _map[li][ci] = tmp;
    tmp->setImage(&_pixmapCache[type]);
    return tmp;
}

void GameScene::removeSprite(int x, int y)
{
    auto sprite = _map[x][y];
    assert(sprite != nullptr);
    removeItem(sprite);
    delete sprite;
    addSprite(SpriteType::Empty, x, y);
}

void GameScene::addWall(int li, int ci)
{
    auto tmp = addSprite(SpriteType::Wall, li, ci);
    tmp->setBrush(_wallBrush);
    tmp->setPen(_wallPen);
}

bool GameScene::loadMap(QString mapPath)
{
    QFile f(mapPath);
    if (!f.open(QIODevice::ReadOnly)) {
        errpr("Could not open file: " << mapPath);
        return false;
    }

    QTextStream in(&f);

    QString dimLine = in.readLine();
    QStringList tks = dimLine.split(" ");
    if (tks.size() != 2) {
        errpr("Invalid map dimensions(too many values)");
        f.close();
        return false;
    }
    int width{}, height{};
    try {
        width = std::stoi(tks[0].toStdString());
        height = std::stoi(tks[1].toStdString());
    }
    catch (std::exception) {
        errpr("Invalid map dimensions(failed to convert)");
        f.close();
        return false;
    }
    vpr(width); vpr(height);
    int wFull = width + 2;
    int hFull = height + 2;

    _tileWidth = (float)_viewWidth / std::max(wFull, hFull);
    { //Allocate map array
        _mapSize = { wFull, hFull };
        _map = new Sprite **[_mapSize.height()];
        for (int i = 0; i < _mapSize.height(); i++) {
            _map[i] = new Sprite *[_mapSize.width()];
            for (int j = 0; j < _mapSize.width(); j++)
                _map[i][j] = nullptr;
        }
    }
    
    { //Draw borders
        _wallBrush = Qt::blue;
        for (int ci = 0; ci < width + 2; ++ci) { //Top and bottom border
            addWall(0, ci);
            addWall(height+1, ci);
        }
        _wallBrush = Qt::green;
        for (int li = 1; li < height + 1; ++li) { //Left and right border
            addWall(li, 0);
            addWall(li, width+1);
        }
    }
    _wallBrush = Qt::red;

    bool targetInMap = false;
    bool playerInMap = false;
    bool keyInMap = false;
    for (int li = 1; li < _mapSize.height()-1; ++li) { //Lines
        if (in.atEnd()) {
            errpr("Too few lines in map");
            f.close();
            return false;
        }

        QString line = in.readLine();
        if (line.size() != width) {
            errpr("Too many characters in line");
            f.close();
            return false;
        }

        for (int ci = 1; ci < _mapSize.width()-1; ++ci) { //Columns

            char cu = line[ci-1].unicode();

            Sprite* tmp = nullptr;
            switch (cu)
            {
                case 'T': //Target(door)
                    _doorPos = { li, ci };
                    targetInMap = true;
                    break;
                case 'K': //Key
                    addSprite(SpriteType::Key, li, ci);
                    keyInMap = true;
                    break;
                case 'G': //Ghost
                    addSprite(SpriteType::Empty, li, ci);
                    break;
                case '.': //Empty
                    addSprite(SpriteType::Ball, li, ci);
                    break;
                case 'X': //Wall
                    addWall(li, ci);
                    break;
                case 'S': //Start(player)
                    //pr("player found at: " << li << ", " << ci);
                    _player = new Player(tileRect(li, ci), this);
                    _player->setScene(this);
                    _player->setPen({ Qt::red, 2, Qt::SolidLine });
                    addItem(_player);
                    _map[li][ci] = _player;
                    playerInMap = true;
                    break;
                default:
                    errpr("Invalid symbol '" << cu << "' in map");
                    f.close();
                    return false;
            }
        }
    }

    if (!targetInMap || !playerInMap) {
        QString errmsg = "";
        errmsg += !targetInMap ? "target not found in map" : "";
        errmsg += !playerInMap ? "player not found in map" : "";
        errpr(errmsg);
        return false;
    }
    if (keyInMap) { //If there is key in map, door is closed by default
        addSprite(SpriteType::Lock, _doorPos.x(), _doorPos.y());
    } else { //Else it is open
        addSprite(SpriteType::Door, _doorPos.x(), _doorPos.y());
    }

    f.close();
    return true;
}
