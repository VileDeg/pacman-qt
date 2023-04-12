#pragma once

#include <QGraphicsView>
#include <QLabel>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QToolBar>

#include <unordered_map>
#include <functional>

class MainWindow;

class WindowUI : public QWidget {
    Q_OBJECT

private slots:
    void loadMapMenuTriggered(QAction* action);
    void loadRecordingMenuTriggered(QAction* action);
    void onReplayStartButtonClick();
    void onReplayEndButtonClick();

    void onStepBackButtonClick();
    void onPauseButtonClick();
    void onStepForwardButtonClick();
    void onPlayBackwardButtonClick();
    void onPlayForwardButtonClick();
    void onReplayModeSwitchButtonClick();
public:
    WindowUI(MainWindow* mainWindow);
    ~WindowUI() {}

    std::unordered_map<QString, QLabel*> labels;
    std::unordered_map<QString, QPushButton*> buttons;
    std::unordered_map<QString, QBoxLayout*> layouts;
    std::unordered_map<QString, QMenu*> menus;
    std::unordered_map<QString, QVector<QAction*>> actions;

    std::unordered_map<QString, QWidget*> centrals;
    /*QWidget* playCentral;
    QWidget* gameEndCentral;*/
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
    
    //void onReplayStart(QString filePath);
    void onGameStart(QString filePath, bool isRecorded);
    void onGameEnd(bool win, int score);

    void onUpdateGameScore(int score);
private:
    MainWindow* _mainWindow;

    std::unordered_map<QString, QVector<QString>> _path;

    friend class MainWindow;
};