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
#define SER_GAME_END_DATA (sizeof(GameState) + 1)


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
    connect(this, SIGNAL(replayFlagsChanged(const ReplayFlags&)), _ui, SLOT(onReplayFlagsChanged(const ReplayFlags&)));

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

    connect(_scene, SIGNAL(gameStateChanged(GameState)), this, SLOT(onGameStateChanged(GameState)));
    connect(_scene, SIGNAL(gameStateChanged(GameState)), _ui, SLOT(onGameStateChanged(GameState)));

    //connect(_scene, SIGNAL(gamePause(bool)), _ui, SLOT(onGamePause(bool)));
    //connect(_scene, SIGNAL(gameDataChanged(int)), _ui, SLOT(onGameDataChanged(int)));

    auto w = _scene->sceneRect().width();
    auto h = _scene->sceneRect().height();
    
    _ui->view->setFixedSize(w, h);

    

    _cleanupNeeded = true;
}

void MainWindow::onSerialize()
{
    ASSERT(_scene != nullptr);

    bool isLastFrame = false;
    /*if (_gameEnded) {
        
        return;
    }*/

    _stream << false;

    _scene->Serialize(_stream);
}

void MainWindow::onDeserialize()
{
    ASSERT(_scene != nullptr);

    if (_rf.Paused) {
        return;
    }

    if (_rf.OneStep || _rf.StepByStep) {
        //qint64 pos = _file.pos();
        //pr("DES file pos 0: " << pos);
        qint64 seekPos = _file.pos();
        if (_rf.Forward) {
            seekPos += _framesPerStep * _frameDataSize;
            while (seekPos >= _file.size()) {
                seekPos -= _frameDataSize;
            }

        } else {
            seekPos -= _framesPerStep * _frameDataSize;
            while (seekPos < _filePosFrameDataStart) {
                seekPos += _frameDataSize;
            }
        }
        ASSERTMSG(seekPos >= 0 && seekPos < _file.size(), "Invalid position in file");
        _file.seek(seekPos);
        //pr("DES file pos 1: " << pos);
        pr("DES paused");
        _rf.Paused = true;
        if (_rf.StepByStep) {
            _stepTimer->start(_stepInterval);
        }
    } else if (!_rf.Forward) {
        qint64 seekPos = _file.pos() - _frameDataSize;
        if (seekPos >= _filePosFrameDataStart) {
            ASSERTMSG(seekPos >= 0 && seekPos < _file.size(), "Invalid position in file");
            _file.seek(seekPos);
        }
    }

    bool isLastFrame = false;
    _stream >> isLastFrame;

    if (isLastFrame) { //TODO:
        GameState gs;
        _stream >> gs;

        sceneEnd(gs);

        _serializationTimer->stop();
        _file.flush();
        _file.close();
        return;
    }

    _scene->Deserialize(_stream);

    //pr("DES file pos 1: " << _file.pos());

    if (!_rf.Forward) {
        if (_rf.OneStep) {
            qint64 seekPos = _file.pos() - _framesPerStep * _frameDataSize;
            while (seekPos < _filePosFrameDataStart) {
                seekPos += _frameDataSize;
            }
            ASSERTMSG(seekPos >= 0 && seekPos < _file.size(), "Invalid position in file");
            _file.seek(seekPos);
        } else {
            qint64 seekPos = _file.pos() - _frameDataSize;
            if (seekPos >= _filePosFrameDataStart) {
                ASSERTMSG(seekPos >= 0 && seekPos < _file.size(), "Invalid position in file");
                _file.seek(seekPos);
            }
        }
    }
}

void MainWindow::onStepTimeout()
{
    _stepTimer->stop();
    _rf.Paused = false;
}

void MainWindow::toggleReplayPaused()
{
    _rf.OneStep = false;

    _rf.Paused = !_rf.Paused;
    emit replayFlagsChanged(_rf);
}

void MainWindow::toggleReplayDir()
{
    _rf.Paused = false;
    _rf.OneStep = false;

    _rf.Forward = !_rf.Forward;
    emit replayFlagsChanged(_rf);
}

void MainWindow::toggleReplayMode()
{
    _rf.Paused = false;
    _rf.OneStep = false;

    _rf.StepByStep = !_rf.StepByStep;
    if (!_rf.StepByStep) {
        _stepTimer->stop();
    }
    emit replayFlagsChanged(_rf);
}

void MainWindow::replayJumpToStart()
{
    replayJumpTo(true);
}

void MainWindow::replayJumpToEnd()
{
    replayJumpTo(false);
}

void MainWindow::replayStepNext()
{
    _rf.Paused = false;
    _rf.OneStep = true;

    _rf.Forward = true;
    emit replayFlagsChanged(_rf);
}

void MainWindow::replayStepBack()
{
    _rf.Paused = false;
    _rf.OneStep = true;

    _rf.Forward = false;
    emit replayFlagsChanged(_rf);
}



void MainWindow::cleanup()
{
    std::cout << "Main window cleanup" << std::endl;
    
    _scene->deleteLater();
    _scene = nullptr;
   
    _cleanupNeeded = false;
}

void MainWindow::replayJumpTo(bool toStart)
{
    _rf.Paused = false;
    _rf.OneStep = false;

    ASSERT(_file.isOpen());
    ASSERT(_file.size() >= _frameDataSize);

    if (toStart) {
        _file.seek(_filePosFrameDataStart);
        _rf.Forward = true;
    } else {
        _file.seek(_filePosFrameDataEnd - _frameDataSize);
        _rf.Forward = false;
    }

    emit replayFlagsChanged(_rf);
}

void MainWindow::onGameStateChanged(GameState gs)
{
    if (gs.gameOver) {
        serializationEnd(gs);
        sceneEnd(gs);
    }
}

void MainWindow::sceneEnd(GameState gs)
{
    _ui->onGameEnd(gs);

    if (_cleanupNeeded) {
        cleanup();
    }
}

void MainWindow::serializationEnd(GameState gs)
{
    bool isLastFrame = true;
    pr("File pos end 0: " << _file.pos());
    _stream << isLastFrame;
    _stream << gs;
    //Scene is already deleted by this moment
    /*_stream << _playerScore;
    _stream << _playerSteps;*/
    pr("File pos end 1: " << _file.pos());

    _serializationTimer->stop();
    _file.flush();
    _file.close();
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

