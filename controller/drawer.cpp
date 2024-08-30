#include "drawer.h"

Drawer::Drawer(Canvas *canvas) {
    this->canvas = canvas;
}

void Drawer::setPaintChart()
{
    if(canvas->curPaintChart != nullptr)
        delete canvas->curPaintChart;
    switch(canvas->curPaintChartType)
    {
    case PaintChartType::RECT:
        canvas->curPaintChart = new ProcessElement(canvas);
        break;
    case PaintChartType::DIAMOND:
        canvas->curPaintChart = new DecisionElement(canvas);
        break;
    case PaintChartType::ROUNDRECT:
        canvas->curPaintChart = new StartEndElement(canvas);
        break;
    case PaintChartType::ELLIPSE:
        canvas->curPaintChart = new ConnectorElement(canvas);
        break;
    case PaintChartType::LINE:
        canvas->curPaintChart = new Line(canvas);
        break;
    case PaintChartType::PARALLELOGRAM:
        canvas->curPaintChart = new DataElement(canvas);
        break;
    case PaintChartType::NONE:
        canvas->curPaintChart = nullptr;
        break;
    }
    if(canvas->curPaintChart)
        canvas->mouseEventType = MOUSE_EVENT_TYPE::CREATING_CNANGE_SIZE;
}
