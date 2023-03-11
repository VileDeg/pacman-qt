#include "gameview.h"

GameView::GameView(QWidget *parent)
    : QGraphicsView(parent)
{
    Init();
}

void GameView::Init()
{
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setCacheMode(QGraphicsView::CacheNone);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

GameView::~GameView() {}
