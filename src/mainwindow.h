#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void onGameStateChanged(GameState);
    void onDeserializationEnded(GameState);
public:
    explicit MainWindow(ErrorCallback errorCallback, QWidget *parent = 0);
    ~MainWindow();

    const Serializer* getSerializer() const { return &_serializer; }
private:
    void startGame(QString mapPath, bool recorded, bool replayFromStart);
    void sceneEnd();

    void loadImages();
    void cleanup();

    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    ErrorCallback _errorCallback;
    WindowUI* _ui{};
    GameScene* _scene = nullptr;

    QSize _windowDim;
    
    int _viewWidth{ 600 };
    int _offsetAroundView{ 200 };

    Serializer _serializer;
    GameState _state;
    
    bool _gameEnded = false;
    bool _cleanupNeeded = false;
    bool _currentlyReplaying = false;

    std::unordered_map<SpriteType, QImage> _pixmapCache;

    friend class WindowUI;
};

#endif // MAINWINDOW_H
