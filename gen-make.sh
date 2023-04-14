FILE=src/Makefile
PROJ=pacman.pro
FLAGS=OBJECTS_DIR=../ DESTDIR=../ TARGET=pacman INCLUDEPATH+=src/

qmake -o $FILE $FLAGS $PROJ