#ifndef PROCESSELEMENT_H
#define PROCESSELEMENT_H

#include "flowchartelement.h"

class ProcessElement : public FlowchartElement
{
public:
    ProcessElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::RECT) : FlowchartElement(parent, type) {}

private:
    virtual void paintChart(QPainter &p);
};

#endif // PROCESSELEMENT_H
