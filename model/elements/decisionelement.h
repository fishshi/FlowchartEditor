#ifndef DECISIONELEMENT_H
#define DECISIONELEMENT_H

#include "flowchartelement.h"

class DecisionElement : public FlowchartElement
{
public:
    DecisionElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::DIAMOND) : FlowchartElement(parent, type) {}
    ~DecisionElement() {}

private:
    virtual void paintChart(QPainter &p);
};

#endif // DECISIONELEMENT_H
