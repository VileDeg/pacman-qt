#include <QGraphicsScene>
#include <QKeyEvent>

#include <QTimer>

#include <QDebug>

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
        //_scoreLabel->setText("Score: ");
        _scoreLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        QTimer* scoreTimer = new QTimer(this);
        connect(scoreTimer, SIGNAL(timeout()), this, SLOT(updateGameScore()));
        scoreTimer->start(100);
    }

    _winLabel = new QLabel(this);
    {
        _winLabel->setStyleSheet("font-family: Fixedsys; color: white; font-size: 60px;");
        _winLabel->setText("You won!\nScore: ");
        _winLabel->setWordWrap(true);
        //_winLabel->setTextFormat(Qt::RichText);
        _winLabel->setAlignment(Qt::AlignCenter);

        _winLabel->hide();
    }

    _scene = new GameScene(this);
    connect(_scene, SIGNAL(playerWin()), this, SLOT(playerWin()));

    _view = new GameView(this);
    _view->setScene(_scene);

    _vLayout = new QVBoxLayout();
    _centeralWidget = new QWidget();
    _centeralWidget->setLayout(_vLayout);
    setCentralWidget(_centeralWidget);

    _vLayout->addWidget(_scoreLabel);
    _vLayout->addWidget(_view);
    _vLayout->setAlignment(Qt::AlignCenter);

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

void MainWindow::playerWin()
{
    _centeralWidget->hide();
    _winLabel->show();
    _winLabel->setText(_winLabel->text() + QString::number(_scene->getScore()));
}

void MainWindow::updateGameScore()
{
    static QString scoreText = "Score: ";
    int score = _scene->getScore();
    _scoreLabel->setText(scoreText + QString::number(score));
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

