#include "canvas.h"

Canvas::Canvas(QWidget *parent, Qt::WindowFlags f) : QWidget(parent,f)
{
    this->grabKeyboard();
    setContextMenuPolicy(Qt::CustomContextMenu);
    setMouseTracking(true);
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter p = QPainter(this);
    p.setPen(QColor(150,150,150));
    const int gap = 30;
    for(int i = 0;i * gap < width(); ++i)
        for(int j = 0; j * gap < height(); ++j)
            p.drawPoint(i * gap, j * gap);
}

void Canvas::setSelChartLineColor(const QColor &color)
{
    curSelecChart->paintChartDrawPen.setColor(color);
    curSelecChart->update();
}

void Canvas::setSelChartFillColor(const QColor &color)
{
    curSelecChart->paintChartFillPen.setColor(color);
    curSelecChart->update();
}

void Canvas::setPaintChart()
{
    if(curPaintChart != nullptr)
        delete curPaintChart;
    switch(curPaintChartType)
    {
        case PaintChartType::RECT:
            curPaintChart = new ProcessElement(this);
            break;
        case PaintChartType::DIAMOND:
            curPaintChart = new DecisionElement(this);
            break;
        case PaintChartType::ROUNDRECT:
            curPaintChart = new StartEndElement(this);
            break;
        case PaintChartType::ELLIPSE:
            curPaintChart = new ConnectorElement(this);
            break;
        case PaintChartType::LINE:
            curPaintChart = new Line(this);
            break;
        case PaintChartType::PARALLELOGRAM:
            curPaintChart = new DataElement(this);
            break;
        case PaintChartType::NONE:
            curPaintChart = nullptr;
            break;
    }
    if(curPaintChart)
        mouseEventType = MOUSE_EVENT_TYPE::CREATING_CNANGE_SIZE;
}

void Canvas::setSelecChart(FlowchartElement * cb, int x, int y)
{
    this->grabKeyboard();
    if(curSelecChart != nullptr) curSelecChart->hideMagSize();
    curSelecChart = cb;
    curSelecChart->showMagSize();
    curSelecChartPos = QPoint(x,y);
    mouseEventType = MOUSE_EVENT_TYPE::RUNTIME_CHANGE_POS;
}

void Canvas::delChart(FlowchartElement *&cb)
{
    for(int i = 0; i < charts.size(); ++i)
    {
        if(charts[i] == cb)
        {
            FlowchartElement *tmp = cb;
            charts.erase(charts.begin() + i);
            --i;
            for(auto magit = tmp->magPoint.i_point.begin();magit!=tmp->magPoint.i_point.end();magit++)
            {
                for(auto magLineStIt = (*magit)->i_lineStart.begin();magLineStIt != (*magit)->i_lineStart.end();magLineStIt++)
                {
                    dynamic_cast<Line*>(*magLineStIt) ->resetEndChart();
                    delLine(*magLineStIt);
                }
                for(auto magLineEnIt = (*magit)->i_lineEnd.begin();magLineEnIt != (*magit)->i_lineEnd.end();magLineEnIt++)
                {
                    dynamic_cast<Line*>(*magLineEnIt) ->resetStartChart();
                    delLine(*magLineEnIt);
                }
            }
            delete tmp;
            break;
        }
    }
}

void Canvas::delLine(FlowchartElement *&cb)
{
    for(auto it = line.begin();it != line.end();it++)
    {
        if(*it == cb)
        {
            Line *tmp = dynamic_cast<Line*>(*it);
            cb = nullptr;
            line.erase(it);
            tmp->resetEndChart();
            tmp->resetStartChart();
            delete tmp;
            break;
        }
    }
}

void Canvas::hideMagSizeAll()
{
    for(auto it = charts.begin();it!=charts.end();it++)
        (*it)->hideMagSize();
    for(auto it = line.begin();it!=line.end();it++)
        (*it)->hideMagSize();
    curSelecChart = nullptr;
    mouseEventType = MOUSE_EVENT_TYPE::NONE;
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    switch(mouseEventType)
    {
        case MOUSE_EVENT_TYPE::CREATING_CNANGE_SIZE:{
            int x = event->pos().rx();
            int y = event->pos().ry();
            if(curSelecChart) curSelecChart->hideMagSize();
            curSelecChart = curPaintChart;
            curPaintChart->setXY(x,y);
            if(curPaintChart->chartType == PaintChartType::LINE)
            {
                connect(curPaintChart,SIGNAL(sendThisClass(FlowchartElement *, int,int)),this,SLOT(setSelecChart(FlowchartElement *, int,int)));
                connect(curPaintChart,SIGNAL(setTypeChangeSize(ORIENTION)),this,SLOT(setTypeChangeSize(ORIENTION)));
                line.push_back(curPaintChart);
            }else
            {
                connect(curPaintChart,SIGNAL(sendThisClass(FlowchartElement *, int,int)),this,SLOT(setSelecChart(FlowchartElement *, int,int)));
                connect(curPaintChart,SIGNAL(setTypeChangeSize(ORIENTION)),this,SLOT(setTypeChangeSize(ORIENTION)));
                connect(curPaintChart,SIGNAL(setTypeCreateMagPoint(FlowchartElement *,ORIENTION,int)),this,SLOT(setTypeCreateMagPoint(FlowchartElement *,ORIENTION,int)));
                charts.push_back(curPaintChart);
            }
            curPaintChart->update();
            curPaintChart->show();
            event->accept();
        }break;
        default:
        case MOUSE_EVENT_TYPE::NONE:{
            for(auto it = charts.begin();it!=charts.end();it++)
            {
                (*it)->overlapChartMousePressed(event);
                if(event->isAccepted()) return;
            }
            for(auto a:line)
            {
                a->overlapChartMousePressed(event);
                if(event->isAccepted()) return;
            }
            hideMagSizeAll();
        }break;
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
    switch(mouseEventType)
    {
        default:case MOUSE_EVENT_TYPE::NONE:
        {
            for(auto it = charts.begin();it!=charts.end();it++)
            {
                (*it)->overlapChartMouseMove(event);
                if(event->isAccepted()) return;
            }
            for(auto it = line.begin();it!=line.end();it++)
            {
                (*it)->overlapChartMouseMove(event);
                if(event->isAccepted()) return;
            }
            if(curSelecChart == nullptr)
            {
                setCursor(QCursor(Qt::ArrowCursor));
                hideMagSizeAll();
            }
        }break;
        case MOUSE_EVENT_TYPE::CREATING_CNANGE_SIZE:
        {
            int x = event->pos().rx(),y = event->pos().ry();
            if(curSelecChart)
            {
                if(curSelecChart->chartType == PaintChartType::LINE)
                    for(auto it = charts.begin();it!=charts.end();it++)
                    {
                        if((*it)->autoSetMagi(x,y,magPointIndex))
                        {
                            lineSelectChart = *it;
                            break;
                        }
                        else
                            lineSelectChart = nullptr;
                    }
                curPaintChart->setWidthHeight(x,y);
            }
            event->accept();
        }break;
        case MOUSE_EVENT_TYPE::RUNTIME_CHANGE_SIZE:
        {
            int x = event->pos().rx(),y = event->pos().ry();
            if(curSelecChart->chartType == PaintChartType::LINE)
                for(auto it = charts.begin();it!=charts.end();it++)
                {
                    if((*it)->autoSetMagi(x,y,magPointIndex))
                    {
                        lineSelectChart = *it;
                        break;
                    }
                    else
                        lineSelectChart = nullptr;
                }
            curSelecChart->setWidthHeight(x,y,sizePointDirect);
            event->accept();
        }break;
        case MOUSE_EVENT_TYPE::RUNTIME_CREATE_MAGPOINT:
        {
            if(newLineChart == nullptr)
            {
                newLineChart = new Line(this);
                if(curSelecChart)
                    curSelecChart->hideMagSize();
                curSelecChart = newLineChart;
                line.push_back(newLineChart);
                connect(newLineChart,SIGNAL(sendThisClass(FlowchartElement *, int,int)),this,SLOT(setSelecChart(FlowchartElement *, int,int)));
                connect(newLineChart,SIGNAL(setTypeChangeSize(ORIENTION)),this,SLOT(setTypeChangeSize(ORIENTION)));
                newLineChart->setXY(newLineFromSelectChart->getMagiPointAbsX(magPointFromIndex),newLineFromSelectChart->getMagiPointAbsY(magPointFromIndex));
                newLineChart->setStartChart(newLineFromSelectChart);
                newLineChart->setStartMagIndex(magPointFromIndex);
                newLineChart->setStartDirect(magPointDirect);
                newLineChart->update();
                newLineChart->show();
                newLineFromSelectChart->addMagiPointStartLine(magPointFromIndex,newLineChart);
            }
            int x = event->pos().rx(),y = event->pos().ry();

            for(auto it = charts.begin();it!=charts.end();it++)
            {
                if((*it)->autoSetMagi(x,y,magPointToIndex))
                {
                    newLineToSelectChart = *it;
                    break;
                }else{
                    newLineToSelectChart = nullptr;
                }
            }
            newLineChart->setWidthHeight(x,y,ORIENTION::ENDPOINT);
            event->ignore();
        }break;
        case MOUSE_EVENT_TYPE::RUNTIME_CHANGE_POS:
        {
            if(curSelecChart->chartType != PaintChartType::LINE)
            {
                curSelecChart->setXY(event->pos().rx() - curSelecChartPos.rx(),event->pos().ry() - curSelecChartPos.ry());
                event->accept();
            }
        }break;
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    switch(mouseEventType)
    {
    case MOUSE_EVENT_TYPE::CREATING_CNANGE_SIZE:
        resetPaintChartType();
        if(curSelecChart->chartType == PaintChartType::LINE)
        {
            Line *cl = reinterpret_cast<Line *>(curSelecChart);
            if(lineSelectChart)
            {
                lineSelectChart->addMagiPointEndLine(magPointIndex,curSelecChart);
                lineSelectChart->hideMagOnly();
                cl->setEndChart(lineSelectChart);
                cl->setEndMagIndex(magPointIndex);
                cl->setEndDirect(lineSelectChart->getMagiPointDirect(magPointIndex));
                cl->update();
                lineSelectChart = nullptr;
            }
            else{
                cl->resetEndChart();
            }
        }
        curPaintChart = nullptr;
        mouseEventType = MOUSE_EVENT_TYPE::NONE;
        event->accept();
        break;
    case MOUSE_EVENT_TYPE::RUNTIME_CHANGE_SIZE:
        if(curSelecChart->chartType == PaintChartType::LINE)
        {
            Line *cl = reinterpret_cast<Line *>(curSelecChart);
            if(lineSelectChart)
            {
                if(sizePointDirect == ORIENTION::STARTPOINT)
                {
                    cl->resetStartChart();
                    lineSelectChart->addMagiPointStartLine(magPointIndex,curSelecChart);
                    lineSelectChart->hideMagOnly();
                    cl->setStartChart(lineSelectChart);
                    cl->setStartMagIndex(magPointIndex);
                    cl->setStartDirect(lineSelectChart->getMagiPointDirect(magPointIndex));
                }else if(sizePointDirect == ORIENTION::ENDPOINT)
                {
                    cl->resetEndChart();
                    lineSelectChart->addMagiPointEndLine(magPointIndex,curSelecChart);
                    lineSelectChart->hideMagOnly();
                    cl->setEndChart(lineSelectChart);
                    cl->setEndMagIndex(magPointIndex);
                    cl->setEndDirect(lineSelectChart->getMagiPointDirect(magPointIndex));
                }
                cl->update();
            }else
            {
                if(sizePointDirect == ORIENTION::STARTPOINT)
                {
                    cl->resetStartChart();
                }else if(sizePointDirect == ORIENTION::ENDPOINT)
                {
                    cl->resetEndChart();
                }
            }
        }
        mouseEventType = MOUSE_EVENT_TYPE::NONE;
        lineSelectChart = nullptr;
        event->accept();
        break;
    case MOUSE_EVENT_TYPE::RUNTIME_CREATE_MAGPOINT:
        if(newLineToSelectChart)
        {
            newLineToSelectChart->addMagiPointEndLine(magPointToIndex,newLineChart);
            newLineToSelectChart->hideMagOnly();
            newLineChart->setEndChart(newLineToSelectChart);
            newLineChart->setEndMagIndex(magPointToIndex);
            newLineChart->setEndDirect(newLineToSelectChart->getMagiPointDirect(magPointToIndex));
            newLineChart->update();
        }else
            if(newLineChart)
                newLineChart->resetEndChart();
        mouseEventType = MOUSE_EVENT_TYPE::NONE;
        newLineChart = nullptr;
        newLineFromSelectChart = nullptr;
        newLineToSelectChart = nullptr;
        event->accept();
        break;
    case MOUSE_EVENT_TYPE::RUNTIME_CHANGE_POS:
        mouseEventType = MOUSE_EVENT_TYPE::NONE;
        event->accept();
        break;
    case MOUSE_EVENT_TYPE::NONE:
        event->ignore();
        break;
    }
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    event->ignore();
    if(event->key() == Qt::Key_Escape)
        emit escPressed();
    else if (event->key() == Qt::Key_Delete)
        emit delPressed();
}
