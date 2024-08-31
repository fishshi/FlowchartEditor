#include "updater.h"

Updater::Updater(Canvas *canvas) {
    this->canvas = canvas;
}

void Updater::setSelecChart(FlowchartElement * cb, int x, int y)
{
    canvas->grabKeyboard();
    if(canvas->curSelecChart != nullptr) canvas->curSelecChart->hideMagSize();
    canvas->curSelecChart = cb;
    canvas->curSelecChart->showMagSize();
    canvas->curSelecChartPos = QPoint(x,y);
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

void Updater::moveToChangeSize(int x, int y)
{
    if(canvas->curSelecChart->chartType == PaintChartType::LINE)
        for(auto it = canvas->charts.begin();it!=canvas->charts.end();it++)
        {
            if((*it)->autoSetMagi(x,y,canvas->magPointIndex))
            {
                canvas->lineSelectChart = *it;
                break;
            }
            else
                canvas->lineSelectChart = nullptr;
        }
    canvas->curSelecChart->setWidthHeight(x,y,canvas->sizePointDirect);
}

void Updater::moveToChangePos(int x, int y)
{
    if(canvas->curSelecChart->chartType != PaintChartType::LINE)
        canvas->curSelecChart->setXY(x - canvas->curSelecChartPos.rx(), y - canvas->curSelecChartPos.ry());
}
