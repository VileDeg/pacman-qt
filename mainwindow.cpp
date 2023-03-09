#include <QGraphicsScene>
#include <QKeyEvent>

#include <QTimer>

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>

#include <cstdlib>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setStyleSheet("background-color: black;");
    setWindowTitle("Pac-Man");
    setFixedSize(_windowDim);

    _scoreLabel = new QLabel(this);
    {
        _scoreLabel->setStyleSheet("font-family: Fixedsys; color: white; font-size: 20px;");
        _scoreLabel->setText("Score: ");
        _scoreLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        QTimer* scoreTimer = new QTimer(this);
        connect(scoreTimer, SIGNAL(timeout()), this, SLOT(updateGameScore()));
        scoreTimer->start(100);
    }


    _scene = new GameScene(this);
    _view = new GameView(this);
    _view->setScene(_scene);

    auto* vLayout = new QVBoxLayout();
    QWidget* placeholderWidget = new QWidget();
    placeholderWidget->setLayout(vLayout);
    setCentralWidget(placeholderWidget);

    vLayout->addWidget(_scoreLabel);
    vLayout->addWidget(_view);
    vLayout->setAlignment(Qt::AlignCenter);

    if (!_scene->loadMap(":/maps/map.txt")) {
        return;
    }

    auto w = _scene->sceneRect().width();
    auto h = _scene->sceneRect().height();
    //Set view size forever according to contents. Add 2 because of the borders
    _view->setFixedSize(w+2,h+2);
    //Move to center of the window
    _view->move(_windowDim.width()/2-w/2, _windowDim.height()/2-h/2);
}

MainWindow::~MainWindow()
{
}

void MainWindow::updateGameScore()
{
    int score = _scene->getScore();
    _scoreLabel->setText("Score: " + QString::number(score));
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

