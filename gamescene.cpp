#include "gamescene.h"

#include <QFile>
#include <QTextStream>
#include <QtGlobal>

#include "utils.h"
#include "mainwindow.h"
#include "wall.h"

GameScene::GameScene(QObject *parent) : QGraphicsScene(parent)
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

    if (_map[y][x] == nullptr) {
       // pr("Warning: nullptr at " << x << ", " << y);
        return true;
    }

    return _map[y][x]->getType() != SpriteType::Wall;
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
        _mapSize = { width, height };
        _map = new Sprite **[height];
        for (int i = 0; i < height; i++) {
            _map[i] = new Sprite *[width];
            for (int j = 0; j < width; j++)
                _map[i][j] = nullptr;
        }
    }

    QPen tilePen = QPen();
    QBrush wallBrush = Qt::red;
    QBrush borderBrush = Qt::blue;
    QBrush borderBrush1 = Qt::green;
    { //Draw borders
        for (int ci = 0; ci < width + 2; ++ci) { //Top and bottom border
            QRect tileRect{ ci * _tileSize.x(), 0, _tileSize.x(), _tileSize.y() };

            addRect(tileRect, tilePen, borderBrush);
            tileRect = { ci * _tileSize.x(), (height + 1) * _tileSize.y(), _tileSize.x(), _tileSize.y() };
            addRect(tileRect, tilePen, borderBrush);
        }
        for (int li = 1; li < height + 1; ++li) { //Left and right border
            QRect tileRect{ 0, li * _tileSize.y(), _tileSize.x(), _tileSize.y() };
            addRect(tileRect, tilePen, borderBrush1);
            tileRect = { (width + 1) * _tileSize.x(), li * _tileSize.y(), _tileSize.x(), _tileSize.y() };
            addRect(tileRect, tilePen, borderBrush1);
        }
    }

    Wall* tmpWall = nullptr;

    for (int li = 0; !in.atEnd(); ++li) { //Lines
        if (li >= height) {
            errpr("Too many lines in map");
            return false;
        }

        QString line = in.readLine();
        if (line.size() != width) {
            errpr("Too many characters in line");
            return false;
        }
        for (int ci = 0; ci < line.size(); ++ci) { //Columns

            //Add 1 because of border
            QRect tileRect = {(1+ci)*_tileSize.x(), (1+li)*_tileSize.y(), _tileSize.x(), _tileSize.y()};

            char cu = line[ci].unicode();

            switch (cu)
            {
                case 'T':
                case 'K':
                case 'G':
                case '.': //Empty

                    break;
                case 'X': //Wall
                    tmpWall = new Wall(tileRect, tilePen, wallBrush);
                    addItem(tmpWall);
                    _map[li][ci] = tmpWall;

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
