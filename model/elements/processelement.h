#ifndef PROCESSELEMENT_H
#define PROCESSELEMENT_H
#include "flowchartelement.h"

class ProcessElement : public FlowchartElement
{
private:
    virtual void paintChart(QPainter & p);
    virtual void updateMagPointInfo();

public:
    ProcessElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::RECT) : FlowchartElement(parent,type,true,8){}
    ProcessElement(ProcessElement &cr):FlowchartElement(cr){}
    ProcessElement( int x1, int y1, int x2, int y2, QWidget *parent = nullptr,PaintChartType type = PaintChartType::RECT) : FlowchartElement(x1,y1,x2,y2,parent,type){}
    ~ProcessElement(){}


};

#endif // PROCESSELEMENT_H
