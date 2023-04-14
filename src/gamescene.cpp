#include "gamescene.h"

#include <QKeyEvent>

#include "utils.h"
#include "mainwindow.h"

GameScene::GameScene(QString filePath, int viewWidth, bool replay, 
    std::unordered_map<SpriteType, QImage>& pixmapCache, QObject *parent)
    : QGraphicsScene(parent), 
    _viewWidth(viewWidth), _replay(replay), _pixmapCache(pixmapCache)
{
    setAppearence();

    if (replay) {
        return;
    }

    try {
        loadFromMap(filePath);
    }
    catch (std::runtime_error& e) {
        throw std::runtime_error("Failed to load map '" + filePath.toStdString() + "' " + e.what());
    }

    int interval = 150 / _tileWidth; //Adjust the speed according to tile size
    {
        _timer.player = new QTimer(this);
        connect(_timer.player, SIGNAL(timeout()), this, SLOT(playerHandler()));
        _timer.player->start(interval);

        _timer.playerAnim = new QTimer(this);
        connect(_timer.playerAnim, SIGNAL(timeout()), this, SLOT(playerAnimHandler()));
        _timer.playerAnim->start(25);
    }
    {
        _timer.enemies = new QTimer(this);
        connect(_timer.enemies, SIGNAL(timeout()), this, SLOT(enemiesHandler()));
        _timer.enemies->start(interval * 4);

        _timer.enemiesAnim = new QTimer(this);
        connect(_timer.enemiesAnim, SIGNAL(timeout()), this, SLOT(enemiesAnimHandler()));
        _timer.enemiesAnim->start(100);
    }
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
    if (_tileClicked != QPoint(-1,-1) && _player->getTileOverlapped()) {
        playerSendToTile(_tileClicked);
    }

    _player->action(); //Move player
}

void GameScene::enemiesHandler()
{
    for (auto enemy : _enemies) {
        enemy->action();
    }
}

void GameScene::playerAnimHandler()
{
    _player->setSpriteByFrame(_playerAnimFrame);
    ++_playerAnimFrame;
}

void GameScene::enemiesAnimHandler()
{
    for (auto enemy : _enemies) {
        enemy->setSpriteByFrame(_enemiesAnimFrame);
    }
    ++_enemiesAnimFrame;
}

void GameScene::onPlayerTileOverlapped()
{
    _state.steps++;
    emit gameStateChanged(_state);
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
        default:
            break;
    }

    return true;
}

void GameScene::setPlayerScore(int score)
{
    _state.score = score;
    emit gameStateChanged(_state);
}

void GameScene::playerInteract(int x, int y, bool* end)
{
    Sprite* target = _map[x][y];
    assert(target);
    
    switch (target->getType())
    {
    case SpriteType::Ball:
        makeEmptyAt(x, y);
        setPlayerScore(_state.score + _ballPoints);
        break;
    case SpriteType::Key:
        makeEmptyAt(x, y); //Key -> Empty
        makeEmptyAt(_doorPos.x(), _doorPos.y()); //Lock -> Empty
        addSprite(SpriteType::Door, _doorPos.x(), _doorPos.y()); //Empty -> Door
        break;
    case SpriteType::Door:
        makeEmptyAt(x, y);
        *end = true;
        endGame(true);
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
            endGame(false);
            *died = true;
            break; //Without break will crash!
        }
    }
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
    ASSERT(sprite != nullptr);
    removeItem(sprite);
    delete sprite;
    addSprite(SpriteType::Empty, x, y);
}

void GameScene::parseMap(QString* inputStr)
{
    QTextStream in(inputStr);

    QString dimLine = in.readLine();
    QStringList tks = dimLine.split(" ");
    if (tks.size() != 2) {
        throw std::runtime_error("Invalid map dimensions(wrong number of values). Line is: " + dimLine.toStdString());
    }
    int width{}, height{};
    try {
        height = std::stoi(tks[0].toStdString());
        width = std::stoi(tks[1].toStdString());
    }
    catch (std::exception&) {
        throw std::runtime_error("Invalid map dimensions(failed to convert)");
    }
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
            TileData t{ci, li, _tileWidth};

            size_t seed;
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
                    if (_replay) {
                        //Seed doesn't matter since the game won't be running anyway
                        enemy = new Enemy(t, 0, this);
                    } else {
                        std::random_device rd;
                        enemy = new Enemy(t, rd(), this);
                    }
                    enemy->setZValue(2);
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
                    addSprite(SpriteType::Empty, ci, li);
                    _player = new Player(t, this);
                    _player->setScene(this);
                    _player->setZValue(2);
                    addItem(_player);
                    playerInMap = true;

                    connect(_player, SIGNAL(tileOverlapped()), this, SLOT(onPlayerTileOverlapped()));
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
}

void GameScene::loadFromMap(QString mapPath)
{
    QFile f(mapPath);
    if (!f.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Could not open file");
    }
    QTextStream in(&f);
    _mapString = in.readAll();
    f.close();
    //PRINF("Normal loading: " << _mapString.toStdString());

    parseMap(&_mapString);

    _astar = new Astar(this, _mapSize);
}


void GameScene::endGame(bool win)
{
    _timer.stopAll();

    _state.win = win;
    _state.gameOver = true;
    emit gameStateChanged(_state);
}



void GameScene::onKeyPress(QKeyEvent* event)
{
    if (!_player) {
        return;
    }

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

void GameScene::playerSendToTile(QPoint tilePos)
{
    _player->setMouseClickPath(_astar->findPath(_player->getTilePos(), tilePos));
    _tileClicked = { -1,-1 };
}

void GameScene::onMousePress(QMouseEvent* event, QPointF localPos)
{
    if (!_player) {
        return;
    }

    if (event->button() == Qt::LeftButton) {
        QPoint tPos(localPos.x() / _tileWidth, localPos.y() / _tileWidth);

        if (!_player->getTileOverlapped()) {
            _tileClicked = tPos;
            return;
        }

        playerSendToTile(tPos);
    }
}


void GameScene::Serialize(QDataStream& stream)
{
    for (int i = 0; i < _mapSize.width(); ++i) {
        for (int j = 0; j < _mapSize.height(); ++j) {
            ASSERT(_map[i][j] != nullptr);
            if (_map[i][j]->getType() == SpriteType::Wall) {
                continue;
            }
            _map[i][j]->Serialize(stream);
        }
    }

    stream << _enemiesAnimFrame;
    for (auto& enemy : _enemies) {
        enemy->Serialize(stream);
    }

    stream << _playerAnimFrame;
    _player->Serialize(stream);
}

void GameScene::Deserialize(QDataStream& stream)
{
    for (int i = 0; i < _mapSize.width(); ++i) {
        for (int j = 0; j < _mapSize.height(); ++j) {
            ASSERT(_map[i][j] != nullptr);
            if (_map[i][j]->getType() == SpriteType::Wall) {
                continue;
            }
            _map[i][j]->Deserialize(stream);
            _map[i][j]->setImage(&_pixmapCache[_map[i][j]->getType()]);
            _map[i][j]->update(); // Repaint
        }
    }

    stream >> _enemiesAnimFrame;
    for (auto& enemy : _enemies) {
        enemy->Deserialize(stream);
        enemy->setSpriteByFrame(_enemiesAnimFrame);
    }

    stream >> _playerAnimFrame;
    _player->Deserialize(stream);
    _player->setSpriteByFrame(_playerAnimFrame);
}

void GameScene::setAppearence()
{
    setBackgroundBrush(Qt::magenta);

    TileData t = { 0, 0, _viewWidth };
    Sprite* bg = new Sprite(SpriteType::Empty, t, this);
    bg->setImage(&_pixmapCache[SpriteType::Background]);
    addItem(bg);
    bg->setZValue(-1);
}
