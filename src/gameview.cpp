/** @file gameview.cpp
 *  @author Vadim Goncearenco <xgonce00@stud.fit.vutbr.cz>
 *  @brief File with game view class definition.
 *  @details This file contains definition for game view class.
 */

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
   
    setStyleSheet("background-color:black;");
}

GameView::~GameView() {}
