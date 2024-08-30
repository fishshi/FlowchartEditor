#include "controller.h"
#include "../ui_mainwindow.h"
#include <QMenu>
#include "../view/canvas.h"

Controller::Controller(MainWindow *w)
{
    this->w = w;
    drawer = new Drawer(w->ui->canvas);
    updater = new Updater(w->ui->canvas);
    remover = new Remover(w->ui->canvas);
    initConnections();
}

void Controller::initConnections()
{
    // 绑定图源选项
    connect(w->ui->processElementBtn, &QPushButton::clicked, [=](){
        drawer->setPaintProcessElement();
    });
    connect(w->ui->decisionElementBtn, &QPushButton::clicked, [=](){
        drawer->setPaintDecisionElement();
    });
    connect(w->ui->startEndElementBtn, &QPushButton::clicked, [=](){
        drawer->setPaintStartEndElement();
    });
    connect(w->ui->connectorElementBtn, &QPushButton::clicked, [=](){
        drawer->setPaintConnectorElement();
    });
    connect(w->ui->lineBtn, &QPushButton::clicked, [=](){
        drawer->setPaintLine();
    });
    connect(w->ui->dataElementBtn, &QPushButton::clicked, [=](){
        drawer->setPaintDataElement();
    });

    //右键菜单
    connect(w->ui->canvas, &QWidget::customContextMenuRequested, this, &Controller::showRrightClickMenu);

    //键盘按键
    connect(w->ui->canvas, &Canvas::escPressed, this, &Controller::on_escPressed);
    connect(w->ui->canvas, &Canvas::delPressed, this, &Controller::on_delPressed);
}

void Controller::showRrightClickMenu(const QPoint &pos)
{
    if(w->ui->canvas->curSelecChart == nullptr)
        return;
    QMenu menu;
    if(dynamic_cast<Line*>(w->ui->canvas->curSelecChart) == nullptr)
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
    menu.exec(w->ui->canvas->mapToGlobal(pos));
}

void Controller::on_escPressed()
{
    FlowchartElement *csc = w->ui->canvas->curSelecChart;
    if(csc)
    {
        csc->hideMagSize();
        csc = nullptr;
    }
}

void Controller::on_delPressed()
{
    FlowchartElement *csc = w->ui->canvas->curSelecChart;
    if(csc)
    {
        if(csc->chartType == PaintChartType::LINE)
            remover->delLine(csc);
        else
            remover->delChart(csc);
        w->ui->canvas->curSelecChart = nullptr;
    }
}
