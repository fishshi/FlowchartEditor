#include "controller.h"
#include "../ui_mainwindow.h"
#include <QMenu>
#include "../view/canvas.h"

Controller::Controller(MainWindow *w)
{
    this->w = w;
    drawer = new Drawer(w->ui->canvas);
    initConnections();
}

void Controller::initConnections()
{
    //创建
    connect(w->ui->processElementBtn, &QPushButton::clicked, [=](){
        w->ui->canvas->setPaintProcessElement();
        if(drawer->curPaintChart)
            mouseEventType = mouseEventType = MOUSE_EVENT_TYPE::CREATING_CNANGE_SIZE;
    });
    connect(w->ui->decisionElementBtn, &QPushButton::clicked, [=](){
        w->ui->canvas->setPaintDecisionElement();
        if(drawer->curPaintChart)
            mouseEventType = mouseEventType = MOUSE_EVENT_TYPE::CREATING_CNANGE_SIZE;
    });
    connect(w->ui->startEndElementBtn, &QPushButton::clicked, [=](){
        w->ui->canvas->setPaintStartEndElement();
        if(drawer->curPaintChart)
            mouseEventType = mouseEventType = MOUSE_EVENT_TYPE::CREATING_CNANGE_SIZE;
    });
    connect(w->ui->connectorElementBtn, &QPushButton::clicked, [=](){
        w->ui->canvas->setPaintConnectorElement();
        if(drawer->curPaintChart)
            mouseEventType = mouseEventType = MOUSE_EVENT_TYPE::CREATING_CNANGE_SIZE;
    });
    connect(w->ui->lineBtn, &QPushButton::clicked, [=](){
        w->ui->canvas->setPaintLine();
        if(drawer->curPaintChart)
            mouseEventType = mouseEventType = MOUSE_EVENT_TYPE::CREATING_CNANGE_SIZE;
    });
    connect(w->ui->dataElementBtn, &QPushButton::clicked, [=](){
        w->ui->canvas->setPaintDataElement();
        if(drawer->curPaintChart)
            mouseEventType = mouseEventType = MOUSE_EVENT_TYPE::CREATING_CNANGE_SIZE;
    });

    //按键
    connect(w->ui->canvas, &QWidget::customContextMenuRequested, this, &Controller::showRrightClickMenu);
}



void Controller::sendSelChartLineColor()
{
    QColor color = QColorDialog::getColor(Qt::white, w,tr("设置线条颜色"));
    w->ui->canvas->setSelChartLineColor(color);
}

void Controller::sendSelChartFillColor()
{
    QColor color = QColorDialog::getColor(Qt::white, w,tr("设置填充颜色"));
    w->ui->canvas->setSelChartFillColor(color);
}

void Controller::showRrightClickMenu(const QPoint &pos)
{
    if(w->ui->canvas->curSelecChart == nullptr)
        return;
    QMenu menu;
    if(dynamic_cast<Line*>(w->ui->canvas->curSelecChart) == nullptr)
    {
        QAction *changeFillColor = menu.addAction("更换填充颜色");
        connect(changeFillColor, &QAction::triggered, this, &Controller::sendSelChartFillColor);
    }
    QAction *changeLineColor = menu.addAction("更换线条颜色");
    connect(changeLineColor, &QAction::triggered, this, &Controller::sendSelChartLineColor);
    menu.exec(w->ui->canvas->mapToGlobal(pos));
}
