#ifndef PROCESSELEMENT_H
#define PROCESSELEMENT_H
#include "flowchartelement.h"

class ProcessElement : public FlowchartElement
{
private:
    virtual void paintChart(QPainter & p);

public:
    ProcessElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::RECT) : FlowchartElement(parent,type){}
    ~ProcessElement(){}
};

#endif // PROCESSELEMENT_H
