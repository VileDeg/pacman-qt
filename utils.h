#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <QDebug>
#include <QPixmap>

//#define _fdpr(_msg, _fd) _fd << _msg << std::endl
//#define errpr(_msg) _fdpr("Error: " << _msg, std::cerr)
//#define pr(_msg) _fdpr(_msg, std::cout)
#define errpr(_msg) qCritical() << _msg
#define pr(_msg) qDebug() << _msg
#define vpr(_v) pr(#_v << ": " << _v)
#define v2pr(_v) pr(#_v << ": (" << _v.x() << "," << _v.y() << ")")


QImage loadPixmap(QString path);
QImage setImageBrightness(QImage& img, int brightness);


#endif // UTILS_H

