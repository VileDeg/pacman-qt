#include <QGraphicsScene>
#include <QKeyEvent>

#include <QTimer>

#include <QDebug>

#include <cstdlib>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    //_ui(new Ui::MainWindow)
{
    //_ui->setupUi(this);

    setWindowTitle("Pac-Man");
    resize(_windowDim);
    //Move window to center of display
   /* QRect desktopRect = QApplication::desktop()->availableGeometry(this);
    QPoint center = desktopRect.center();
    setGeometry(center.x() - width() * 0.5, center.y() - height() * 0.5,
                width(), height());*/

    _scene = new GameScene(this);
    //_scene->setBackgroundBrush(Qt::black);
    _view = new GameView(this);
    _view->setScene(_scene);

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
    //delete _ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_W) {
        pr("W pressed");
        _scene->setPlayerMoveDir(MoveDir::Up);
        // Handle "W" key press
    } else if (event->key() == Qt::Key_A) {
        pr("A pressed");
        _scene->setPlayerMoveDir(MoveDir::Left);
        // Handle "A" key press
    } else if (event->key() == Qt::Key_S) {
        pr("S pressed");
        _scene->setPlayerMoveDir(MoveDir::Down);
        // Handle "S" key press
    } else if (event->key() == Qt::Key_D) {
        pr("D pressed");
        _scene->setPlayerMoveDir(MoveDir::Right);
    }
}

