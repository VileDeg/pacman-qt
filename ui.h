#pragma once

#include <QGraphicsView>
#include <QLabel>
#include <QVBoxLayout>
#include <QMainWindow>

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
        QVBoxLayout* main;
        QVBoxLayout* map;
        QVBoxLayout* other;
    } layouts;
    struct {
        QMenu* file;
        QMenu* loadMap;
    } menus;
    QWidget* mapCentral;
    QWidget* otherCentral;
    QGraphicsView* view;
private:
    void init();
    void initLabels();
    void initLayouts();
    void initMenus();

    void onPlayerWin(int score);
    void onUpdateGameScore(int score);
private:
    MainWindow* _mainWindow;
    QVector<QString> _mapsPath;

    friend class MainWindow;
};