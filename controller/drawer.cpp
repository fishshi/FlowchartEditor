#include "drawer.h"

Drawer::Drawer(Canvas *canvas)
{
    this->canvas = canvas;
}

void Drawer::setPaintChart()
{
    if (curPaintChart != nullptr)
        delete curPaintChart;
    switch (curPaintChartType)
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
    case PaintChartType::SUBPROCESSELEMENT:
        curPaintChart = new SubprocessElement(canvas);
        break;
    case PaintChartType::DOCUMENTELEMENT:
        curPaintChart = new DocumentElement(canvas);
        break;
    }
}

void Drawer::moveToCreate(int x, int y)
{
    curPaintChart->setXY(x, y);
    if (!curPaintChart->isVisible())
        curPaintChart->show();
    else
        curPaintChart->update();
}

int Drawer::moveToLink(int x, int y)
{
    int flag = 0;
    if (newLineChart == nullptr)
    {
        newLineChart = new Line(canvas);
        if (canvas->curSelecChart)
            canvas->curSelecChart->hideMagSize();
        canvas->curSelecChart = newLineChart;
        canvas->line.push_back(newLineChart);
        newLineChart->setXY(newLineFromSelectChart->getMagiPointAbsX(magPointFromIndex), newLineFromSelectChart->getMagiPointAbsY(magPointFromIndex));
        newLineChart->setStartChart(newLineFromSelectChart);
        newLineChart->setStartMagIndex(magPointFromIndex);
        newLineChart->setStartDirect(magPointDirect);
        newLineChart->update();
        newLineChart->show();
        newLineFromSelectChart->addMagiPointStartLine(magPointFromIndex, newLineChart);
        flag = 1;
    }

    for (auto it = canvas->charts.begin(); it != canvas->charts.end(); it++)
    {
        if ((*it)->autoSetMagi(x, y, magPointToIndex))
        {
            newLineToSelectChart = *it;
            break;
        }
        else
            newLineToSelectChart = nullptr;
    }
    newLineChart->setWidthHeight(x, y, DIRECTION::ENDPOINT);
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
    if (newLineToSelectChart)
    {
        newLineToSelectChart->addMagiPointEndLine(magPointToIndex, newLineChart);
        newLineToSelectChart->hideMagOnly();
        newLineChart->setEndChart(newLineToSelectChart);
        newLineChart->setEndMagIndex(magPointToIndex);
        newLineChart->setEndDirect(newLineToSelectChart->getMagiPointDirect(magPointToIndex));
        newLineChart->update();
    }
    else if (newLineChart)
        newLineChart->resetEndChart();
    newLineChart = nullptr;
    newLineFromSelectChart = nullptr;
    newLineToSelectChart = nullptr;
}

void Drawer::copy()
{
    if (canvas->curSelecChart == nullptr || canvas->curSelecChart->chartType == PaintChartType::LINE)
        return;
    switch (canvas->curSelecChart->chartType)
    {
    case PaintChartType::RECT:
        curPasteChart = new ProcessElement(canvas);
        break;
    case PaintChartType::DIAMOND:
        curPasteChart = new DecisionElement(canvas);
        break;
    case PaintChartType::ROUNDRECT:
        curPasteChart = new StartEndElement(canvas);
        break;
    case PaintChartType::ELLIPSE:
        curPasteChart = new ConnectorElement(canvas);
        break;
    case PaintChartType::PARALLELOGRAM:
        curPasteChart = new DataElement(canvas);
        break;
    case PaintChartType::SUBPROCESSELEMENT:
        curPasteChart = new SubprocessElement(canvas);
        break;
    case PaintChartType::DOCUMENTELEMENT:
        curPasteChart = new DocumentElement(canvas);
        break;
    default:
        break;
    }
    curPasteChart->widgetStart = canvas->curSelecChart->widgetStart;
    curPasteChart->widgetEnd = canvas->curSelecChart->widgetEnd;
    curPasteChart->paintStart = canvas->curSelecChart->paintStart;
    curPasteChart->paintEnd = canvas->curSelecChart->paintEnd;
    curPasteChart->paintChartDrawPen = canvas->curSelecChart->paintChartDrawPen;
    curPasteChart->paintChartFillPen = canvas->curSelecChart->paintChartFillPen;
    curPasteChart->chartText.text->setText(canvas->curSelecChart->chartText.text->text());
    curPasteChart->chartText.text->setGeometry(canvas->curSelecChart->chartText.text->geometry());
    curPasteChart->chartText.text->setFont(canvas->curSelecChart->chartText.text->font());
    curPasteChart->chartText.text->setStyleSheet(canvas->curSelecChart->chartText.text->styleSheet());
}

void Drawer::paste(int x, int y)
{
    if (curPasteChart == nullptr)
        return;
    curPasteChart->setXY(x, y);
    curPasteChart->show();
    canvas->hideMagSizeAll();
    canvas->charts.push_back(curPasteChart);
    canvas->curSelecChart = curPasteChart;
    copy();
}
