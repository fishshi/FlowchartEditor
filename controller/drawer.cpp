#include "drawer.h"
#include "../model/elements/processelement.h"
#include "../model/elements/decisionelement.h"
#include "../model/elements/startendelement.h"
#include "../model/elements/connectorelement.h"
#include "../model/elements/line.h"
#include "../model/elements/dataelement.h"

Drawer::Drawer(Canvas *canvas) {
    this->canvas = canvas;
}

void Drawer::setPaintChart()
{
    if(curPaintChart != nullptr)
        delete curPaintChart;

    switch(curPaintChartType)
    {
    case PaintChartType::RECT:
    {
        curPaintChart = new ProcessElement(canvas);
    }break;
    case PaintChartType::DIAMOND:
    {
        curPaintChart = new DecisionElement(canvas);
    }break;
    case PaintChartType::ROUNDRECT:
    {
        curPaintChart = new StartEndElement(canvas);
    }break;
    case PaintChartType::ELLIPSE:
    {
        curPaintChart = new ConnectorElement(canvas);
    }break;
    case PaintChartType::LINE:
    {
        curPaintChart = new Line(canvas);
    }break;
    case PaintChartType::PARALLELOGRAM:
    {
        curPaintChart = new DataElement(canvas);
    }break;
    default:case PaintChartType::NONE:{
        curPaintChart = nullptr;
    }break;
    }
}
