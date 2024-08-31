#ifndef DRAWER_H
#define DRAWER_H

#include "../view/canvas.h"

class Drawer
{
public:
    Drawer(Canvas *canvas);

    void setPaintProcessElement(){canvas->curPaintChartType = PaintChartType::RECT;setPaintChart();}                // 获取一个新的矩形
    void setPaintDecisionElement(){canvas->curPaintChartType = PaintChartType::DIAMOND;setPaintChart();}          // 获取一个新的菱形
    void setPaintStartEndElement(){canvas->curPaintChartType = PaintChartType::ROUNDRECT;setPaintChart();}      // 获取一个新的圆角矩形
    void setPaintConnectorElement(){canvas->curPaintChartType = PaintChartType::ELLIPSE;setPaintChart();}          // 获取一个新的圆形
    void setPaintLine(){canvas->curPaintChartType = PaintChartType::LINE;setPaintChart();}                // 获取一个新的线段
    void setPaintDataElement(){canvas->curPaintChartType = PaintChartType::PARALLELOGRAM;setPaintChart();}      // 获取一个新的梯形
    void setPaintChart();

    void clickToCreate(int x, int y);
    void moveToCreate(int x, int y);
    int moveToLink(int x, int y);

private:
    Canvas *canvas;
};

#endif // DRAWER_H
