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
    case PaintChartType::LINE:
        curPaintChart = new Line(canvas);
        break;
    case PaintChartType::PARALLELOGRAM:
        curPaintChart = new DataElement(canvas);
        break;
    case PaintChartType::NONE:
        curPaintChart = nullptr;
        break;
    }
}

void Drawer::clickToCreate(int x, int y)
{
    if(canvas->curSelecChart)
        canvas->curSelecChart->hideMagSize();
    canvas->curSelecChart = curPaintChart;
    curPaintChart->setXY(x,y);
    if(curPaintChart->chartType == PaintChartType::LINE)
        canvas->line.push_back(curPaintChart);
    else
        canvas->charts.push_back(curPaintChart);
    curPaintChart->update();
    curPaintChart->show();
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
        curPaintChart->setWidthHeight(x,y);
    }
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
        if((*it)->autoSetMagi(x,y, canvas->magPointToIndex))
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
    if(canvas->curSelecChart->chartType == PaintChartType::LINE)
    {
        Line *cl = reinterpret_cast<Line *>(canvas->curSelecChart);
        if(canvas->lineSelectChart)
        {
            canvas->lineSelectChart->addMagiPointEndLine(canvas->magPointIndex, canvas->curSelecChart);
            canvas->lineSelectChart->hideMagOnly();
            cl->setEndChart(canvas->lineSelectChart);
            cl->setEndMagIndex(canvas->magPointIndex);
            cl->setEndDirect(canvas->lineSelectChart->getMagiPointDirect(canvas->magPointIndex));
            cl->update();
            canvas->lineSelectChart = nullptr;
        }
        else{
            cl->resetEndChart();
        }
    }
    curPaintChart = nullptr;
}

void Drawer::doneLink()
{
    if(newLineToSelectChart)
    {
        newLineToSelectChart->addMagiPointEndLine(canvas->magPointToIndex, newLineChart);
        newLineToSelectChart->hideMagOnly();
        newLineChart->setEndChart(newLineToSelectChart);
        newLineChart->setEndMagIndex(canvas->magPointToIndex);
        newLineChart->setEndDirect(newLineToSelectChart->getMagiPointDirect(canvas->magPointToIndex));
        newLineChart->update();
    }else
        if(newLineChart)
            newLineChart->resetEndChart();
    newLineChart = nullptr;
    newLineFromSelectChart = nullptr;
    newLineToSelectChart = nullptr;
}
