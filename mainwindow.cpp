#include <QGraphicsScene>

#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <QMenuBar>
#include <QDirIterator>
#include <QMouseEvent>
#include <QFile>

#include <cstdlib>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include "ui.h"

//#define SER_FRAME_BYTE_SIZE 2945
//#define SER_GAME_END_DATA (2 * sizeof(bool) + 2 * sizeof(int))

#define CONNECT(_sender, _signal, _receiver, _slot) connect(_sender, SIGNAL(_signal), _receiver, SLOT(_slot))

void MainWindow::onDeserializationEnded(GameState gs)
{
    sceneEnd(gs);
}

//ErrorCallback errorCallback, 
MainWindow::MainWindow(QApplication* app, ErrorCallback errorCallback, QWidget *parent) :
    QMainWindow(parent), _app(app) , _errorCallback(errorCallback)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setPalette(pal);
    setWindowTitle("Pac-Man");
    int w = _viewWidth + _offsetAroundView;
    _windowDim = {w,w};
    setFixedSize(_windowDim);

    _ui = new WindowUI(this);
    CONNECT(&_serializer, replayFlagsChanged(const ReplayFlags&), _ui, onReplayFlagsChanged(const ReplayFlags&));

    setCentralWidget(_ui);

    try {
        loadImages();
    } catch (std::runtime_error& e) {
        _errorCallback("Error loading images: " + QString(e.what()), 1);
        return;
    }
}

MainWindow::~MainWindow() {}

void MainWindow::startGame(QString mapPath, bool recorded, bool replayFromStart)
{
    if (_cleanupNeeded) {
        cleanup();
    }

    _currentlyReplaying = recorded;

    try {
        _scene = new GameScene(mapPath, _viewWidth, recorded, _pixmapCache, this);
    } catch (std::runtime_error& e) {
        _errorCallback("Error loading scene: " + QString(e.what()), 2);
        return;
    }

    _ui->view->setScene(_scene);

    CONNECT(_scene, gameStateChanged(GameState), &_serializer, onGameStateChanged(GameState));
    CONNECT(_scene, gameStateChanged(GameState), &_serializer, onGameStateChanged(GameState));
    CONNECT(_scene, gameStateChanged(GameState), this        , onGameStateChanged(GameState));
    CONNECT(_scene, gameStateChanged(GameState), _ui         , onGameStateChanged(GameState));


    CONNECT(&_serializer, gameStateChanged(GameState), this, onGameStateChanged(GameState));
    CONNECT(&_serializer, gameStateChanged(GameState), _ui, onGameStateChanged(GameState));
    CONNECT(&_serializer, replayFlagsChanged(ReplayFlags), _ui, onReplayFlagsChanged(ReplayFlags));

    auto w = _scene->sceneRect().width();
    auto h = _scene->sceneRect().height();
    
    _ui->view->setFixedSize(w, h);

    try {
        _serializer.Init(_scene, mapPath, recorded, replayFromStart);
    } catch (std::runtime_error& e) {
        _errorCallback("Serialization error: " + QString(e.what()), 3);
        return;
    }

    _cleanupNeeded = true;
}



void MainWindow::cleanup()
{
    std::cout << "Main window cleanup" << std::endl;
    
    _scene->deleteLater();
    _scene = nullptr;
   
    _cleanupNeeded = false;
}


void MainWindow::onGameStateChanged(GameState gs)
{
    if (gs.gameOver && gs.gameOver != _state.gameOver) {
        //serializationEnd(gs);
        sceneEnd(gs);
    }
    _state = gs;
}

void MainWindow::sceneEnd(GameState gs)
{
    //_ui->onGameEnd(gs);

    if (_cleanupNeeded) {
        cleanup();
    }
}



void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (!_scene || _currentlyReplaying) {
        return;
    }

    _scene->onKeyPress(event);
}

void MainWindow::mousePressEvent(QMouseEvent * event)
{
    if (!_scene || _currentlyReplaying) {
        return;
    }

    QPoint origin = _ui->view->mapFromGlobal(QCursor::pos());
    QPointF relativeOrigin = _ui->view->mapToScene(origin);
    _scene->onMousePress(event, relativeOrigin);
}

void MainWindow::loadImages()
{
    try {
        QImage floor = loadPixmap(":/sprites/object/floor.jpg");
        setImageBrightness(floor, 20);
        _pixmapCache[SpriteType::Background] = floor;
        
        _pixmapCache[SpriteType::Empty] = QImage();
        _pixmapCache[SpriteType::Wall] = loadPixmap(":/sprites/object/wall.png");
        _pixmapCache[SpriteType::Ball] = loadPixmap(":/sprites/object/dot.png");
        _pixmapCache[SpriteType::Key] = loadPixmap(":/sprites/object/key.png");
        _pixmapCache[SpriteType::Lock] = loadPixmap(":/sprites/object/lock.png");

        QImage door = loadPixmap(":/sprites/object/door.png");
        setImageBrightness(door, 70);
        _pixmapCache[SpriteType::Door] = door;
    }
    catch (std::runtime_error& e) {
        errpr(e.what());
    }
}

