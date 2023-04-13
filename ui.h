#pragma once

#include <QGraphicsView>
#include <QLabel>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QToolBar>

#include <unordered_map>
#include <functional>

#include "replay.h"

class MainWindow;

class WindowUI : public QWidget {
    Q_OBJECT

private slots:
    void loadMapMenuTriggered(QAction* action);
    void loadRecordingMenuTriggered(QAction* action);
    void onReplayFromStartButtonClick();
    void onReplayFromEndButtonClick();

    void onReplayFlagsChanged(ReplayFlags);

    void onGameStateChanged(GameState);
public:
    WindowUI(MainWindow* mainWindow);
    ~WindowUI() {}

    std::unordered_map<QString, QLabel*> labels;
    std::unordered_map<QString, QPushButton*> buttons;
    std::unordered_map<QString, QBoxLayout*> layouts;
    std::unordered_map<QString, QMenu*> menus;
    std::unordered_map<QString, QVector<QAction*>> actions;

    std::unordered_map<QString, QWidget*> centrals;
    
    QGraphicsView* view;
    QToolBar* toolbar;

    QString currentMapName;
private:
    void init();
    void initLabels();
    void initActions();
    void initButtons();
    void initLayouts();
    void initMenus();
    void refresh();
    
    void onGameStart(QString filePath, bool isRecorded, bool replayFromStart);
    void onGameEnd(GameState);

    
    
private:
    MainWindow* _mainWindow;
    GameState _state;

    std::unordered_map<QString, QVector<QString>> _path;

    friend class MainWindow;
};