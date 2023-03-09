#include <QGraphicsScene>
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <QMenuBar>
#include <QDirIterator>

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
    setFixedSize(_windowDim);

    _ui = new WindowUI(this);
    setCentralWidget(_ui);


    startGame(":/maps/map.txt");
}

MainWindow::~MainWindow()
{
}

void MainWindow::startGame(QString mapPath)
{
    _ui->otherCentral->hide();
    _ui->mapCentral->show();

    _scene = new GameScene(_viewWidth, this);
    _ui->view->setScene(_scene);

    connect(_scene, SIGNAL(playerWin()), this, SLOT(playerWin()));

    if (!_scene->loadMap(":/maps/map.txt")) {
        return;
    }

    auto w = _scene->sceneRect().width();
    auto h = _scene->sceneRect().height();
    //Set view size forever according to contents. Add 2 because of the borders
    _ui->view->setFixedSize(w + 2, h + 2);
    //Move to center of the window
    _ui->view->move(_windowDim.width() / 2 - w / 2, _windowDim.height() / 2 - h / 2);

    _scoreTimer = new QTimer(this);
    connect(_scoreTimer, SIGNAL(timeout()), this, SLOT(updateGameScore()));
    _scoreTimer->start(100);
}

void MainWindow::endGame()
{
    _scoreTimer->deleteLater();
    _scoreTimer = nullptr;
    _scene->deleteLater();
    _scene = nullptr;
}

void MainWindow::playerWin()
{
    _ui->onPlayerWin(_scene->getScore());
}

void MainWindow::updateGameScore()
{
    _ui->onUpdateGameScore(_scene->getScore());
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_W) {
        _scene->setPlayerMoveDir(MoveDir::Up);
    } else if (event->key() == Qt::Key_A) {
        _scene->setPlayerMoveDir(MoveDir::Left);
    } else if (event->key() == Qt::Key_S) {
        _scene->setPlayerMoveDir(MoveDir::Down);
    } else if (event->key() == Qt::Key_D) {
        _scene->setPlayerMoveDir(MoveDir::Right);
    }
}

