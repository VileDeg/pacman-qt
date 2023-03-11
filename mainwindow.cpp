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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
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

    //startGame(":/maps/asymm.txt");
    startGame(":/maps/map.txt");
}

MainWindow::~MainWindow() {}

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
}

void MainWindow::cleanup()
{
    disconnect(_scoreTimer, SIGNAL(timeout()), this, SLOT(updateGameScore()));
    delete _scoreTimer;
    delete _scene;
    _scene = nullptr;
   
    cleanupDone = true;
}

void MainWindow::gameEnd(bool win, int score)
{
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
