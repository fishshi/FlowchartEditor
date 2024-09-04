#ifndef CHART_ROUND_H
#define CHART_ROUND_H
#include "flowchartelement.h"

class ConnectorElement : public FlowchartElement
{
private:
    virtual void paintChart(QPainter & p);

public:
    ConnectorElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::ELLIPSE) : FlowchartElement(parent,type){}
    ~ConnectorElement(){}
};

#endif // CHART_ROUND_H
