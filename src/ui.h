/** @file ui.h
 *  @author Vadim Goncearenco <xgonce00@stud.fit.vutbr.cz>
 *  @brief File with declration of window UI class.
 *  @details This file contains declration of window UI class.
 *  Every UI element in project was created manually. Qt Designer was not used at all.
 */

#ifndef UI_H
#define UI_H

#include <QGraphicsView>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

#include <unordered_map>
#include <functional>

#include "replay.h"

class MainWindow;

/**
 * @brief Window UI class.
 * @details This class is responsible for window UI.
 * It contains all UI elements and is responsible for their layout. 
 */
class WindowUI : public QWidget {
    Q_OBJECT

private slots:
    void onLoadMapMenuTriggered(QAction* action);
    void onLoadRecordingMenuTriggered(QAction* action);
    
    void onRetryButtonClick();

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
    std::unordered_map<std::string, QLabel*> labels = {};
    std::unordered_map<std::string, QPushButton*> buttons = {};
    std::unordered_map<std::string, QBoxLayout*> layouts = {};
    std::unordered_map<std::string, QMenu*> menus = {};
    std::unordered_map<std::string, QVector<QAction*>> actions = {};
    std::unordered_map<std::string, QWidget*> centrals = {};

    QGraphicsView* view = nullptr;
    QString currentMapName;

    MainWindow* _mainWindow = nullptr;
    GameState _state;
    std::unordered_map<std::string, QVector<QString>> _path = {};

    friend class MainWindow;
};

#endif // UI_H