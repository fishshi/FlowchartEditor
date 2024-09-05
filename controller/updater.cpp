#include "updater.h"

Updater::Updater(Canvas *canvas)
{
    this->canvas = canvas;
}

void Updater::setSelecChart(FlowchartElement *cb, int x, int y)
{
    if (canvas->curSelecChart != nullptr)
        canvas->curSelecChart->hideMagSize();
    canvas->curSelecChart = cb;
    canvas->curSelecChart->showMagSize();
    curSelecChartPos = QPoint(x, y);
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
    if (canvas->curSelecChart->chartType == PaintChartType::LINE)
        for (auto it = canvas->charts.begin(); it != canvas->charts.end(); it++)
        {
            if ((*it)->autoSetMagi(x, y, canvas->magPointIndex))
            {
                canvas->lineSelectChart = *it;
                break;
            }
            else
                canvas->lineSelectChart = nullptr;
        }
    canvas->curSelecChart->setWidthHeight(x, y, sizePointDirect);
}

void Updater::moveToChangePos(int x, int y)
{
    if (canvas->curSelecChart->chartType != PaintChartType::LINE)
        canvas->curSelecChart->setXY(x - curSelecChartPos.rx(), y - curSelecChartPos.ry());
}

void Updater::doneChangeSize()
{
    if (canvas->curSelecChart->chartType == PaintChartType::LINE)
    {
        Line *cl = reinterpret_cast<Line *>(canvas->curSelecChart);
        if (canvas->lineSelectChart)
        {
            if (sizePointDirect == DIRECTION::STARTPOINT)
            {
                cl->resetStartChart();
                canvas->lineSelectChart->addMagiPointStartLine(canvas->magPointIndex, canvas->curSelecChart);
                canvas->lineSelectChart->hideMagOnly();
                cl->setStartChart(canvas->lineSelectChart);
                cl->setStartMagIndex(canvas->magPointIndex);
                cl->setStartDirect(canvas->lineSelectChart->getMagiPointDirect(canvas->magPointIndex));
            }
            else if (sizePointDirect == DIRECTION::ENDPOINT)
            {
                cl->resetEndChart();
                canvas->lineSelectChart->addMagiPointEndLine(canvas->magPointIndex, canvas->curSelecChart);
                canvas->lineSelectChart->hideMagOnly();
                cl->setEndChart(canvas->lineSelectChart);
                cl->setEndMagIndex(canvas->magPointIndex);
                cl->setEndDirect(canvas->lineSelectChart->getMagiPointDirect(canvas->magPointIndex));
            }
            cl->update();
        }
        else
        {
            if (sizePointDirect == DIRECTION::STARTPOINT)
                cl->resetStartChart();
            else if (sizePointDirect == DIRECTION::ENDPOINT)
                cl->resetEndChart();
        }
    }
    canvas->lineSelectChart = nullptr;
}

void Updater::frameSelect(int x, int y)
{
    canvas->setDrawFrame(frameX, frameY, x, y);
    for (auto fce : canvas->charts)
    {
        if (frameSelCharts.find(fce) == frameSelCharts.end() && fce->x() > frameX && fce->x() < x && fce->y() > frameY && fce->y() < y)
        {
            fce->showMagSize();
            frameSelCharts.insert(fce);
        }
    }
}

void Updater::doneFrameSelect()
{
    if(frameSelCharts.empty())
    {
        clearFrameSelect();
        return;
    }
    isFrameSelected = true;
}

void Updater::clearFrameSelect()
{
    frameSelCharts.clear();
    canvas->setDrawFrame(0, 0, 0, 0);
    isFrameSelected = false;
    canvas->unsetDrawFrame();
    canvas->hideMagSizeAll();
}

void Updater::moveToChangeFramePos(int x, int y)
{
    int moveX = x - curSelecFramePos.rx();
    int moveY = y - curSelecFramePos.ry();
    for (auto fce : frameSelCharts)
        fce->setXY(fce->widgetStart.rx() + moveX, fce->widgetStart.ry() + moveY);
    canvas->moveFrameBy(moveX, moveY);
    curSelecFramePos = QPoint(x, y);
}

void Updater::search(QString target)
{
    for (auto x : canvas->charts)
    {
        if (x->chartText.text->text().contains(target))
            x->showMagSize();
    }
}

void Updater::replace(QString target, QString newTarget)
{
    for (auto x : canvas->charts)
    {
        if (x->chartText.text->text().contains(target))
        {
            QString tmp = x->chartText.text->text();
            QStringList tmpSplit = tmp.split(target);
            tmp = tmpSplit[0];
            for (int i = 1; i < tmpSplit.size(); ++i)
                tmp += (newTarget + tmpSplit[i]);
            x->chartText.text->setText(tmp);
            x->updateTextInfo();
            x->showMagSize();
        }
    }
}
