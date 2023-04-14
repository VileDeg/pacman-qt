/** @file mainwindow.h
 *  @author Vadim Goncearenco <xgonce00@stud.fit.vutbr.cz>
 *  @brief File with window class declaration.
 *  @details This file contains main window class declaration.
 *  Main window is responsible for creating and managing game scene and view.
 *  It also binds the game scene with UI elements.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <functional>

#include "sprite.h"
#include "gameview.h"
#include "gamescene.h"
#include "replay.h"

class WindowUI;

/**
 * @brief Main window class.
 * @details Controls communications between UI and game scene.
 * Also contains serializer class that is responsible 
 * for saving and loading game state for replay purposes.
 */
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
    ErrorCallback _errorCallback; /**< Callback for handling errors during construction. */
    WindowUI* _ui = {};
    GameScene* _scene = nullptr; /**< Current game scene. */

    QSize _windowDim = { 800, 600 }; /**< Window dimensions. */
    
    int _viewWidth = 600; /**< Width/height of the game view. */
    int _offsetAroundView = 200;

    Serializer _serializer;
    GameState _state; /**< Current game state. */
    
    bool _cleanupNeeded = false; 
    bool _currentlyReplaying = false;

    std::unordered_map<SpriteType, QImage> _pixmapCache = {}; /**< Cache for loaded images. */

    friend class WindowUI;
};

#endif // MAINWINDOW_H
