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
}

MainWindow::~MainWindow() {}

void MainWindow::startGame(QString mapPath, bool recorded)
{
    if (_cleanupNeeded) {
        cleanup();
    }

    try {
        _scene = new GameScene(mapPath, _viewWidth, recorded, this);
    } catch (std::exception& e) {
        throw std::runtime_error("Error creating scene: " + std::string(e.what()));
    }

    _ui->view->setScene(_scene);

    connect(_scene, SIGNAL(gameEnd(bool, int)), this, SLOT(gameEnd(bool, int)));

    connect(_scene, SIGNAL(gamePause(bool)), _ui, SLOT(onGamePause(bool)));
    connect(_scene, SIGNAL(playerScoreChanged(int)), _ui, SLOT(onPlayerScoreChanged(int)));
    connect(_scene, SIGNAL(playerStepsChanged(int)), _ui, SLOT(onPlayerStepsChanged(int)));

    auto w = _scene->sceneRect().width();
    auto h = _scene->sceneRect().height();
    
    _ui->view->setFixedSize(w, h);

    //_scoreTimer = new QTimer(this); //this
    //connect(_scoreTimer, SIGNAL(timeout()), this, SLOT(updateGameScore()));
    //_scoreTimer->start(100);

    _cleanupNeeded = true;
}

void MainWindow::cleanup()
{
    std::cout << "Main window cleanup" << std::endl;
    //_scoreTimer->stop();
    //disconnect(_scoreTimer, SIGNAL(timeout()), this, SLOT(updateGameScore()));
    
    //_scoreTimer->deleteLater();
    _scene->deleteLater();
   
    _cleanupNeeded = false;
}

void MainWindow::gameEnd(bool win, int score)
{
    if (_cleanupNeeded) {
        cleanup();
    }
    _ui->onGameEnd(win, score);
}

//void MainWindow::updateGameScore()
//{
//    if (!_scene) return;
//    _ui->onUpdateGameScore(_scene->getScore());
//}

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
