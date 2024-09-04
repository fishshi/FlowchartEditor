#ifndef SUBPROCESSELEMENT_H
#define SUBPROCESSELEMENT_H

#include "flowchartelement.h"

class SubprocessElement : public FlowchartElement
{
private:
    virtual void paintChart(QPainter & p);

public:
    SubprocessElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::SUBPROCESSELEMENT) : FlowchartElement(parent,type){}
    ~SubprocessElement(){}
};

#endif // SUBPROCESSELEMENT_H
