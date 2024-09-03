#ifndef SUBPROCESSELEMENT_H
#define SUBPROCESSELEMENT_H

#include "flowchartelement.h"

class SubprocessElement : public FlowchartElement
{
private:
    virtual void paintChart(QPainter & p);

public:
    SubprocessElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::SUBPROCESSELEMENT) : FlowchartElement(parent,type){}
    SubprocessElement(SubprocessElement &cr):FlowchartElement(cr){}
    SubprocessElement( int x1, int y1, int x2, int y2, QWidget *parent = nullptr,PaintChartType type = PaintChartType::ROUNDRECT) : FlowchartElement(x1,y1,x2,y2,parent,type){}
    ~SubprocessElement(){}
};

#endif // SUBPROCESSELEMENT_H
