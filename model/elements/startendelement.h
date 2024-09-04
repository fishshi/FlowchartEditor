#ifndef STARTENDELEMENT_H
#define STARTENDELEMENT_H
#include "flowchartelement.h"

class StartEndElement : public FlowchartElement
{
private:
    virtual void paintChart(QPainter & p);

public:
    StartEndElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::ROUNDRECT) : FlowchartElement(parent,type){}
    ~StartEndElement(){}
};

#endif // STARTENDELEMENT_H
