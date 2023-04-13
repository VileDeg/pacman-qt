######################################################################
# Automatically generated by qmake (3.1) Thu Apr 13 23:53:05 2023
######################################################################

QT += core gui widgets
QMAKE_CXXFLAGS += -std=c++17

QMAKE_CXXFLAGS_WARN_ON = -Wall

TEMPLATE = app
TARGET = pacman-new
INCLUDEPATH += src




# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += src/animatedsprite.h \
           src/astar.h \
           src/enemy.h \
           src/gamescene.h \
           src/gameview.h \
           src/interfaces.h \
           src/mainwindow.h \
           src/player.h \
           src/replay.h \
           src/sprite.h \
           src/ui.h \
           src/utils.h
SOURCES += src/animatedsprite.cpp \
           src/astar.cpp \
           src/enemy.cpp \
           src/gamescene.cpp \
           src/gameview.cpp \
           src/main.cpp \
           src/mainwindow.cpp \
           src/player.cpp \
           src/replay.cpp \
           src/sprite.cpp \
           src/ui.cpp \
           src/utils.cpp
RESOURCES += resource.qrc
