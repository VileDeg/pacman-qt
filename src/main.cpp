/** @file main.cpp
 *  @author Vadim Goncearenco <xgonce00@stud.fit.vutbr.cz>
 *  @brief Program entry point.
 *  @details This file contains main function definition.
 *  Main function is only responsible for creating QT application and main window class.
 *  An error callback is passed to main window class, to handle errors that occur during construction.
 */

#include <QApplication>
#include <QString>

#include "mainwindow.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    auto errorCallback = [](QString message, int errorCode) {
        qCritical() << "Error[" << errorCode << "]: " << message;
        QApplication::exit(errorCode);
    };

    int ret = 0;
    QApplication a(argc, argv);
    MainWindow w(errorCallback);
    
    w.show();
    
    ret = a.exec();

    return ret;
}
