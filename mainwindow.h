#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QLabel>
#include <QVBoxLayout>
#include <vector>

#include "sprite.h"
#include "gameview.h"
#include "gamescene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void updateGameScore();
    void playerWin();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void keyPressEvent(QKeyEvent* event) override;
private:
    GameScene* _scene;
    GameView* _view;
    QLabel* _scoreLabel;
    QLabel* _winLabel;
    QVBoxLayout* _vLayout;
    QWidget* _centeralWidget;

    QSize _windowDim{640, 480};
};

#endif // MAINWINDOW_H
