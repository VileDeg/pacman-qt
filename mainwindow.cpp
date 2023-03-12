#include <QGraphicsScene>

#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <QMenuBar>
#include <QDirIterator>
#include <QMouseEvent>

#include <cstdlib>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include "ui.h"

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

    connect(app, SIGNAL(aboutToQuit()), this, SLOT(atQuit()));

    //startGame(":/maps/asymm.txt");
    startGame(":/maps/map.txt");
}

MainWindow::~MainWindow() {}

void MainWindow::atQuit()
{
    
}

void MainWindow::startGame(QString mapPath)
{
    _ui->otherCentral->hide();
    _ui->mapCentral->show();

    try {
        _scene = new GameScene(mapPath, _viewWidth, this);
    } catch (std::exception& e) {
        throw std::runtime_error("Error creating scene: " + std::string(e.what()));
    }

    _ui->view->setScene(_scene);

    connect(_scene, SIGNAL(gameEnd(bool, int)), this, SLOT(gameEnd(bool, int)));

    auto w = _scene->sceneRect().width();
    auto h = _scene->sceneRect().height();
    //Set view size forever according to contents. Add 2 because of the borders
    _ui->view->setFixedSize(w + 2, h + 2);
    //Move to center of the window
    _ui->view->move(_windowDim.width() / 2 - w / 2, _windowDim.height() / 2 - h / 2);

    _scoreTimer = new QTimer(this); //this
    connect(_scoreTimer, SIGNAL(timeout()), this, SLOT(updateGameScore()));
    _scoreTimer->start(100);

    _saveFile.setFileName("saves/save.txt");
    if (!_saveFile.open(_replay ? QIODevice::ReadOnly : QIODevice::WriteOnly)) {
        throw std::runtime_error("Could not open file");
    }

    _saveStream.setDevice(&_saveFile);
    _startTime = QTime::currentTime();

    if (_replay) {
        _replayTimer = new QTimer(this);
        connect(_replayTimer, SIGNAL(timeout()), this, SLOT(replay()));
        _replayTimer->start(100);
    }
}

void MainWindow::replay()
{
    static bool timeRead = false;
    static int nextTime = 0;
    if (_saveFile.atEnd()) {
        __debugbreak();
        assert(false);
    }

    int msSinceStart = _startTime.msecsTo(QTime::currentTime());
    pr("Since: " << msSinceStart);
    pr("Next time: " << nextTime);

    if (!timeRead) {
        _saveStream >> nextTime;
        timeRead = true;
    }
    if (msSinceStart < nextTime) {
        return;
    }
    timeRead = false;

    int type = 0;
    _saveStream >> type;
    if (type == 0) {
        int key;
        _saveStream >> key;
        auto ke = QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier);
        _scene->onKeyPress(&ke);
    } else if (type == 1) {
        QPointF mousePos;
        _saveStream >> mousePos;
        auto me = QMouseEvent(QEvent::MouseButtonPress, mousePos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        _scene->onMousePress(&me, mousePos);
    } else if (type == 2) {
        bool win;
        int score;
        _saveStream >> win;
        _saveStream >> score;
        gameEnd(win, score);
    }
}

void MainWindow::cleanup()
{
    _scoreTimer->stop();
    disconnect(_scoreTimer, SIGNAL(timeout()), this, SLOT(updateGameScore()));
    if (_replay) {
        _replayTimer->stop();
        delete _replayTimer;
    }
    delete _scoreTimer;
    delete _scene;
    _scene = nullptr;
   
    cleanupDone = true;
}

void MainWindow::gameEnd(bool win, int score)
{
    if (!_replay) {
        _saveStream << _startTime.msecsTo(QTime::currentTime());
        _saveStream << 2;
        _saveStream << win;
        _saveStream << score;
    }
    cleanup();
    _ui->onGameEnd(win, score);
}

void MainWindow::updateGameScore()
{
    if (!_scene) return;
    _ui->onUpdateGameScore(_scene->getScore());
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (_replay) return;
    if (!_scene) return;
    _saveStream << _startTime.msecsTo(QTime::currentTime());
    _saveStream << 0;
    _saveStream << event->key();
    _scene->onKeyPress(event);
}

void MainWindow::mousePressEvent(QMouseEvent * event)
{
    if (_replay) return;
    if (!_scene) return;

    QPoint origin = _ui->view->mapFromGlobal(QCursor::pos());
    QPointF relativeOrigin = _ui->view->mapToScene(origin);
    _saveStream << relativeOrigin;
    _scene->onMousePress(event, relativeOrigin);
}
