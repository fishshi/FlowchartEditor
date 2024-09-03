#ifndef FLOWCHART_H
#define FLOWCHART_H
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
#include "../model/elements/subprocesselement.h"

#include "../model/config.h"

class Canvas : public QWidget
{
    Q_OBJECT
    friend class Controller;
    friend class Drawer;
    friend class Updater;
    friend class Remover;
    friend class Filer;
private:
    std::vector<FlowchartElement*> charts;    // 画板上图形的集合
    std::vector<FlowchartElement*> line;      // 画板上线的集合
    FlowchartElement *curSelecChart = nullptr;    // 选中的画板上的图形
    FlowchartElement *lineSelectChart = nullptr;  // 线的吸附到的图形的指针
    int magPointIndex;                      // 线吸附到的图形的磁力点的索引
    void clearChartsLine();                      // 释放图形、连线内存

public:
    explicit Canvas(QWidget *parent = nullptr , Qt::WindowFlags f = Qt::WindowFlags());
    void hideMagSizeAll();              // 隐藏所有大小、磁力点

    bool isframe = false;
    int x1, y1, x2, y2;
    void setDrawFrame(int x1, int y1, int x2, int y2);
    void moveFrameBy(int x, int y);
    void unsetDrawFrame();

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
