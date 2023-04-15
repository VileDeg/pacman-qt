/** @file ui.cpp
 *  @author Vadim Goncearenco <xgonce00@stud.fit.vutbr.cz>
 *  @brief File with definition of window UI class.
 *  @details This file contains definition of window UI class.
 */

#include <QMenuBar>
#include <QDirIterator>

#include "ui.h"
#include "gameview.h"
#include "mainwindow.h"
#include "utils.h"

#define BTN_CREATE(_key, _name, _parent)\
    b = buttons[_key] = new QPushButton(_name, _parent)

#define BTN_CONNECT(_recv, _slot) connect(b, SIGNAL(clicked()), _recv, SLOT(_slot))

#define SAVES_FOLDER "examples/"
#define INTRO_STR\
    "How to play:\n"\
    "You can load a map by clicking the button below.\n"\
    "These are the maps from \"" SAVES_FOLDER "\" folder.\n"\
    "After the game ends, you can replay it.\n\n"\
    "You can also load a map or a recording from the \"File\" menu above."

WindowUI::WindowUI(MainWindow* mainWindow)
    : QWidget(qobject_cast<QWidget*>(mainWindow)), _mainWindow(mainWindow)
{
    init();
}

void WindowUI::initLabels() {
    QFont font("Fixedsys", 12);
    QString sheet = "color: white;";
    auto l = labels["score"] = new QLabel(this);
    {
        l->setStyleSheet(sheet);
        l->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        l->setText("Score: ");
        l->setFont(font);
    }

    l = labels["steps"] = new QLabel(this);
    {
        l->setStyleSheet(sheet);
        l->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        l->setText("Steps: ");
        l->setFont(font);
    }

    l = labels["win"] = new QLabel(this);
    {
        l->setStyleSheet(sheet);
        l->setAlignment(Qt::AlignCenter);
        l->setFont(font);
    }

    l = labels["intro"] = new QLabel(this);
    {
        l->setStyleSheet(sheet);
        l->setAlignment(Qt::AlignCenter);
        l->setText(INTRO_STR);
        l->setFont(font);
    }
}

void WindowUI::initActions()
{
    {
        actions["map"].clear();
        _path["map"].clear();

        QDirIterator it("examples/", { "*.txt" }, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString filePath = it.next();
            if (filePath.isEmpty()) {
                qDebug() << "Empty file path!";
                continue;
            }
            // Do something with the file
            QAction* act = new QAction(it.fileName(), this);
            actions["map"].append(act);
            _path["map"].append(filePath);
        }
    }
    {
        actions["rec"].clear();
        _path["rec"].clear();

        QDirIterator it("saves/", { "*.bin" }, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString filePath = it.next();
            if (filePath.isEmpty()) {
                qDebug() << "Empty file path!";
                continue;
            }
            // Do something with the file
            QAction* act = new QAction(it.fileName(), this);
            actions["rec"].append(act);
            _path["rec"].append(filePath);
        }
    }
}

void WindowUI::initButtons()
{
    QPushButton* b;
    BTN_CREATE("map", "Load map", this);
    {
        b->setMenu(menus["map"]);
    }

    BTN_CREATE("rec", "Load recording", this);
    {
        b->setMenu(menus["rec"]);
    }

    BTN_CREATE("retry", "Try again", this);
    BTN_CONNECT(this, onRetryButtonClick());


    BTN_CREATE("replay_start", "Replay from start", this);
    BTN_CONNECT(this, onReplayFromStartButtonClick());

    BTN_CREATE("replay_end", "Replay from end", this);
    BTN_CONNECT(this, onReplayFromEndButtonClick());

    
    BTN_CREATE("pause", "Pause", this);
    BTN_CONNECT(_mainWindow->getSerializer(), toggleReplayPaused());

    BTN_CREATE("next", "Step Forward", this);
    BTN_CONNECT(_mainWindow->getSerializer(), replayStepNext());

    BTN_CREATE("back", "Step Back", this);
    BTN_CONNECT(_mainWindow->getSerializer(), replayStepBack());

    BTN_CREATE("replay_mode", "Step by Step Mode", this);
    BTN_CONNECT(_mainWindow->getSerializer(), toggleReplayMode());


    BTN_CREATE("jump_start", "Jump to Start", this);
    BTN_CONNECT(_mainWindow->getSerializer(), replayJumpToStart());

    BTN_CREATE("jump_end", "Jump to End", this);
    BTN_CONNECT(_mainWindow->getSerializer(), replayJumpToEnd());


    BTN_CREATE("replay_dir", "Play Backward", this);
    BTN_CONNECT(_mainWindow->getSerializer(), toggleReplayDir());
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

            l->addWidget(buttons["retry"]);
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
            l->addWidget(buttons["rec"]);
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
    
    auto mb = _mainWindow->menuBar();
    QPalette menuPalette = mb->palette();
    menuPalette.setColor(QPalette::WindowText, Qt::white);
    mb->setPalette(menuPalette);
    mb->setStyleSheet("QMenuBar { background-color: black; color: white; }");

    menus["file"] = mb->addMenu("File");
    {
        menus["map"] = new QMenu("Load map", this);
        {
            menus["file"]->addMenu(menus["map"]);
            connect(menus["map"], SIGNAL(triggered(QAction*)), this, SLOT(onLoadMapMenuTriggered(QAction*)));
        }
        for (auto& act : actions["map"]) {
            menus["map"]->addAction(act);
        }

        menus["rec"] = new QMenu("Load recording", this);
        {
            menus["file"]->addMenu(menus["rec"]);
            connect(menus["rec"], SIGNAL(triggered(QAction*)), this, SLOT(onLoadRecordingMenuTriggered(QAction*)));
        }
        for (auto& act : actions["rec"]) {
            menus["rec"]->addAction(act);
        }
    }
}

void WindowUI::refresh()
{
    _state = GameState();
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
    layouts["gameend"]->addWidget(buttons["rec"]);

    buttons["replay_mode"]->setText("Step By Step Mode");
    buttons["replay_dir"]->setText("Play Backward");
}

void WindowUI::init() {
    initLabels();
    initActions();
    initMenus();
    initButtons();
    

    view = new GameView(this);
    view->setFrameShape(QFrame::NoFrame);
    view->resize(_mainWindow->_viewWidth, _mainWindow->_viewWidth);

    initLayouts();
}

void WindowUI::onLoadMapMenuTriggered(QAction* act)
{
    //Iterate through menu actions and find the one that was triggered
    auto acts = menus["map"]->actions();
    auto paths = _path["map"];
    for (int i = 0; i < acts.size(); ++i) {
        if (acts[i] == act) {
            onGameStart(paths[i], false, true);
            return;
        }
    }
    ASSERT(false);
}

void WindowUI::onLoadRecordingMenuTriggered(QAction* act)
{
    //Iterate through menu actions and find the one that was triggered
    auto acts = menus["rec"]->actions();
    auto paths = _path["rec"];
    for (int i = 0; i < acts.size(); ++i) {
        if (acts[i] == act) {
            onGameStart(paths[i], true, true);
            return;
        }
    }
    ASSERT(false);
}

void WindowUI::onReplayFlagsChanged(ReplayFlags rf)
{
    buttons["replay_dir"]->setText(rf.Forward ? "Play Backward" : "Play Forward");
    buttons["pause"]->setText(rf.Paused ? "Resume" : "Pause");

    if (rf.StepByStep) {
        buttons["back"]->hide();
        buttons["pause"]->hide();
        buttons["next"]->hide();
    } else {
        buttons["back"]->show();
        buttons["pause"]->show();
        buttons["next"]->show();
    }
    buttons["replay_mode"]->setText(rf.StepByStep ? "Normal Mode" : "Step By Step Mode");
}

void WindowUI::onGameStateChanged(GameState gs)
{
    if (gs.gameOver && _state.gameOver != gs.gameOver) {
        onGameEnd(gs);
    } 
    if (_state.score != gs.score) {
        labels["score"]->setText("Score: " + QString::number(gs.score));
    }
    if (_state.steps != gs.steps) {
        labels["steps"]->setText("Steps: " + QString::number(gs.steps));
    }
    _state = gs;
}

void WindowUI::onRetryButtonClick()
{
    for (auto& map : _path["map"]) {
        if (map.split("/").last().split(".").first() == currentMapName) {
            onGameStart(map, false, true);
            return;
        }
    }
    ASSERT(false);
}

void WindowUI::onReplayFromStartButtonClick()
{
    for (auto& rec : _path["rec"]) {
        if (rec.split("/").last().split(".").first() == currentMapName) {
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

void WindowUI::onGameEnd(GameState ged)
{
    pr("UI on game end");
    QString text = ged.win ? "You won!" : "You lost :(";
    labels["win"]->setText(text + "\nScore: " + QString::number(ged.score) + "\nSteps: " + QString::number(ged.steps));

    centrals["play"]->hide();
    centrals["replay"]->hide();
    centrals["gameend"]->show();

    refresh();
}
