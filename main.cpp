#include <QApplication>

#include "mainwindow.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    int ret = 0;

    try {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        ret = a.exec();
    } catch (std::exception& e) {
        errpr(e.what());
        return 1;
    }

    return ret;
}
