#include "utils.h"

QPixmap loadPixmap(QString path)
{
    QPixmap tmp(path);
    if (tmp.isNull()) {
        throw std::runtime_error("failed to open pixmap" + path.toStdString());
    }
    return tmp;
}