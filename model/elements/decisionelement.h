#ifndef DECISIONELEMENT_H
#define DECISIONELEMENT_H
#include "flowchartelement.h"

class DecisionElement : public FlowchartElement
{
private:
    virtual void paintChart(QPainter & p);
public:
    DecisionElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::DIAMOND) : FlowchartElement(parent,type,true){}
    DecisionElement(DecisionElement &cr):FlowchartElement(cr){}
    DecisionElement( int x1, int y1, int x2, int y2, QWidget *parent = nullptr,PaintChartType type = PaintChartType::DIAMOND) : FlowchartElement(x1,y1,x2,y2,parent,type){}
    ~DecisionElement(){}
};

#endif // DECISIONELEMENT_H
