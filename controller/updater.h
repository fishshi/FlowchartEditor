#ifndef UPDATER_H
#define UPDATER_H

#include "../view/canvas.h"

class Updater
{
public:
    Updater(Canvas *canvas);

    void setSelChartLineColor(const QColor &color);
    void setSelChartFillColor(const QColor &color);

private:
    Canvas *canvas;
};

#endif // UPDATER_H
