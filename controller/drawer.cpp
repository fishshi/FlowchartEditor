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
}

void Drawer::clickToCreate(int x, int y)
{
    if(canvas->curSelecChart)
        canvas->curSelecChart->hideMagSize();
    canvas->curSelecChart = canvas->curPaintChart;
    canvas->curPaintChart->setXY(x,y);
    if(canvas->curPaintChart->chartType == PaintChartType::LINE)
        canvas->line.push_back(canvas->curPaintChart);
    else
        canvas->charts.push_back(canvas->curPaintChart);
    canvas->curPaintChart->update();
    canvas->curPaintChart->show();
}

void Drawer::moveToCreate(int x, int y)
{
    if(canvas->curSelecChart)
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
        canvas->curPaintChart->setWidthHeight(x,y);
    }
}

int Drawer::moveToLink(int x, int y)
{
    int flag = 0;
    if(canvas->newLineChart == nullptr)
    {
        canvas->newLineChart = new Line(canvas);
        if(canvas->curSelecChart)
            canvas->curSelecChart->hideMagSize();
        canvas->curSelecChart = canvas->newLineChart;
        canvas->line.push_back(canvas->newLineChart);
        canvas->newLineChart->setXY(canvas->newLineFromSelectChart->getMagiPointAbsX(canvas->magPointFromIndex),canvas->newLineFromSelectChart->getMagiPointAbsY(canvas->magPointFromIndex));
        canvas->newLineChart->setStartChart(canvas->newLineFromSelectChart);
        canvas->newLineChart->setStartMagIndex(canvas->magPointFromIndex);
        canvas->newLineChart->setStartDirect(canvas->magPointDirect);
        canvas->newLineChart->update();
        canvas->newLineChart->show();
        canvas->newLineFromSelectChart->addMagiPointStartLine(canvas->magPointFromIndex, canvas->newLineChart);
        flag = 1;
    }

    for(auto it = canvas->charts.begin();it!= canvas->charts.end();it++)
    {
        if((*it)->autoSetMagi(x,y, canvas->magPointToIndex))
        {
            canvas->newLineToSelectChart = *it;
            break;
        }else{
            canvas->newLineToSelectChart = nullptr;
        }
    }
    canvas->newLineChart->setWidthHeight(x,y,ORIENTION::ENDPOINT);
    return flag;
}
