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
    labels.score = new QLabel(this);
    {
        labels.score->setStyleSheet("font-family: Fixedsys; color: white; font-size: 20px;");
        //_scoreLabel->setText("Score: ");
        labels.score->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }

    labels.win = new QLabel(this);
    {
        labels.win->setStyleSheet("font-family: Fixedsys; color: white; font-size: 60px;");
        //labels.win->setText("You won!\nScore: ");
        //labels.win->setWordWrap(true);
        labels.win->setAlignment(Qt::AlignCenter);
    }
}

void WindowUI::initActions()
{
    {
        QDirIterator it(":/maps/", QStringList() << "*.txt", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            qDebug() << it.next();
            QAction* act = new QAction(it.filePath(), this);
            actions.map.append(act);
            _path.map.append(it.filePath());
        }
    }

    QDirIterator it("saves/", QStringList() << "*.bin", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        qDebug() << it.next();
        QAction* act = new QAction(it.filePath(), this);
        actions.recording.append(act);
        _path.recording.append(it.filePath());
    }
}

void WindowUI::initButtons()
{
    buttons.loadMap = new QPushButton("Load map", this);
    {
        buttons.loadMap->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        //buttons.loadMap->addAction(menus.loadMap->menuAction());
        //connect(buttons.loadMap, SIGNAL(clicked()), this, SLOT(loadMapMenuTriggered()));
        buttons.loadMap->setMenu(menus.loadMap);
    }
}

void WindowUI::initLayouts() {
    mapCentral = new QWidget(this);
    {
        layouts.map = new QVBoxLayout(this);
        {
            layouts.map->setAlignment(Qt::AlignCenter);

            layouts.map->addWidget(labels.score);
            layouts.map->addWidget(view);
        }
    }
    mapCentral->setLayout(layouts.map);

    otherCentral = new QWidget(this);
    {
        layouts.other = new QVBoxLayout(this);
        {
            layouts.other->setAlignment(Qt::AlignCenter);

            layouts.other->addWidget(labels.win);
            layouts.other->addWidget(buttons.loadMap);
        }
    }
    otherCentral->setLayout(layouts.other);
    otherCentral->hide();

    layouts.main = new QVBoxLayout(this);
    {
        layouts.main->setAlignment(Qt::AlignCenter);

        layouts.main->addWidget(mapCentral);
        layouts.main->addWidget(otherCentral);
    }
}

void WindowUI::initMenus() {
    menus.file = _mainWindow->menuBar()->addMenu("File");
    {
        menus.loadMap = new QMenu("Load map", this);
        {
            menus.file->addMenu(menus.loadMap);
            connect(menus.loadMap, SIGNAL(triggered(QAction*)), this, SLOT(loadMapMenuTriggered(QAction*)));

            for (auto& act : actions.map) {
                menus.loadMap->addAction(act);
            }
        }

        menus.loadRecording = new QMenu("Load recording", this);
        {
            menus.file->addMenu(menus.loadRecording);
            connect(menus.loadRecording, SIGNAL(triggered(QAction*)), this, SLOT(loadRecordingMenuTriggered(QAction*)));

            for (auto& act: actions.recording) {
                menus.loadRecording->addAction(act);
            }
        }
    }
}

void WindowUI::init() {
    initLabels();
    initActions();
    initMenus();
    initButtons();

    view = new GameView(this);

    initLayouts();
    setLayout(layouts.main);
}

void WindowUI::loadMapMenuTriggered(QAction* act)
{
    pr("loadMapMenuTriggered");
    //Iterate through menu actions and find the one that was triggered
    auto acts = menus.loadMap->actions();
    auto paths = _path.map;
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
    auto acts = menus.loadRecording->actions();
    auto paths = _path.recording;
    for (int i = 0; i < acts.size(); ++i) {
        if (acts[i] == act) {
            //_mainWindow->cleanup();
            _mainWindow->startGame(paths[i], true);
            return;
        }
    }
    assert(false);
}

void WindowUI::onGameEnd(bool win, int score)
{
    if (win) {
        
        labels.win->setText("You won!\nScore: " + QString::number(score));
        //buttons.loadMap->hide();
    } else {
        labels.win->setText("You lost :(\nScore: " + QString::number(score));
    }

    mapCentral->hide();
    otherCentral->show();
}


void WindowUI::onUpdateGameScore(int score)
{
    labels.score->setText("Score: " + QString::number(score));
}