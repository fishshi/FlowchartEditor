#ifndef DATAELEMENT_H
#define DATAELEMENT_H
#include "flowchartelement.h"

class DataElement : public FlowchartElement
{
private:
    virtual void paintChart(QPainter & p);
    virtual void updateMagPointInfo();

public:
    DataElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::PARALLELOGRAM) : FlowchartElement(parent,type){}
    DataElement(DataElement &cr):FlowchartElement(cr){}
    DataElement( int x1, int y1, int x2, int y2, QWidget *parent = nullptr,PaintChartType type = PaintChartType::RECT) : FlowchartElement(x1,y1,x2,y2,parent,type){}
    ~DataElement(){}
};

#endif // DATAELEMENT_H
