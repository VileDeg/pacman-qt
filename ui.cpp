#include <QMenuBar>
#include <QDirIterator>

#include "ui.h"
#include "gameview.h"
#include "mainwindow.h"
#include "utils.h"

WindowUI::WindowUI(MainWindow* mainWindow)
    : QWidget(qobject_cast<QWidget*>(mainWindow)), _mainWindow(mainWindow)
{
    init();
}

void WindowUI::initLabels() {
    labels["score"] = new QLabel(this);
    {
        labels["score"]->setStyleSheet("font-family: Fixedsys; color: white; font-size: 20px;");
        labels["score"]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }

    labels["win"] = new QLabel(this);
    {
        labels["win"]->setStyleSheet("font-family: Fixedsys; color: white; font-size: 100px;");
        labels["win"]->setAlignment(Qt::AlignCenter);
    }
}

void WindowUI::initActions()
{
    {
        actions["map"].clear();
        _path["map"].clear();
       
        QDirIterator it(":/maps/", { "*.txt" }, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            pr(it.next());
            QAction* act = new QAction(it.fileInfo().fileName(), this);
            actions["map"].append(act);
            _path["map"].append(it.filePath());
        }
    }
    {
        actions["rec"].clear();
        _path["rec"].clear();
        QDirIterator it("saves/", { "*.bin" }, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            pr(it.next());
            QAction* act = new QAction(it.fileInfo().fileName(), this);
            actions["rec"].append(act);
            _path["rec"].append(it.filePath());
        }
    }
}

#define BTN_CREATE(_key, _name, _parent)\
    b = buttons[_key] = new QPushButton(_name, _parent)
 
#define BTN_CONNECT(_b, _slot) connect(_b, SIGNAL(clicked()), this, SLOT(_slot))

void WindowUI::initButtons()
{
    QPushButton* b;
    BTN_CREATE("map", "Load map", this);
    {
        b->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        b->setMenu(menus["map"]);
    }

    BTN_CREATE("replay", "Replay", this);
    BTN_CONNECT(b, onReplayButtonClick());

    BTN_CREATE("back", "Back", this);
    BTN_CONNECT(b, onBackButtonClick());
    
    BTN_CREATE("pause", "Pause", this);
    BTN_CONNECT(b, onPauseButtonClick());

    BTN_CREATE("next", "Next", this);
    BTN_CONNECT(b, onNextButtonClick());

    /*BTN_CREATE("replaymode", "Replay mode:\nstep by step", this);
    BTN_CONNECT(b, onReplayModeSwitchButtonClick());*/
}

void WindowUI::initLayouts() {
    centrals["replay"] = new QWidget(this);
    {
        auto l = layouts["replay"] = new QHBoxLayout(centrals["replay"]);
        {
            //layouts["play"]->setAlignment(Qt::AlignCenter);
            l->addWidget(buttons["back"]);
            l->addWidget(buttons["pause"]);
            l->addWidget(buttons["next"]);
        }
    }
    centrals["replay"]->hide();

    centrals["play"] = new QWidget(this);
    {
        auto l = layouts["play"] = new QVBoxLayout(centrals["play"]);
        {
            l->setAlignment(Qt::AlignCenter);
            l->addWidget(labels["score"]);
            l->addWidget(view);
            l->addWidget(centrals["replay"]);
        }
    }

    centrals["gameend"] = new QWidget(this);
    {
        auto l = layouts["gameend"] = new QVBoxLayout(centrals["gameend"]);
        {
            l->setAlignment(Qt::AlignCenter);
            l->addWidget(labels["win"]);
            l->addWidget(buttons["map"]);
            l->addWidget(buttons["replay"]);
        }
    }
    centrals["gameend"]->hide();

    layouts["main"] = new QVBoxLayout(this);
    {
        layouts["main"]->setAlignment(Qt::AlignCenter);

        layouts["main"]->addWidget(centrals["play"]);
        layouts["main"]->addWidget(centrals["gameend"]);
    }
}

void WindowUI::initMenus() {
    menus["file"] = _mainWindow->menuBar()->addMenu("File");
    {
        menus["map"] = new QMenu("Load map", this);
        {
            menus["file"]->addMenu(menus["map"]);
            connect(menus["map"], SIGNAL(triggered(QAction*)), this, SLOT(loadMapMenuTriggered(QAction*)));
        }

        menus["rec"] = new QMenu("Load recording", this);
        {
            menus["file"]->addMenu(menus["rec"]);
            connect(menus["rec"], SIGNAL(triggered(QAction*)), this, SLOT(loadRecordingMenuTriggered(QAction*)));
        }
    }
}

void WindowUI::refresh()
{
    initActions();
    menus["map"]->clear();
    for (auto& act : actions["map"]) {
        menus["map"]->addAction(act);
    }
    menus["rec"]->clear();
    for (auto& act : actions["rec"]) {
       menus["rec"]->addAction(act);
    }
}

void WindowUI::init() {
    /*toolbar = new QToolBar(this);
    _mainWindow->addToolBar(Qt::LeftToolBarArea, toolbar);
    toolbar->setOrientation(Qt::Vertical);
    toolbar->addWidget(buttons["tbback"]);
    toolbar->addWidget(buttons["tbreplay"]);
    toolbar->addWidget(buttons["tbnext"]);*/
    
    initLabels();
    initActions();
    initMenus();
    initButtons();
    refresh();

    view = new GameView(this);
    view->setFrameShape(QFrame::NoFrame);
    view->resize(_mainWindow->_viewWidth, _mainWindow->_viewWidth);

    initLayouts();
    //_mainWindow->setLayout(layouts["main"]);
}

void WindowUI::loadMapMenuTriggered(QAction* act)
{
    pr("loadMapMenuTriggered");
    //Iterate through menu actions and find the one that was triggered
    auto acts = menus["map"]->actions();
    auto paths = _path["map"];
    for (int i = 0; i < acts.size(); ++i) {
        if (acts[i] == act) {
            //_mainWindow->cleanup();
            onGameStart(paths[i], false);
            return;
        }
    }
    assert(false);
}

void WindowUI::loadRecordingMenuTriggered(QAction* act)
{
    pr("loadRecordingMenuTriggered");
    //Iterate through menu actions and find the one that was triggered
    auto acts = menus["rec"]->actions();
    auto paths = _path["rec"];
    for (int i = 0; i < acts.size(); ++i) {
        if (acts[i] == act) {
            onGameStart(paths[i], true);
            return;
        }
    }
    assert(false);
}

void WindowUI::onBackButtonClick() {
    pr("onBackButtonClick");
    if (_mainWindow->_scene->_isPaused) {
        _mainWindow->_scene->setGamePause(false);
    }
    _mainWindow->_scene->_replayUntilNextTile = true;
    _mainWindow->_scene->setReplayMode(false);
}
void WindowUI::onPauseButtonClick() { //TODO
    pr("onPauseButtonClick");
    _mainWindow->_scene->setGamePause(!_mainWindow->_scene->_isPaused); //TODO
    buttons["pause"]->setText(_mainWindow->_scene->_isPaused ? "Resume" : "Pause");
}
void WindowUI::onNextButtonClick() { //TODO
    pr("onNextButtonClick");
    if (_mainWindow->_scene->_isPaused) {
        _mainWindow->_scene->setGamePause(false);
    }
    _mainWindow->_scene->_replayUntilNextTile = true;
    _mainWindow->_scene->setReplayMode(true);
}

void WindowUI::onReplayModeSwitchButtonClick()
{
}

void WindowUI::onReplayButtonClick()
{
    pr("onReplayButtonClick");
    for (auto& rec : _path["rec"]) {
        if (rec.split("/").last().split(".").first() == currentMapName) {
            onGameStart(rec, true);
            return;
        }
    }
    ASSERT(false);
}

//void WindowUI::onReplayStart(QString filePath) {
//    centrals["replay"]->show();
//    _mainWindow->startGame(filePath, true);
//}

void WindowUI::onGameStart(QString filePath, bool isRecorded)
{
    centrals["gameend"]->hide();
    centrals["play"]->show();
    if (isRecorded) {
        centrals["replay"]->show();
    } else {
        centrals["replay"]->hide();
    }

    currentMapName = filePath.split("/").last().split(".").first();
    _mainWindow->startGame(filePath, isRecorded);
}

//void WindowUI::onGameStart(QString mapName)
//{
//    centrals["gameend"]->hide();
//    centrals["play"]->show();
//
//    currentMapName = mapName;
//}

void WindowUI::onGameEnd(bool win, int score)
{
    if (win) {
        labels["win"]->setText("You won!\nScore: " + QString::number(score));
    } else {
        labels["win"]->setText("You lost :(\nScore: " + QString::number(score));
    }

    centrals["play"]->hide();
    centrals["replay"]->hide();
    centrals["gameend"]->show();

    refresh();
}


void WindowUI::onUpdateGameScore(int score)
{
    labels["score"]->setText("Score: " + QString::number(score));
}