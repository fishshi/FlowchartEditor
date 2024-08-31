#ifndef FLOWCHART_H
#define FLOWCHART_H
#pragma execution_character_set("utf-8")
#include <QWidget>
#include <QPainter>
#include <vector>
#include <QMouseEvent>
#include <QPolygon>
#include <QStyleOption>

#include <QPushButton>
#include <QFileDialog>
#include <QColorDialog>

#include "../model/elements/flowchartelement.h"
#include "../model/elements/line.h"
#include "../model/elements/processelement.h"
#include "../model/elements/decisionelement.h"
#include "../model/elements/startendelement.h"
#include "../model/elements/connectorelement.h"
#include "../model/elements/dataelement.h"

#include "../model/config.h"

class Canvas : public QWidget
{
    Q_OBJECT
    friend class Controller;
    friend class Drawer;
    friend class Updater;
    friend class Remover;
private:
    std::vector<FlowchartElement*> charts;    // 画板上图形的集合
    std::vector<FlowchartElement*> line;      // 画板上线的集合

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

    FlowchartElement *curPaintChart = nullptr;    // 将要放置的图形
    FlowchartElement *curSelecChart = nullptr;    // 选中的画板上的图形

    void clearChartsLine();                      // 释放图形、连线内存

public:
    explicit Canvas(QWidget *parent = nullptr , Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~Canvas()
    {
        clearChartsLine();
    }
    void hideMagSizeAll();              // 隐藏所有大小、磁力点

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

signals:
    void leftPressed(QMouseEvent *event);
    void mouseMoved(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);
    void delPressed();
    void escPressed();
};
#endif // FLOWCHART_H
