#ifndef STARTENDELEMENT_H
#define STARTENDELEMENT_H

#include "flowchartelement.h"

class StartEndElement : public FlowchartElement
{
public:
    StartEndElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::ROUNDRECT) : FlowchartElement(parent, type) {}

private:
    virtual void paintChart(QPainter &p);
};

#endif // STARTENDELEMENT_H
