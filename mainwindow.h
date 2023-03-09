#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QLabel>
#include <vector>

#include "sprite.h"
#include "gameview.h"
#include "gamescene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void updateGameScore();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QSize GetDimensions() { return _windowDim; }
private:
    void keyPressEvent(QKeyEvent* event) override;
private:
    GameScene* _scene;
    GameView* _view;
    QLabel* _scoreLabel;

    QSize _windowDim{640, 480};
};

#endif // MAINWINDOW_H
