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
    void gameEnd(bool win, int score, int steps);
    void onSerialize();
    void onDeserialize();

public:
    explicit MainWindow(QApplication* app, QWidget *parent = 0);
    ~MainWindow();

private:
    void startGame(QString mapPath, bool recorded, bool replayFromStart);
    void cleanup();

    void toggleReplayPaused();
    bool toggleReplayDir();

    void replayJumpTo(bool toStart);
    void replayStepNext();
    void replayStepBack();

    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

    void loadImages();

private:
    QApplication* _app;
    GameScene* _scene = nullptr;
    WindowUI* _ui{};
    QSize _windowDim;
    
    int _viewWidth{ 600 };
    int _offsetAroundView{ 200 };

    QFile _file;
    QDataStream _stream;
    QTimer* _serializationTimer;

    bool _replayOneStep = false;
    bool _replayForward = true;
    bool _replayPaused = false;
    bool _gameEnded = false;
    bool _gameWon = false;
    bool _cleanupNeeded = false;

    int _playerScore, _playerSteps;

    int _serializationFPS = 24;
    int _serializationInterval = 100;
    int _stepInterval = 500;
    int _framesPerStep = 10;
    int _msSinceLastStep = 0;
    //int _sceneDataSize = 0;
    int _frameDataSize = 0;

    int _filePosFrameDataStart = 0;
    int _filePosFrameDataEnd = 0;

    std::unordered_map<SpriteType, QImage> _pixmapCache;

    friend class WindowUI;
};

#endif // MAINWINDOW_H
