#ifndef SUBPROCESSELEMENT_H
#define SUBPROCESSELEMENT_H

#include "flowchartelement.h"

class SubprocessElement : public FlowchartElement
{
public:
    SubprocessElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::SUBPROCESSELEMENT) : FlowchartElement(parent, type) {}
    ~SubprocessElement() {}

private:
    virtual void paintChart(QPainter &p);
};

#endif // SUBPROCESSELEMENT_H
