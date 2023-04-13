#include <QMenuBar>
#include <QDirIterator>

#include "ui.h"
#include "gameview.h"
#include "mainwindow.h"
#include "utils.h"

#define BTN_CREATE(_key, _name, _parent)\
    b = buttons[_key] = new QPushButton(_name, _parent)

#define BTN_CONNECT(_b, _slot) connect(_b, SIGNAL(clicked()), this, SLOT(_slot))

#define INTRO_STR\
    "How to play:\n"\
    "You can load a map by clicking the button below.\n"\
    "You can choose one of the maps from \"saves/\" folder.\n"\
    "After the game ends, you can replay it."

WindowUI::WindowUI(MainWindow* mainWindow)
    : QWidget(qobject_cast<QWidget*>(mainWindow)), _mainWindow(mainWindow)
{
    init();
}

void WindowUI::initLabels() {
    auto l = labels["score"] = new QLabel(this);
    {
        l->setStyleSheet("font-family: Fixedsys; color: white; font-size: 80px;");
        l->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        l->setText("Score: ");
    }

    l = labels["steps"] = new QLabel(this);
    {
        l->setStyleSheet("font-family: Fixedsys; color: white; font-size: 80px;");
        l->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        l->setText("Steps: ");
    }

    l = labels["win"] = new QLabel(this);
    {
        l->setStyleSheet("font-family: Fixedsys; color: white; font-size: 100px;");
        l->setAlignment(Qt::AlignCenter);
    }

    l = labels["intro"] = new QLabel(this);
    {
        l->setStyleSheet("font-family: Fixedsys; color: white; font-size: 100px;");
        l->setAlignment(Qt::AlignCenter);
        l->setText(INTRO_STR);
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

void WindowUI::initButtons()
{
    QPushButton* b;
    BTN_CREATE("map", "Load map", this);
    {
        //b->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        b->setMenu(menus["map"]);
    }

    BTN_CREATE("replay_start", "Replay from start", this);
    BTN_CONNECT(b, onReplayFromStartButtonClick());

    BTN_CREATE("replay_end", "Replay from end", this);
    BTN_CONNECT(b, onReplayFromEndButtonClick());

    
    BTN_CREATE("pause", "Pause", this);
    BTN_CONNECT(b, onPauseButtonClick());

    BTN_CREATE("next", "Step Forward", this);
    BTN_CONNECT(b, onStepNextButtonClick());

    BTN_CREATE("back", "Step Back", this);
    BTN_CONNECT(b, onStepBackButtonClick());

    BTN_CREATE("replay_mode", "Step by Step Mode", this);
    BTN_CONNECT(b, onReplayModeButtonClick());


    BTN_CREATE("jump_start", "Jump to Start", this);
    BTN_CONNECT(b, onJumpToStartButtonClick());

    BTN_CREATE("jump_end", "Jump to End", this);
    BTN_CONNECT(b, onJumpToEndButtonClick());


    BTN_CREATE("replay_dir", "Play Backward", this);
    BTN_CONNECT(b, onReplayDirButtonClick());

    /*BTN_CREATE("normal", "Normal Mode", this);
    BTN_CONNECT(b, onNormalButtonClick());*/

    /*BTN_CREATE("backward", "Play Backward", this);
    BTN_CONNECT(b, onPlayBackwardButtonClick());

    BTN_CREATE("forward", "Play Forward", this);
    BTN_CONNECT(b, onPlayForwardButtonClick());*/

    /*BTN_CREATE("replaymode", "Replay mode:\nstep by step", this);
    BTN_CONNECT(b, onReplayModeSwitchButtonClick());*/
}

void WindowUI::initLayouts() {
    centrals["replay"] = new QWidget(this);
    {
        auto l = layouts["replay"] = new QVBoxLayout(centrals["replay"]);
        {
            auto l1 = layouts["replay1"] = new QHBoxLayout();
            {
                l1->addWidget(buttons["back"]);
                l1->addWidget(buttons["pause"]);
                l1->addWidget(buttons["next"]);
            }
            l->addLayout(l1);
            auto l2 = layouts["replay2"] = new QHBoxLayout();
            {
                l2->addWidget(buttons["replay_dir"]);
                l2->addWidget(buttons["replay_mode"]);
            }
            l->addLayout(l2);
            auto l3 = layouts["replay2"] = new QHBoxLayout();
            {
                l3->addWidget(buttons["jump_start"]);
                l3->addWidget(buttons["jump_end"]);
            }
            l->addLayout(l3);
        }
    }
    centrals["replay"]->hide();

    centrals["play"] = new QWidget(this);
    {
        auto l = layouts["play"] = new QVBoxLayout(centrals["play"]);
        {
            l->setAlignment(Qt::AlignCenter);

            auto l1 = layouts["play1"] = new QHBoxLayout();
            {
                l1->addWidget(labels["score"]);
                l1->addWidget(labels["steps"]);
            }
            l->addLayout(l1);

            l->addWidget(view);
            l->addWidget(centrals["replay"]);
        }
    }
    centrals["play"]->hide();

    centrals["gameend"] = new QWidget(this);
    {
        auto l = layouts["gameend"] = new QVBoxLayout(centrals["gameend"]);
        {
            l->setAlignment(Qt::AlignCenter);
            l->addWidget(labels["win"]);
            l->addWidget(buttons["map"]);
            l->addWidget(buttons["replay_start"]);
            l->addWidget(buttons["replay_end"]);
        }
    }
    centrals["gameend"]->hide();

    centrals["intro"] = new QWidget(this);
    {
        auto l = layouts["intro"] = new QVBoxLayout(centrals["intro"]);
        {
            l->addWidget(labels["intro"]);
            l->addWidget(buttons["map"]);
            l->setAlignment(Qt::AlignCenter);
        }
    }

    layouts["main"] = new QVBoxLayout(this);
    {
        layouts["main"]->setAlignment(Qt::AlignCenter);

        layouts["main"]->addWidget(centrals["play"]);
        layouts["main"]->addWidget(centrals["gameend"]);
        layouts["main"]->addWidget(centrals["intro"]);
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
        for (auto& act : actions["map"]) {
            menus["map"]->addAction(act);
        }

        menus["rec"] = new QMenu("Load recording", this);
        {
            menus["file"]->addMenu(menus["rec"]);
            connect(menus["rec"], SIGNAL(triggered(QAction*)), this, SLOT(loadRecordingMenuTriggered(QAction*)));
        }
        for (auto& act : actions["rec"]) {
            menus["rec"]->addAction(act);
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
    layouts["gameend"]->addWidget(buttons["map"]);

    //buttons["forward"]->setText("Play Backward");
}

void WindowUI::init() {
    initLabels();
    initActions();
    initMenus();
    //refresh();
    initButtons();
    

    view = new GameView(this);
    view->setFrameShape(QFrame::NoFrame);
    view->resize(_mainWindow->_viewWidth, _mainWindow->_viewWidth);

    initLayouts();
}

void WindowUI::loadMapMenuTriggered(QAction* act)
{
    //pr("loadMapMenuTriggered");
    //Iterate through menu actions and find the one that was triggered
    auto acts = menus["map"]->actions();
    auto paths = _path["map"];
    for (int i = 0; i < acts.size(); ++i) {
        if (acts[i] == act) {
            //_mainWindow->cleanup();
            onGameStart(paths[i], false, true);
            return;
        }
    }
    assert(false);
}

void WindowUI::loadRecordingMenuTriggered(QAction* act)
{
    //pr("loadRecordingMenuTriggered");
    //Iterate through menu actions and find the one that was triggered
    auto acts = menus["rec"]->actions();
    auto paths = _path["rec"];
    for (int i = 0; i < acts.size(); ++i) {
        if (acts[i] == act) {
            std::cout << "Ui: recording selected: " << paths[i].toStdString() << std::endl;
            onGameStart(paths[i], true, true);
            return;
        }
    }
    assert(false);
}



void WindowUI::onJumpToStartButtonClick()
{
    _mainWindow->replayJumpTo(true);
}

void WindowUI::onJumpToEndButtonClick()
{
    _mainWindow->replayJumpTo(false);
}

void WindowUI::onReplayDirButtonClick()
{
    bool forw = _mainWindow->toggleReplayDir();
    buttons["replay_dir"]->setText(!forw ? "Play Forward" : "Play Backward");
}

void WindowUI::onGamePause(bool pause)
{
    buttons["pause"]->setText(pause ? "Resume" : "Pause");
}

void WindowUI::onPauseButtonClick() 
{
    //pr("onPauseButtonClick");
    _mainWindow->toggleReplayPaused(); //TODO
}

void WindowUI::onStepBackButtonClick()
{
    _mainWindow->replayStepBack();
}

void WindowUI::onStepNextButtonClick() 
{
    _mainWindow->replayStepNext();
}

void WindowUI::onReplayModeButtonClick()
{
    //bool mode = _mainWindow->_scene->ToggleReplayMode();
    //buttons["replay_mode"]->setText(!mode ? "Step By Step Mode" : "Normal Mode");
    ////buttons["pause"]->setHidden(stepbystep);
    //if (mode) {
    //    buttons["pause"]->hide();
    //} else {
    //    buttons["pause"]->show();
    //}
}


void WindowUI::onReplayFromStartButtonClick()
{
    //pr("onReplayButtonClick");
    
    for (auto& rec : _path["rec"]) {
        if (rec.split("/").last().split(".").first() == currentMapName) {
            std::cout << "UI replay: " << rec.toStdString() << std::endl;
            onGameStart(rec, true, true);
            return;
        }
    }
    ASSERT(false);
}

void WindowUI::onReplayFromEndButtonClick()
{
    for (auto& rec : _path["rec"]) {
        if (rec.split("/").last().split(".").first() == currentMapName) {
            std::cout << "UI replay: " << rec.toStdString() << std::endl;
            onGameStart(rec, true, false);
            return;
        }
    }
    ASSERT(false);
}


void WindowUI::onGameStart(QString filePath, bool isRecorded, bool replayFromStart)
{
    centrals["intro"]->hide();
    centrals["gameend"]->hide();
    centrals["play"]->show();
    if (isRecorded) {
        centrals["replay"]->show();
        buttons["replay_mode"]->setText("Step By Step Mode");
        buttons["pause"]->setText("Pause");
    } else {
        centrals["replay"]->hide();
    }

    currentMapName = filePath.split("/").last().split(".").first();
    _mainWindow->startGame(filePath, isRecorded, replayFromStart);
}


void WindowUI::onGameEnd(bool win, int score, int steps)
{
    if (win) {
        labels["win"]->setText("You won!\nScore: " + QString::number(score));
    } else {
        labels["win"]->setText("You lost :(\nScore: " + QString::number(score) + "\nSteps: " + QString::number(steps));
    }

    centrals["play"]->hide();
    centrals["replay"]->hide();
    centrals["gameend"]->show();

    refresh();
}


void WindowUI::onPlayerScoreChanged(int score)
{
    labels["score"]->setText("Score: " + QString::number(score));
}

void WindowUI::onPlayerStepsChanged(int steps)
{
    labels["steps"]->setText("Steps: " + QString::number(steps));
}
