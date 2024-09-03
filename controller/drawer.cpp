#include "drawer.h"

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
        curPaintChart = new ProcessElement(canvas);
        break;
    case PaintChartType::DIAMOND:
        curPaintChart = new DecisionElement(canvas);
        break;
    case PaintChartType::ROUNDRECT:
        curPaintChart = new StartEndElement(canvas);
        break;
    case PaintChartType::ELLIPSE:
        curPaintChart = new ConnectorElement(canvas);
        break;
    case PaintChartType::PARALLELOGRAM:
        curPaintChart = new DataElement(canvas);
        break;
    case PaintChartType::NONE:
        curPaintChart = nullptr;
        break;
    }
}

void Drawer::moveToCreate(int x, int y)
{
    curPaintChart->setXY(x, y);
    if(!curPaintChart->isVisible())
        curPaintChart->show();
    else
        curPaintChart->update();
}

int Drawer::moveToLink(int x, int y)
{
    int flag = 0;
    if(newLineChart == nullptr)
    {
        newLineChart = new Line(canvas);
        if(canvas->curSelecChart)
            canvas->curSelecChart->hideMagSize();
        canvas->curSelecChart = newLineChart;
        canvas->line.push_back(newLineChart);
        newLineChart->setXY(newLineFromSelectChart->getMagiPointAbsX(magPointFromIndex),newLineFromSelectChart->getMagiPointAbsY(magPointFromIndex));
        newLineChart->setStartChart(newLineFromSelectChart);
        newLineChart->setStartMagIndex(magPointFromIndex);
        newLineChart->setStartDirect(magPointDirect);
        newLineChart->update();
        newLineChart->show();
        newLineFromSelectChart->addMagiPointStartLine(magPointFromIndex, newLineChart);
        flag = 1;
    }

    for(auto it = canvas->charts.begin();it!= canvas->charts.end();it++)
    {
        if((*it)->autoSetMagi(x,y, magPointToIndex))
        {
            newLineToSelectChart = *it;
            break;
        }else{
            newLineToSelectChart = nullptr;
        }
    }
    newLineChart->setWidthHeight(x,y,ORIENTION::ENDPOINT);
    return flag;
}

void Drawer::doneCreate()
{
    curPaintChartType = PaintChartType::NONE;
    curPaintChart->updateMagPointInfo();
    canvas->charts.push_back(curPaintChart);
    canvas->curSelecChart = curPaintChart;
    curPaintChart = nullptr;
}

void Drawer::doneLink()
{
    if(newLineToSelectChart)
    {
        newLineToSelectChart->addMagiPointEndLine(magPointToIndex, newLineChart);
        newLineToSelectChart->hideMagOnly();
        newLineChart->setEndChart(newLineToSelectChart);
        newLineChart->setEndMagIndex(magPointToIndex);
        newLineChart->setEndDirect(newLineToSelectChart->getMagiPointDirect(magPointToIndex));
        newLineChart->update();
    }else
        if(newLineChart)
            newLineChart->resetEndChart();
    newLineChart = nullptr;
    newLineFromSelectChart = nullptr;
    newLineToSelectChart = nullptr;
}
