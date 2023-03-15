#pragma once

#include <QGraphicsView>
#include <QLabel>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QToolBar>

#include <unordered_map>

class MainWindow;

class WindowUI : public QWidget {
    Q_OBJECT

private slots:
    void loadMapMenuTriggered(QAction* action);
    void loadRecordingMenuTriggered(QAction* action);
    void onReplayButtonClick();
public:
    WindowUI(MainWindow* mainWindow);
    ~WindowUI() {}

    std::unordered_map<QString, QLabel*> labels;
    std::unordered_map<QString, QPushButton*> buttons;
    std::unordered_map<QString, QVBoxLayout*> layouts;
    std::unordered_map<QString, QMenu*> menus;
    std::unordered_map<QString, QVector<QAction*>> actions;

    QWidget* mapCentral;
    QWidget* otherCentral;
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

    
    void onGameEnd(bool win, int score);

    void onUpdateGameScore(int score);
private:
    MainWindow* _mainWindow;

    std::unordered_map<QString, QVector<QString>> _path;

    friend class MainWindow;
};