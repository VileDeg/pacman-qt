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
    void gameEnd(bool win, int score);
    void replay();
    //void atQuit();
   
public:
    explicit MainWindow(QApplication* app, QWidget *parent = 0);
    ~MainWindow();

private:
    void startGame(QString mapPath, bool recorded, bool replayFromStart);
    
    
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
    QTimer* _scoreTimer;
    bool _cleanupNeeded = false;
    //bool anyGameStarted = false;
    /*QFile _saveFile;
    QTime _startTime;
    QDataStream _saveStream;
    bool _replay = 1;
    QTimer* _replayTimer;
    */
    friend class WindowUI;
};

#endif // MAINWINDOW_H
