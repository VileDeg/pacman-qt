/** @file utils.cpp
 *  @author Vadim Goncearenco <xgonce00@stud.fit.vutbr.cz>
 *  @brief File with utility function definition.
 *  @details This file contains utility function definition.
 */

#include "utils.h"

QImage loadPixmap(QString path)
{
    QImage tmp(path);
    if (tmp.isNull()) {
        throw std::runtime_error("failed to open pixmap" + path.toStdString());
    }
    return tmp;
}

QImage setImageBrightness(QImage& img, int brightness)
{
    for (int i = 0; i < img.width(); ++i) {
        for (int j = 0; j < img.height(); ++j) {
            QColor c = img.pixelColor(i, j);
            c.setRed(std::clamp(c.red() * brightness / 100, 0, 255));
            c.setGreen(std::clamp(c.green() * brightness / 100, 0, 255));
            c.setBlue(std::clamp(c.blue() * brightness / 100, 0, 255));
            img.setPixelColor(i, j, c);
        }
    }

    return img;
}
