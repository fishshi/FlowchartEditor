#ifndef UPDATER_H
#define UPDATER_H

#include "../view/canvas.h"

#include <unordered_set>

class Updater
{
    friend class Controller;

public:
    Updater(Canvas *canvas);

    void setSelChartLineColor(const QColor &color);
    void setSelChartFillColor(const QColor &color);

    void moveToChangeSize(int x, int y);
    void moveToChangePos(int x, int y);
    void setSelecChart(FlowchartElement *cb, int x, int y);

    void doneChangeSize();

    void frameSelect(int x, int y);
    void doneFrameSelect();
    void clearFrameSelect();

    void search(QString target);
    void replace(QString target, QString newTarget);

private:
    bool isFrameSelected = false;
    int frameX;
    int frameY;
    std::unordered_set<FlowchartElement *> frameSelCharts;
    QPoint curSelecFramePos; // 选中时指针在框架上的位置信息
    void moveToChangeFramePos(int x, int y);

    QPoint curSelecChartPos; // 选中时指针在图像对象上的位置信息
    Canvas *canvas;
    DIRECTION sizePointDirect = DIRECTION::NONE; // 选中时要改变大小的大小点的方向类型
};

#endif // UPDATER_H
