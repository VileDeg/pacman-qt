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

#define SER_FRAME_BYTE_SIZE 2945

void MainWindow::onSerialize()
{
    ASSERT(_scene != nullptr);

    bool isLastFrame = false;
    if (_gameEnded) {
        isLastFrame = true;
        _stream << isLastFrame;
        _stream << _gameWon;
        _stream << _scene->getScore();
        _stream << _scene->getSteps();

        _serializationTimer->stop();
        _file.flush();
        _file.close();
        return;
    }

    _stream << isLastFrame;
    
    _scene->Serialize(_stream);
}

void MainWindow::onDeserialize()
{
    ASSERT(_scene != nullptr);

    if (_replayPaused) {
        return;
    }

    
    bool isLastFrame = false;
    _stream >> isLastFrame;

    if (isLastFrame) { //TODO:
        bool gameWon;
        int score, steps;
        _stream >> gameWon;
        _stream >> score;
        _stream >> steps;

        gameEnd(gameWon, score, steps);
        _serializationTimer->stop();
        _file.close();
        return;
    }

    
    if (_replayOneStep) {
        //qint64 pos = _file.pos();
        //pr("DES file pos 0: " << pos);
        qint64 seekPos = _file.pos();
        if (_replayForward) {
            seekPos += _framesPerStep * SER_FRAME_BYTE_SIZE;
            while (seekPos >= _file.size()) {
                seekPos -= SER_FRAME_BYTE_SIZE;
            }
            
        } else {
            seekPos -= _framesPerStep * SER_FRAME_BYTE_SIZE;
            while (seekPos < 0) {
                seekPos += SER_FRAME_BYTE_SIZE;
            }
        }
        ASSERTMSG(seekPos >= 0 && seekPos < _file.size(), "Invalid position in file");
        _file.seek(seekPos);
        //pr("DES file pos 1: " << pos);
        pr("DES paused");
        _replayPaused = true;
    }
    
    _scene->Deserialize(_stream);

    //pr("DES file pos 1: " << _file.pos());

    if (!_replayForward) {
        if (_replayOneStep) {
            qint64 seekPos = _file.pos() - _framesPerStep * SER_FRAME_BYTE_SIZE;
            while (seekPos < 0) {
                seekPos += SER_FRAME_BYTE_SIZE;
            }
            ASSERTMSG(seekPos >= 0 && seekPos < _file.size(), "Invalid position in file");
            _file.seek(seekPos);
        }
    }
}

MainWindow::MainWindow(QApplication* app, QWidget *parent) :
    QMainWindow(parent), _app(app)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setPalette(pal);
    setWindowTitle("Pac-Man");
    int w = _viewWidth + _offsetAroundView;
    _windowDim = {w,w};
    setFixedSize(_windowDim);

    _ui = new WindowUI(this);
    setCentralWidget(_ui);

    loadImages();
}

MainWindow::~MainWindow() {}

void MainWindow::startGame(QString mapPath, bool recorded)
{
    if (_cleanupNeeded) {
        cleanup();
    }

    try {
        _scene = new GameScene(mapPath, _viewWidth, recorded, _pixmapCache, this);
    } catch (std::exception& e) {
        throw std::runtime_error("Error creating scene: " + std::string(e.what()));
    }

    _ui->view->setScene(_scene);

    connect(_scene, SIGNAL(gameEnd(bool, int, int)), this, SLOT(gameEnd(bool, int, int)));

    //connect(_scene, SIGNAL(gamePause(bool)), _ui, SLOT(onGamePause(bool)));
    connect(_scene, SIGNAL(playerScoreChanged(int)), _ui, SLOT(onPlayerScoreChanged(int)));
    connect(_scene, SIGNAL(playerStepsChanged(int)), _ui, SLOT(onPlayerStepsChanged(int)));

    auto w = _scene->sceneRect().width();
    auto h = _scene->sceneRect().height();
    
    _ui->view->setFixedSize(w, h);

    _serializationTimer = new QTimer(this);
    _framesPerStep = _stepInterval / _serializationInterval;
    
    if (!recorded) {
        connect(_serializationTimer, SIGNAL(timeout()), this, SLOT(onSerialize()));
        
        auto name = mapPath.split('/').last().split('.').first();
        _file.setFileName("saves/" + name + ".bin");

        if (!_file.open(QIODevice::WriteOnly)) {
            throw std::runtime_error("Could not open file");
        }
        _file.resize(0);
        _stream.setDevice(&_file);

        _stream << _scene->_mapString;
    } else {
        connect(_serializationTimer, SIGNAL(timeout()), this, SLOT(onDeserialize()));

        auto name = mapPath.split('/').last().split('.').first();
        _file.setFileName("saves/" + name + ".bin");

        if (!_file.open(QIODevice::ReadOnly)) {
            throw std::runtime_error("Could not open file");
        }
        _file.resize(0);
        _stream.setDevice(&_file);

        // Read all data from stream





        _stream >> _scene->_mapString;
        _scene->parseMap(&_scene->_mapString);
    }
    _serializationTimer->start(0);
    _serializationTimer->stop();
    _serializationTimer->start(_serializationInterval);

    _cleanupNeeded = true;
}


void MainWindow::replayStepNext()
{
    _replayPaused = false;
    _replayOneStep = true;

    _replayForward = true;
}

void MainWindow::replayStepBack()
{
    _replayPaused = false;
    _replayOneStep = true;

    _replayForward = false;
}

void MainWindow::cleanup()
{
    std::cout << "Main window cleanup" << std::endl;
    
    _scene->deleteLater();
   
    _cleanupNeeded = false;
}

void MainWindow::gameEnd(bool win, int score, int steps)
{
    if (_cleanupNeeded) {
        cleanup();
    }
    _gameEnded = true;
    _gameWon = win;

    _ui->onGameEnd(win, score, steps);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (!_scene) return;

    _scene->onKeyPress(event);
}

void MainWindow::mousePressEvent(QMouseEvent * event)
{
    if (!_scene) return;

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

