#include "controller.h"

Controller::Controller(MainWindow *w)
{
    this->w = w;
    canvas = w->ui->canvas;
    drawer = new Drawer(canvas);
    updater = new Updater(canvas);
    remover = new Remover(canvas);
    initConnections();
}

void Controller::initConnections()
{
    // 绑定图源选项
    connect(w->ui->processElementBtn, &QPushButton::clicked, [=](){
        drawer->setPaintProcessElement();
        if(canvas->curPaintChart)
            mouseEventType = MOUSE_EVENT_TYPE::CREATING;
    });
    connect(w->ui->decisionElementBtn, &QPushButton::clicked, [=](){
        drawer->setPaintDecisionElement();
        if(canvas->curPaintChart)
            mouseEventType = MOUSE_EVENT_TYPE::CREATING;
    });
    connect(w->ui->startEndElementBtn, &QPushButton::clicked, [=](){
        drawer->setPaintStartEndElement();
        if(canvas->curPaintChart)
            mouseEventType = MOUSE_EVENT_TYPE::CREATING;
    });
    connect(w->ui->connectorElementBtn, &QPushButton::clicked, [=](){
        drawer->setPaintConnectorElement();
        if(canvas->curPaintChart)
            mouseEventType = MOUSE_EVENT_TYPE::CREATING;
    });
    connect(w->ui->lineBtn, &QPushButton::clicked, [=](){
        drawer->setPaintLine();
        if(canvas->curPaintChart)
            mouseEventType = MOUSE_EVENT_TYPE::CREATING;
    });
    connect(w->ui->dataElementBtn, &QPushButton::clicked, [=](){
        drawer->setPaintDataElement();
        if(canvas->curPaintChart)
            mouseEventType = MOUSE_EVENT_TYPE::CREATING;
    });

    //鼠标事件
    connect(canvas, &Canvas::leftPressed, this, &Controller::on_leftPressed);
    connect(canvas, &QWidget::customContextMenuRequested, this, &Controller::showRrightClickMenu);
    connect(canvas, &Canvas::mouseMoved, this, &Controller::on_mouseMoved);
    connect(canvas, &Canvas::mouseReleased, this, &Controller::on_mouseReleased);

    //键盘按键
    connect(canvas, &Canvas::escPressed, this, &Controller::on_escPressed);
    connect(canvas, &Canvas::delPressed, this, &Controller::on_delPressed);
}

void Controller::showRrightClickMenu(const QPoint &pos)
{
    if(canvas->curSelecChart == nullptr)
        return;
    QMenu menu;
    if(dynamic_cast<Line*>(canvas->curSelecChart) == nullptr)
    {
        QAction *changeFillColor = menu.addAction("更换填充颜色");
        connect(changeFillColor, &QAction::triggered, [=](){
            QColor color = QColorDialog::getColor(Qt::white, w,tr("设置填充颜色"));
            updater->setSelChartFillColor(color);
        });
    }
    QAction *changeLineColor = menu.addAction("更换线条颜色");
    connect(changeLineColor, &QAction::triggered, [=](){
        QColor color = QColorDialog::getColor(Qt::white, w,tr("设置线条颜色"));
        updater->setSelChartLineColor(color);
    });
    menu.exec(canvas->mapToGlobal(pos));
}

void Controller::on_escPressed()
{
    FlowchartElement *csc = canvas->curSelecChart;
    if(csc)
    {
        csc->hideMagSize();
        csc = nullptr;
    }
}

void Controller::on_delPressed()
{
    FlowchartElement *csc = canvas->curSelecChart;
    if(csc)
    {
        if(csc->chartType == PaintChartType::LINE)
            remover->delLine(csc);
        else
            remover->delChart(csc);
        canvas->curSelecChart = nullptr;
    }
}

void Controller::on_leftPressed(QMouseEvent *event)
{
    if(mouseEventType == MOUSE_EVENT_TYPE::CREATING)
        on_leftClickToCreate(event->pos().rx(), event->pos().ry());
    else
    {
        for(auto x : canvas->charts)
        {
            x->overlapChartMousePressed(event);
            if(event->isAccepted()) return;
        }
        for(auto x : canvas->line)
        {
            x->overlapChartMousePressed(event);
            if(event->isAccepted()) return;
        }
        canvas->hideMagSizeAll();
        mouseEventType = MOUSE_EVENT_TYPE::NONE;
    }
}

void Controller::on_leftClickToCreate(int x, int y)
{
    drawer->clickToCreate(x, y);
    connect(canvas->curPaintChart,&FlowchartElement::sendThisClass, this, &Controller::on_leftClickToSelect);
    connect(canvas->curPaintChart,SIGNAL(setTypeChangeSize(ORIENTION)), this, SLOT(setTypeChangeSize(ORIENTION)));
    if(canvas->curPaintChart->chartType != PaintChartType::LINE)
        connect(canvas->curPaintChart,SIGNAL(setTypeCreateMagPoint(FlowchartElement *,ORIENTION,int)),this,SLOT(setTypeCreateMagPoint(FlowchartElement *,ORIENTION,int)));
}

void Controller::on_leftClickToSelect(FlowchartElement * cb, int x, int y)
{
    updater->setSelecChart(cb, x, y);
    mouseEventType = MOUSE_EVENT_TYPE::RUNTIME_CHANGE_POS;
}

void Controller::on_mouseMoved(QMouseEvent *event)
{
    if (mouseEventType == MOUSE_EVENT_TYPE::CREATING)
        on_moveToCreate(event->pos().rx(), event->pos().ry());
    else if (mouseEventType == MOUSE_EVENT_TYPE::CHANGE_SIZE)
        on_moveToChangeSize(event->pos().rx(), event->pos().ry());
    else if (mouseEventType == MOUSE_EVENT_TYPE::RUNTIME_CREATE_MAGPOINT)
        on_moveToLink(event->pos().rx(), event->pos().ry());
    else if (mouseEventType == MOUSE_EVENT_TYPE::RUNTIME_CHANGE_POS)
        on_moveToChangePos(event->pos().rx(), event->pos().ry());
    else
    {
        for(auto it = canvas->charts.begin();it!= canvas->charts.end();it++)
        {
            (*it)->overlapChartMouseMove(event);
            if(event->isAccepted()) return;
        }
        for(auto it = canvas->line.begin();it!=canvas->line.end();it++)
        {
            (*it)->overlapChartMouseMove(event);
            if(event->isAccepted()) return;
        }
        if(canvas->curSelecChart == nullptr)
        {
            canvas->setCursor(QCursor(Qt::ArrowCursor));
            canvas->hideMagSizeAll();
            mouseEventType = MOUSE_EVENT_TYPE::NONE;
        }
    }
}
void Controller::on_moveToCreate(int x, int y)
{
    drawer->moveToCreate(x, y);
}

void Controller::on_moveToChangeSize(int x, int y)
{
    updater->moveToChangeSize(x, y);
}

void Controller::on_moveToChangePos(int x, int y)
{
    updater->moveToChangePos(x, y);
}

void Controller::on_moveToLink(int x, int y)
{
    if(drawer->moveToLink(x, y) == 1)
    {
        connect(canvas->newLineChart,&FlowchartElement::sendThisClass, this, &Controller::on_leftClickToSelect);
        connect(canvas->newLineChart,SIGNAL(setTypeChangeSize(ORIENTION)), this,SLOT(setTypeChangeSize(ORIENTION)));
    }
}

void Controller::on_mouseReleased(QMouseEvent *event)
{
    if(mouseEventType == MOUSE_EVENT_TYPE::CREATING)
        on_doneCreate();
    else if(mouseEventType == MOUSE_EVENT_TYPE::CHANGE_SIZE)
        on_doneChangeSize();
    else if(mouseEventType == MOUSE_EVENT_TYPE::RUNTIME_CREATE_MAGPOINT)
        on_doneLink();
    else if(mouseEventType == MOUSE_EVENT_TYPE::NONE)
        event->ignore();
    mouseEventType = MOUSE_EVENT_TYPE::NONE;
}

void Controller::on_doneCreate()
{
    canvas->curPaintChartType = PaintChartType::NONE;
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
    canvas->curPaintChart = nullptr;
}

void Controller::on_doneChangeSize()
{
    if(canvas->curSelecChart->chartType == PaintChartType::LINE)
    {
        Line *cl = reinterpret_cast<Line *>(canvas->curSelecChart);
        if(canvas->lineSelectChart)
        {
            if(canvas->sizePointDirect == ORIENTION::STARTPOINT)
            {
                cl->resetStartChart();
                canvas->lineSelectChart->addMagiPointStartLine(canvas->magPointIndex, canvas->curSelecChart);
                canvas->lineSelectChart->hideMagOnly();
                cl->setStartChart(canvas->lineSelectChart);
                cl->setStartMagIndex(canvas->magPointIndex);
                cl->setStartDirect(canvas->lineSelectChart->getMagiPointDirect(canvas->magPointIndex));
            }else if(canvas->sizePointDirect == ORIENTION::ENDPOINT)
            {
                cl->resetEndChart();
                canvas->lineSelectChart->addMagiPointEndLine(canvas->magPointIndex, canvas->curSelecChart);
                canvas->lineSelectChart->hideMagOnly();
                cl->setEndChart(canvas->lineSelectChart);
                cl->setEndMagIndex(canvas->magPointIndex);
                cl->setEndDirect(canvas->lineSelectChart->getMagiPointDirect(canvas->magPointIndex));
            }
            cl->update();
        }else
        {
            if(canvas->sizePointDirect == ORIENTION::STARTPOINT)
            {
                cl->resetStartChart();
            }else if(canvas->sizePointDirect == ORIENTION::ENDPOINT)
            {
                cl->resetEndChart();
            }
        }
    }
    canvas->lineSelectChart = nullptr;
}

void Controller::on_doneLink()
{
    if(canvas->newLineToSelectChart)
    {
        canvas->newLineToSelectChart->addMagiPointEndLine(canvas->magPointToIndex, canvas->newLineChart);
        canvas->newLineToSelectChart->hideMagOnly();
        canvas->newLineChart->setEndChart(canvas->newLineToSelectChart);
        canvas->newLineChart->setEndMagIndex(canvas->magPointToIndex);
        canvas->newLineChart->setEndDirect(canvas->newLineToSelectChart->getMagiPointDirect(canvas->magPointToIndex));
        canvas->newLineChart->update();
    }else
        if(canvas->newLineChart)
            canvas->newLineChart->resetEndChart();
    canvas->newLineChart = nullptr;
    canvas->newLineFromSelectChart = nullptr;
    canvas->newLineToSelectChart = nullptr;
}
