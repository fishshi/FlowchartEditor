#ifndef UPDATER_H
#define UPDATER_H

#include "../view/canvas.h"

class Updater
{
public:
    Updater(Canvas *canvas);

    void setSelChartLineColor(const QColor &color);
    void setSelChartFillColor(const QColor &color);

    void moveToChangeSize(int x, int y);
    void moveToChangePos(int x, int y);
    void setSelecChart(FlowchartElement * cb, int x, int y);

private:
    Canvas *canvas;
};

#endif // UPDATER_H
