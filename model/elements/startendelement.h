#ifndef STARTENDELEMENT_H
#define STARTENDELEMENT_H
#include "flowchartelement.h"

class StartEndElement : public FlowchartElement
{
private:
    virtual void paintChart(QPainter & p);

public:
    StartEndElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::ROUNDRECT) : FlowchartElement(parent,type){}
    StartEndElement(StartEndElement &cr):FlowchartElement(cr){}
    StartEndElement( int x1, int y1, int x2, int y2, QWidget *parent = nullptr,PaintChartType type = PaintChartType::ROUNDRECT) : FlowchartElement(x1,y1,x2,y2,parent,type){}
    ~StartEndElement(){}
};

#endif // STARTENDELEMENT_H
