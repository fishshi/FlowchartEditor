#ifndef FLOWCHART_H
#define FLOWCHART_H
#pragma execution_character_set("utf-8")
#include <map>
#include <stdexcept>

#include <QWidget>
#include <QPainter>
#include <vector>
#include <QMouseEvent>
#include <QPolygon>
#include <QStyleOption>

#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>

#include "../model/elements/flowchartelement.h"
#include "../model/elements/processelement.h"
#include "../model/elements/decisionelement.h"
#include "../model/elements/startendelement.h"
#include "../model/elements/connectorelement.h"
#include "../model/elements/line.h"
#include "../model/elements/dataelement.h"

#include "../model/config.h"

class Canvas : public QWidget
{
    Q_OBJECT
    friend class Controller;
private:
    std::vector<FlowchartElement*> charts;    // 画板上图形的集合
    std::vector<FlowchartElement*> line;      // 画板上线的集合
    void clearChartsLine(){                 // 释放图形、连线内存
        for(auto it = charts.begin();it!=charts.end();it++) {
            if(*it) delete *it;
            *it = nullptr;
        }
        for(auto it = line.begin();it!=line.end();it++) {
            if(*it) delete *it;
            *it = nullptr;
        }
    }

    MOUSE_EVENT_TYPE mouseEventType = MOUSE_EVENT_TYPE::NONE;   // 当前鼠标事件类型


    PaintChartType curPaintChartType = PaintChartType::NONE;    // 绘制的图形的类型

    QPoint curSelecChartPos;                // 选中时指针在图像对象上的位置信息

    Line *newLineChart = nullptr;             // 要创建的磁力点的连线的指针
    FlowchartElement *newLineToSelectChart = nullptr;     // 要连接到的磁力点的图形的指针
    FlowchartElement *newLineFromSelectChart = nullptr;   // 要创建的磁力点的图形的指针
    ORIENTION magPointDirect = ORIENTION::NONE;     // 选中时要创建的起始磁力点的方向类型
    int magPointFromIndex;                          // 要创建的磁力点的索引
    int magPointToIndex;                            // 要创建的磁力点的索引

    FlowchartElement *lineSelectChart = nullptr;  // 运行时线的吸附到的图形的指针
    int magPointIndex;                      // 运行时线吸附到的图形的磁力点的索引

    ORIENTION sizePointDirect = ORIENTION::NONE;    // 选中时要改变大小的大小点的方向类型

public:
    explicit Canvas(QWidget *parent = nullptr , Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~Canvas()
    {
        clearChartsLine();
    }

    FlowchartElement *curPaintChart = nullptr;    // 将要放置的图形
    FlowchartElement *curSelecChart = nullptr;    // 选中的画板上的图形
    void delChart(FlowchartElement *&cb);      // 删除图形
    void delLine(FlowchartElement *&cb);       // 添加图形
    void hideMagSizeAll();              // 隐藏所有元素

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *ev);

signals:
    void leftClicked();
    void mouseMoved();
    void delPressed();
    void escPressed();

public slots:
    void setPen(QPen &qp){curSelecChart->paintChartDrawPen = qp;}                                                       // 设置笔刷
    void setBrush(QBrush &qb){curSelecChart->paintChartFillPen = qb;}                                                   // 设置填充颜色

public:
    void setSelChartLineColor(const QColor &color);     // 设置图形线条颜色
    void setSelChartFillColor(const QColor &color);     // 设置图形填充颜色

public slots:
    void setPaintChart();                           // 设置将要绘制的图形
    void setSelecChart(FlowchartElement *, int ,int);     // 设置选中的画板的图像

    void setPaintProcessElement(){curPaintChartType = PaintChartType::RECT;setPaintChart();}                // 获取一个新的矩形
    void setPaintDecisionElement(){curPaintChartType = PaintChartType::DIAMOND;setPaintChart();}          // 获取一个新的菱形
    void setPaintStartEndElement(){curPaintChartType = PaintChartType::ROUNDRECT;setPaintChart();}      // 获取一个新的圆角矩形
    void setPaintConnectorElement(){curPaintChartType = PaintChartType::ELLIPSE;setPaintChart();}          // 获取一个新的圆形
    void setPaintLine(){curPaintChartType = PaintChartType::LINE;setPaintChart();}                // 获取一个新的线段
    void setPaintDataElement(){curPaintChartType = PaintChartType::PARALLELOGRAM;setPaintChart();}      // 获取一个新的梯形
    void resetPaintChartType(){curPaintChartType = PaintChartType::NONE;}                               // 清空类型

    void setTypeChangeSize(ORIENTION i){mouseEventType = MOUSE_EVENT_TYPE::RUNTIME_CHANGE_SIZE;sizePointDirect = i; }                                                                                                       // 设置鼠标事件类型为改变大小
    void setTypeCreateMagPoint(FlowchartElement *cb,ORIENTION d,int i) // 设置鼠标事件类型为创建线段
    {
        newLineFromSelectChart = cb;
        mouseEventType = MOUSE_EVENT_TYPE::RUNTIME_CREATE_MAGPOINT;
        magPointDirect = d;
        magPointFromIndex = i;
        newLineChart = nullptr;
    }
};
#endif // FLOWCHART_H
