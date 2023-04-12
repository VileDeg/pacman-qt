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
    //void updateGameScore();
    void gameEnd(bool win, int score);
    
public:
    explicit MainWindow(QApplication* app, QWidget *parent = 0);
    ~MainWindow();

private:
    void startGame(QString mapPath, bool recorded);
    void cleanup();

    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    QApplication* _app;
    GameScene* _scene = nullptr;
    WindowUI* _ui{};
    QSize _windowDim;
    int _viewWidth{ 600 };
    int _offsetAroundView{ 200 };
    //QTimer* _scoreTimer;
    bool _cleanupNeeded = false;

    friend class WindowUI;
};

#endif // MAINWINDOW_H
