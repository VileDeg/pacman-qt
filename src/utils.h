#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <QDebug>
#include <QPixmap>

#define errpr(_msg) qCritical() << _msg
#define pr(_msg) qDebug() << _msg
#define vpr(_v) pr(#_v << ": " << _v)
#define v2pr(_v) pr(#_v << ": (" << _v.x() << "," << _v.y() << ")")

#define HHPR(_msg, _type, _file) do{ _file << "[" << __FILE__ << " " << __func__ << " " << __LINE__ << "] " << _type << ": " << _msg << std::endl; }while(0)
#define HPR(_msg, _type) HHPR(_msg, _type, std::cerr)
#define HPRINFO(_msg, _type) HHPR(_msg, _type, std::cout)

#define PRERR(_msg) HPR(_msg, "Error");
#define PRWRN(_msg) HPR(_msg, "Warning");
#define PRABRT(_msg) HPR(_msg, "Abort");

#define PRINF(_msg) HPRINFO(_msg, "Info");

//Macro for breaking into the debugger or aborting the program
#define NDEBUG
#ifdef NDEBUG
    #define TRAP() 
#else
    #ifdef _WIN32
        #ifdef _MSC_VER
            #define TRAP() __debugbreak()
        #else
            #include <intrin.h>
            #define TRAP() __debugbreak()
        #endif // _MSC_VER
    #else
        #include <csignal>
        #define TRAP() raise(SIGTRAP)
    #endif // _WIN32
#endif // NDEBUG

//Assert macros for boolean expressions
#define HASSERT(_x, _tru) do{ if ((_x) != _tru) { TRAP(); } }while(0)
#define HASSERTMSG(_x, _tru, _msg) do{ if ((_x) != _tru) { PRERR(_msg); TRAP(); } }while(0)

//Assert macros for functions returning bool
#define ASSERT(_x) HASSERT(_x, true)
#define ASSERTMSG(_x, _msg) HASSERTMSG(_x, true, _msg)

#define V2PR(_v) " " << #_v << ": " << _v.x() << ", " << _v.y() << " "


QImage loadPixmap(QString path);
QImage setImageBrightness(QImage& img, int brightness);


#endif // UTILS_H

