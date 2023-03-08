#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <vector>

#include "sprite.h"
#include "gameview.h"
#include "gamescene.h"


//namespace Ui {
//class MainWindow;
//}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QSize GetDimensions() { return _windowDim; }
private:
    void keyPressEvent(QKeyEvent* event) override;
private:
    //Ui::MainWindow* _ui;
    GameScene* _scene;
    GameView* _view;

    QSize _windowDim{1024, 768};
};

#endif // MAINWINDOW_H
