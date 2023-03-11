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


class WindowUI;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void updateGameScore();
    /*void playerWin();
    void playerLoose();*/
    void gameEnd(bool win, int score);
   
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void startGame(QString mapPath);
    void cleanup();

    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    GameScene* _scene;
    WindowUI* _ui{};
    QSize _windowDim;
    int _viewWidth{ 600 };
    int _offsetAroundView{ 100 };
    QTimer* _scoreTimer;
    bool cleanupDone = false;
    
    friend class WindowUI;
};

#endif // MAINWINDOW_H
