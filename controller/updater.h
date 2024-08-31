#ifndef UPDATER_H
#define UPDATER_H

#include "../view/canvas.h"

class Updater
{
    friend class Controller;
public:
    Updater(Canvas *canvas);

    void setSelChartLineColor(const QColor &color);
    void setSelChartFillColor(const QColor &color);

    void moveToChangeSize(int x, int y);
    void moveToChangePos(int x, int y);
    void setSelecChart(FlowchartElement * cb, int x, int y);

    void doneChangeSize();

private:
    QPoint curSelecChartPos;                // 选中时指针在图像对象上的位置信息
    Canvas *canvas;
    ORIENTION sizePointDirect = ORIENTION::NONE;    // 选中时要改变大小的大小点的方向类型
};

#endif // UPDATER_H
