/** @file mainwindow.cpp
 *  @author Vadim Goncearenco <xgonce00@stud.fit.vutbr.cz>
 *  @brief File with window class definition.
 *  @details This file contains main window class definition.
 */

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
#include "utils.h"
#include "ui.h"

#define CONNECT(_sender, _signal, _receiver, _slot) connect(_sender, SIGNAL(_signal), _receiver, SLOT(_slot))

void MainWindow::onDeserializationEnded(GameState)
{
    sceneEnd();
}

MainWindow::MainWindow(ErrorCallback errorCallback, QWidget *parent) :
    QMainWindow(parent), _errorCallback(errorCallback)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    pal.setColor(QPalette::WindowText, Qt::white);
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
    pr("Main window cleanup");
    
    _scene->deleteLater();
    _scene = nullptr;
   
    _cleanupNeeded = false;
}


void MainWindow::onGameStateChanged(GameState gs)
{
    if (gs.gameOver && gs.gameOver != _state.gameOver) {
        sceneEnd();
    }
    _state = gs;
}

void MainWindow::sceneEnd()
{
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
    QString path = "sprites/object";
    try {
        QImage floor = loadPixmap(path + "/floor.jpg");
        setImageBrightness(floor, 20);
        _pixmapCache[SpriteType::Background] = floor;
        
        _pixmapCache[SpriteType::Empty] = QImage();
        _pixmapCache[SpriteType::Wall] = loadPixmap(path + "/wall.png");
        _pixmapCache[SpriteType::Ball] = loadPixmap(path + "/dot.png");
        _pixmapCache[SpriteType::Key] = loadPixmap(path + "/key.png");
        _pixmapCache[SpriteType::Lock] = loadPixmap(path + "/lock.png");

        QImage door = loadPixmap(path + "/door.png");
        setImageBrightness(door, 70);
        _pixmapCache[SpriteType::Door] = door;
    }
    catch (std::runtime_error& e) {
        errpr(e.what());
    }
}

