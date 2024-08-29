#include "controller.h"
#include "../ui_mainwindow.h"
#include <QMenu>
#include "../view/canvas.h"

Controller::Controller(MainWindow *w)
{
    this->w = w;
    initConnections();
}

Controller::~Controller()
{
}

void Controller::initConnections()
{
    //创建
    connect(w->ui->processElementBtn,SIGNAL(clicked()),w->ui->canvas,SLOT(setPaintProcessElement()));
    connect(w->ui->decisionElementBtn,SIGNAL(clicked()),w->ui->canvas,SLOT(setPaintDecisionElement()));
    connect(w->ui->startEndElementBtn,SIGNAL(clicked()),w->ui->canvas,SLOT(setPaintStartEndElement()));
    connect(w->ui->connectorElementBtn,SIGNAL(clicked()),w->ui->canvas,SLOT(setPaintConnectorElement()));
    connect(w->ui->lineBtn,SIGNAL(clicked()),w->ui->canvas,SLOT(setPaintLine()));
    connect(w->ui->dataElementBtn,SIGNAL(clicked()),w->ui->canvas,SLOT(setPaintDataElement()));

    //按键
    connect(w->ui->canvas, &QWidget::customContextMenuRequested, this, &Controller::showRrightClickMenu);

    //更新
    connect(w->ui->canvas,SIGNAL(sendChartStyle(QPen &, QBrush &)),this,SLOT(setFillStyle(QPen &, QBrush &)));
    connect(w->ui->canvas,SIGNAL(sendLineStyle(QPen &, LINE_HEAD_TYPE &, LINE_HEAD_TYPE &)),this,SLOT(setLineStyle(QPen &, LINE_HEAD_TYPE &, LINE_HEAD_TYPE &)));
    connect(w->ui->canvas,SIGNAL(disableStyle()),this,SLOT(disableStylePanel()));

    //文件
    connect(w->ui->actionOpenFile,SIGNAL(triggered(bool)),w->ui->canvas,SLOT(openChartFile()));
    connect(w->ui->actionSaveFile,SIGNAL(triggered(bool)),w->ui->canvas,SLOT(saveChartFile()));
    connect(w->ui->actionNewFile,SIGNAL(triggered(bool)),w->ui->canvas,SLOT(newChartFile()));
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

    QAction *changeFillColor = menu.addAction("更换填充颜色");
    QAction *changeLineColor = menu.addAction("更换线条颜色");

    connect(changeFillColor, SIGNAL(triggered()), this, SLOT(sendSelChartFillColor()));
    connect(changeLineColor, SIGNAL(triggered()), this, SLOT(sendSelChartLineColor()));

    // 在光标位置显示菜单
    menu.exec(w->ui->canvas->mapToGlobal(pos));
}
