#pragma once

#include <QGraphicsView>
#include <QLabel>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QPushButton>

class MainWindow;

class WindowUI : public QWidget {
    Q_OBJECT

private slots:
    void loadMapMenuTriggered(QAction* action);
    void loadRecordingMenuTriggered(QAction* action);
public:
    WindowUI(MainWindow* mainWindow);
    ~WindowUI() {}

    struct {
        QLabel* score;
        QLabel* win;
    } labels;
    struct {
        QPushButton* loadMap;
    } buttons;
    struct {
        QVBoxLayout* main;
        QVBoxLayout* map;
        QVBoxLayout* other;
    } layouts;
    struct {
        QMenu* file;
        QMenu* loadMap;
        QMenu* loadRecording;
    } menus;
    struct {
        QVector<QAction*> map;
        QVector<QAction*> recording;
    } actions;
    QWidget* mapCentral;
    QWidget* otherCentral;
    QGraphicsView* view;
private:
    void init();
    void initLabels();
    void initActions();
    void initButtons();
    void initLayouts();
    void initMenus();

    void onGameEnd(bool win, int score);

    void onUpdateGameScore(int score);
private:
    MainWindow* _mainWindow;
    struct{
        QVector<QString> map;
        QVector<QString> recording;
    } _path;

    friend class MainWindow;
};