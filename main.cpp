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
    MainWindow w(&a, errorCallback);
    
    w.show();
    
    ret = a.exec();

    return ret;
}
