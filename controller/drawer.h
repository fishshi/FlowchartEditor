#ifndef DRAWER_H
#define DRAWER_H

#include "../view/canvas.h"

class Drawer
{
    friend class Controller;

public:
    Drawer(Canvas *canvas);

    void setPaintProcessElement()
    {
        curPaintChartType = PaintChartType::RECT;
        setPaintChart();
    }
    void setPaintDecisionElement()
    {
        curPaintChartType = PaintChartType::DIAMOND;
        setPaintChart();
    }
    void setPaintStartEndElement()
    {
        curPaintChartType = PaintChartType::ROUNDRECT;
        setPaintChart();
    }
    void setPaintConnectorElement()
    {
        curPaintChartType = PaintChartType::ELLIPSE;
        setPaintChart();
    }
    void setPaintDataElement()
    {
        curPaintChartType = PaintChartType::PARALLELOGRAM;
        setPaintChart();
    }
    void setPaintSubprocessElement()
    {
        curPaintChartType = PaintChartType::SUBPROCESSELEMENT;
        setPaintChart();
    }
    void setPaintDocumentElement()
    {
        curPaintChartType = PaintChartType::DOCUMENTELEMENT;
        setPaintChart();
    }
    void setPaintChart();

    void moveToCreate(int x, int y);
    int moveToLink(int x, int y);

    void doneCreate();
    void doneLink();

    void copy();
    void paste(int x, int y);

private:
    Canvas *canvas;

    PaintChartType curPaintChartType = PaintChartType::NONE; // 绘制的图形的类型
    FlowchartElement *curPaintChart = nullptr;               // 将要放置的图形
    FlowchartElement *curPasteChart = nullptr;               // 当前复制的图形
    int magPointToIndex;                                     // 要创建的磁力点的索引
    Line *newLineChart = nullptr;                            // 要创建的磁力点的连线的指针
    FlowchartElement *newLineToSelectChart = nullptr;        // 要连接到的磁力点的图形的指针
    FlowchartElement *newLineFromSelectChart = nullptr;      // 要创建的磁力点的图形的指针
    DIRECTION magPointDirect = DIRECTION::NONE;              // 选中时要创建的起始磁力点的方向类型
    int magPointFromIndex;                                   // 要创建的磁力点的索引
};

#endif // DRAWER_H
