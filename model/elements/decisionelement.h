#ifndef DECISIONELEMENT_H
#define DECISIONELEMENT_H
#include "flowchartelement.h"

class DecisionElement : public FlowchartElement
{
private:
    virtual void paintChart(QPainter & p);
public:
    DecisionElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::DIAMOND) : FlowchartElement(parent,type){}
    ~DecisionElement(){}
};

#endif // DECISIONELEMENT_H
