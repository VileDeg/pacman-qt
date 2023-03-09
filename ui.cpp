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
        labels.win->setText("You won!\nScore: ");
        labels.win->setWordWrap(true);
        labels.win->setAlignment(Qt::AlignCenter);
    }
}
void WindowUI::initLayouts() {
    mapCentral = new QWidget(this);
    {
        layouts.map = new QVBoxLayout(this);
        {
            layouts.map->addWidget(labels.score);
            layouts.map->addWidget(view);
            layouts.map->setAlignment(Qt::AlignCenter);
        }
    }
    mapCentral->setLayout(layouts.map);

    otherCentral = new QWidget(this);
    {
        layouts.other = new QVBoxLayout(this);
        {
            layouts.other->addWidget(labels.win);
            layouts.other->setAlignment(Qt::AlignCenter);
        }
    }
    otherCentral->setLayout(layouts.other);
    otherCentral->hide();

    layouts.main = new QVBoxLayout(this);
    {
        layouts.main->addWidget(mapCentral);
        layouts.main->addWidget(otherCentral);
        layouts.main->setAlignment(Qt::AlignCenter);
    }
}

void WindowUI::initMenus() {
    menus.loadMap = new QMenu("Load map", this);
    menus.file = _mainWindow->menuBar()->addMenu("File");
    menus.file->addMenu(menus.loadMap);
    connect(menus.loadMap, SIGNAL(triggered(QAction*)), this, SLOT(loadMapMenuTriggered(QAction*)));


    QDirIterator it(":/maps/", QStringList() << "*.txt", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        qDebug() << it.next();
        QAction* act = new QAction(it.filePath(), this);
        menus.loadMap->addAction(act);
        _mapsPath.append(it.filePath());
    }
}

void WindowUI::init() {
    initLabels();

    view = new GameView(this);

    initLayouts();

    setLayout(layouts.main);

    initMenus();
}

void WindowUI::loadMapMenuTriggered(QAction* act)
{
    pr("loadMapMenuTriggered");
    //Iterate through menu actions and find the one that was triggered
    for (int i = 0; i < menus.loadMap->actions().size(); ++i) {
        if (menus.loadMap->actions()[i] == act) {
            _mainWindow->endGame();
            _mainWindow->startGame(_mapsPath[i]);
            return;
        }
    }
    assert(false);
}

void WindowUI::onPlayerWin(int score)
{
    labels.win->setText("You won!\nScore: " + QString::number(score));

    mapCentral->hide();
    otherCentral->show();
}

void WindowUI::onUpdateGameScore(int score)
{
    labels.score->setText("Score: " + QString::number(score));
}