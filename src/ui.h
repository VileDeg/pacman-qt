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
    void onloadMapMenuTriggered(QAction* action);
    void onloadRecordingMenuTriggered(QAction* action);
    
    void onReplayFromStartButtonClick();
    void onReplayFromEndButtonClick();

    void onReplayFlagsChanged(ReplayFlags);
    void onGameStateChanged(GameState);

public:
    WindowUI(MainWindow* mainWindow);
    ~WindowUI() {}

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
    std::unordered_map<std::string, QLabel*> labels;
    std::unordered_map<std::string, QPushButton*> buttons;
    std::unordered_map<std::string, QBoxLayout*> layouts;
    std::unordered_map<std::string, QMenu*> menus;
    std::unordered_map<std::string, QVector<QAction*>> actions;
    std::unordered_map<std::string, QWidget*> centrals;

    QGraphicsView* view;
    QString currentMapName;

    MainWindow* _mainWindow;
    GameState _state;
    std::unordered_map<std::string, QVector<QString>> _path;

    friend class MainWindow;
};