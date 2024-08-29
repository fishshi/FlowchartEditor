#ifndef DRAWER_H
#define DRAWER_H

#include "../view/canvas.h"
#include "../model/config.h"
#include "../model/elements/flowchartelement.h"

class Drawer
{
    friend class Controller;
public:
    Drawer(Canvas *canvas);

    void setPaintProcessElement(){curPaintChartType = PaintChartType::RECT;setPaintChart();}                // 获取一个新的矩形
    void setPaintDecisionElement(){curPaintChartType = PaintChartType::DIAMOND;setPaintChart();}          // 获取一个新的菱形
    void setPaintStartEndElement(){curPaintChartType = PaintChartType::ROUNDRECT;setPaintChart();}      // 获取一个新的圆角矩形
    void setPaintConnectorElement(){curPaintChartType = PaintChartType::ELLIPSE;setPaintChart();}          // 获取一个新的圆形
    void setPaintLine(){curPaintChartType = PaintChartType::LINE;setPaintChart();}                // 获取一个新的线段
    void setPaintDataElement(){curPaintChartType = PaintChartType::PARALLELOGRAM;setPaintChart();}      // 获取一个新的梯形
    void resetPaintChartType(){curPaintChartType = PaintChartType::NONE;}                               // 清空类型
    void setPaintChart();                          // 设置将要绘制的图形

private:
    PaintChartType curPaintChartType = PaintChartType::NONE;    // 绘制的图形的类型
    FlowchartElement *curPaintChart = nullptr;    // 将要放置的图形
    Canvas *canvas;
};

#endif // DRAWER_H
