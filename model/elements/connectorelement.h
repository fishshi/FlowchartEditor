#ifndef CHART_ROUND_H
#define CHART_ROUND_H

#include "flowchartelement.h"

class ConnectorElement : public FlowchartElement
{
public:
    ConnectorElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::ELLIPSE) : FlowchartElement(parent, type) {}

private:
    virtual void paintChart(QPainter &p);
};

#endif // CHART_ROUND_H
