#ifndef CHART_ROUND_H
#define CHART_ROUND_H
#include "flowchartelement.h"

class ConnectorElement : public FlowchartElement
{
private:
    virtual void paintChart(QPainter & p);

public:
    ConnectorElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::ELLIPSE) : FlowchartElement(parent,type){}
    ConnectorElement(ConnectorElement &cr):FlowchartElement(cr){}
    ConnectorElement( int x1, int y1, int x2, int y2, QWidget *parent = nullptr,PaintChartType type = PaintChartType::ELLIPSE) : FlowchartElement(x1,y1,x2,y2,parent,type){}
    ~ConnectorElement(){}
};

#endif // CHART_ROUND_H
