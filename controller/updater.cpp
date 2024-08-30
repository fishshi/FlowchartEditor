#include "updater.h"

Updater::Updater(Canvas *canvas) {
    this->canvas = canvas;
}

void Updater::setSelChartLineColor(const QColor &color)
{
    canvas->curSelecChart->paintChartDrawPen.setColor(color);
    canvas->curSelecChart->update();
}

void Updater::setSelChartFillColor(const QColor &color)
{
    canvas->curSelecChart->paintChartFillPen.setColor(color);
    canvas->curSelecChart->update();
}
