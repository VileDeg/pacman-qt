#include "gamescene.h"

#include <QFile>
#include <QTextStream>
#include <QtGlobal>

#include "utils.h"
#include "mainwindow.h"
#include "wall.h"

GameScene::GameScene(QObject *parent) 
    : QGraphicsScene(parent) ,_wallPen(), _wallBrush(Qt::red)
{
    setBackgroundBrush(Qt::black);

    _intervals["player"] = 10;

    _playerTimer = new QTimer(this);
    connect(_playerTimer, SIGNAL(timeout()), this, SLOT(playerHandler()));
    _playerTimer->start(_intervals["player"]);
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

    if (_map[x][y] == nullptr) {
       // pr("Warning: nullptr at " << x << ", " << y);
        return true;
    }

    return _map[x][y]->getType() != SpriteType::Wall;
}


void GameScene::addToMap(Sprite* sprite, int li, int ci)
{
    addItem(sprite);
    _map[li][ci] = sprite;
}

void GameScene::addWall(int li, int ci)
{
    Wall* tmp = new Wall(
        QRect(ci*_tileSize.x(), li*_tileSize.y(), _tileSize.x(), _tileSize.y()), 
        _wallPen, _wallBrush);
    addItem(tmp);
    _map[li][ci] = tmp;
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
        errpr("Invalid map dimensions(number of tokens)");
    }
    int width = std::stoi(tks[0].toStdString());
    int height = std::stoi(tks[1].toStdString());
    vpr(width); vpr(height);

    { //Allocate map array
        _mapSize = { width+2, height+2 };
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


    for (int li = 1; li < _mapSize.height()-1; ++li) { //Lines
        if (in.atEnd()) {
            errpr("Too few lines in map");
            return false;
        }

        QString line = in.readLine();
        if (line.size() != width) {
            errpr("Too many characters in line");
            return false;
        }

        for (int ci = 1; ci < _mapSize.width()-1; ++ci) { //Columns

            QRect tileRect = {ci*_tileSize.x(), li*_tileSize.y(), _tileSize.x(), _tileSize.y()};

            char cu = line[ci-1].unicode();

            switch (cu)
            {
                case 'T':
                case 'K':
                case 'G':
                case '.': //Empty

                    break;
                case 'X': //Wall
                    addWall(li, ci);
                    break;
                case 'S':
                    pr("player found at: " << li << ", " << ci);
                    _player = new Player(tileRect);
                    _player->setScene(this);
                    _player->setPen({ Qt::red, 2, Qt::SolidLine });
                    addItem(_player);
                    _map[li][ci] = _player;
                    break;
                default:
                    errpr("Invalid symbol in map");
                    return false;
            }
        }
    }

    f.close();
    return true;
}
