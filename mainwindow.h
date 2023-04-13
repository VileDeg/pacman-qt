#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QLabel>
#include <QVBoxLayout>
#include <vector>
#include <functional>

#include "sprite.h"
#include "gameview.h"
#include "gamescene.h"

#include "replay.h"

class WindowUI;

class MainWindow : public QMainWindow
{
    using ErrorCallback = std::function<void(QString, int)>;

    Q_OBJECT

private slots:
    //void updateGameScore();
    void onGameStateChanged(GameState);
    void onDeserializationEnded(GameState);
    
//signals:
//    void replayFlagsChanged(const ReplayFlags& rf);
public:
    explicit MainWindow(QApplication* app, ErrorCallback errorCallback, QWidget *parent = 0);
    ~MainWindow();

    const Serializer* getSerializer() const { return &_serializer; }
private:
    void startGame(QString mapPath, bool recorded, bool replayFromStart);
    void cleanup();

    
    void sceneEnd(GameState);
    GameState getGameState() { return _state; }
    

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

    Serializer _serializer;
    GameState _state;
   /* QFile _file;
    QDataStream _stream;
    QTimer* _serializationTimer;
    QTimer* _stepTimer;*/

    ErrorCallback _errorCallback;
    
    bool _gameEnded = false;
    //bool _gameWon = false;
    bool _cleanupNeeded = false;

    bool _currentlyReplaying = false;

    //ReplayFlags _rf;

    //int _playerScore, _playerSteps;

    /*int _serializationFPS = 24;
    int _serializationInterval = 100;
    int _stepInterval = 250;
    int _framesPerStep = 10;
    int _msSinceLastStep = 0;*/
    //int _sceneDataSize = 0;
    

    /*int _filePosFrameDataStart = 0;
    int _filePosFrameDataEnd = 0;*/

    std::unordered_map<SpriteType, QImage> _pixmapCache;

    friend class WindowUI;
};

#endif // MAINWINDOW_H
