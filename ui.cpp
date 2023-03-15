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

void WindowUI::initButtons()
{
    auto b = buttons["map"] = new QPushButton("Load map", this);
    {
        b->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        b->setMenu(menus["map"]);
    }
    b = buttons["replay"] = new QPushButton("Replay", this);
    {
        b->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        connect(b, SIGNAL(clicked()), this, SLOT(onReplayButtonClick()));
    }
    b = buttons["tbreplay"] = new QPushButton("Replay", this);
    {
        //b->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        connect(b, SIGNAL(clicked()), this, SLOT(onReplayButtonClick()));
    }
    b = buttons["tbback"] = new QPushButton("Back", this);
    {
        //b->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        //connect(b, SIGNAL(clicked()), this, SLOT(onReplayButtonClick()));
    }
    b = buttons["tbnext"] = new QPushButton("Next", this);
    {
        //b->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        //connect(b, SIGNAL(clicked()), this, SLOT(onReplayButtonClick()));
    }
}

void WindowUI::initLayouts() {
    mapCentral = new QWidget(this);
    {
        layouts["map"] = new QVBoxLayout(this);
        {
            layouts["map"]->setAlignment(Qt::AlignCenter);

            layouts["map"]->addWidget(labels["score"]);
            layouts["map"]->addWidget(view);
            layouts["map"]->addWidget(toolbar);
        }
    }
    mapCentral->setLayout(layouts["map"]);

    otherCentral = new QWidget(this);
    {
        layouts["other"] = new QVBoxLayout(this);
        {
            layouts["other"]->setAlignment(Qt::AlignCenter);

            layouts["other"]->addWidget(labels["win"]);
            layouts["other"]->addWidget(buttons["map"]);
            layouts["other"]->addWidget(buttons["replay"]);
        }
    }
    otherCentral->setLayout(layouts["other"]);
    otherCentral->hide();

    layouts["main"] = new QVBoxLayout(this);
    {
        layouts["main"]->setAlignment(Qt::AlignCenter);

        layouts["main"]->addWidget(mapCentral);
        layouts["main"]->addWidget(otherCentral);
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
    toolbar = new QToolBar(this);
    _mainWindow->addToolBar(Qt::LeftToolBarArea, toolbar);
    toolbar->setOrientation(Qt::Vertical);
    toolbar->addWidget(buttons["tbback"]);
    toolbar->addWidget(buttons["tbreplay"]);
    toolbar->addWidget(buttons["tbnext"]);
    
    initLabels();
    initActions();
    initMenus();
    initButtons();
    refresh();

    view = new GameView(this);
    view->setFrameShape(QFrame::NoFrame);
    view->resize(_mainWindow->_viewWidth, _mainWindow->_viewWidth);

    initLayouts();
    setLayout(layouts["main"]);
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
            _mainWindow->startGame(paths[i], false);
            return;
        }
    }
    assert(false);
}

void WindowUI::loadRecordingMenuTriggered(QAction* act)
{
    pr("loadRecordingMenuTriggered");
    //Iterate through menu actions and find the one that was triggered
    auto acts =menus["rec"]->actions();
    auto paths = _path["rec"];
    for (int i = 0; i < acts.size(); ++i) {
        if (acts[i] == act) {
            //_mainWindow->cleanup();
            _mainWindow->startGame(paths[i], true);
            return;
        }
    }
    assert(false);
}

void WindowUI::onReplayButtonClick()
{
    pr("onReplayButtonClick");
    for (auto& rec : _path["rec"]) {
        if (rec.split("/").last().split(".").first() == currentMapName) {
            _mainWindow->startGame(rec, true);
            return;
        }
    }
    ASSERT(false);
}

void WindowUI::onGameEnd(bool win, int score)
{
    if (win) {
        labels["win"]->setText("You won!\nScore: " + QString::number(score));
    } else {
        labels["win"]->setText("You lost :(\nScore: " + QString::number(score));
    }

    mapCentral->hide();
    otherCentral->show();

    refresh();
}


void WindowUI::onUpdateGameScore(int score)
{
    labels["score"]->setText("Score: " + QString::number(score));
}