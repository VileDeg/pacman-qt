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

    //connect(app, SIGNAL(aboutToQuit()), this, SLOT(atQuit()));

    //startGame(":/maps/asymm.txt");
    //startGame(":/maps/map.txt", false);
}

MainWindow::~MainWindow() {}

//void MainWindow::atQuit()
//{
//    
//}

void MainWindow::startGame(QString mapPath, bool recorded)
{
    if (_cleanupNeeded) {
        cleanup();
    }

    //_ui->onGameStart(mapPath.split("/").last().split(".").first());

    try {
        _scene = new GameScene(mapPath, _viewWidth, recorded, this);
    } catch (std::exception& e) {
        throw std::runtime_error("Error creating scene: " + std::string(e.what()));
    }

    _ui->view->setScene(_scene);

    connect(_scene, SIGNAL(gameEnd(bool, int)), this, SLOT(gameEnd(bool, int)));

    auto w = _scene->sceneRect().width();
    auto h = _scene->sceneRect().height();
    //_ui->view->move(_windowDim.width() / 2 - w / 2, _windowDim.height() / 2 - h / 2);
    _ui->view->setFixedSize(w, h);
    //Move to center of the window

    _scoreTimer = new QTimer(this); //this
    connect(_scoreTimer, SIGNAL(timeout()), this, SLOT(updateGameScore()));
    _scoreTimer->start(100);

    _cleanupNeeded = true;
}

void MainWindow::replay()
{
    /*static bool timeRead = false;
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
    }*/
}

void MainWindow::cleanup()
{
    _scoreTimer->stop();
    disconnect(_scoreTimer, SIGNAL(timeout()), this, SLOT(updateGameScore()));
    
    _scoreTimer->deleteLater();
    _scene->deleteLater();
   
    _cleanupNeeded = false;
}

void MainWindow::gameEnd(bool win, int score)
{
   
    _ui->onGameEnd(win, score);
}

void MainWindow::updateGameScore()
{
    if (!_scene) return;
    _ui->onUpdateGameScore(_scene->getScore());
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
