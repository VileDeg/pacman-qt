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
    } menus;
    QVector<QAction*> actions;
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
    QVector<QString> _mapsPath;

    friend class MainWindow;
};