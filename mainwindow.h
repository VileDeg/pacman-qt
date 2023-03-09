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
    void playerWin();
    
   
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void startGame(QString mapPath);
    void endGame();

    void keyPressEvent(QKeyEvent* event) override;
private:
    GameScene* _scene;
    WindowUI* _ui{};
    QSize _windowDim{ 640, 480 };
    int _viewWidth{ 320 };
    QTimer* _scoreTimer;
    
    friend class WindowUI;
};

#endif // MAINWINDOW_H
