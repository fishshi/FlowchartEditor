#ifndef DATAELEMENT_H
#define DATAELEMENT_H

#include "flowchartelement.h"

class DataElement : public FlowchartElement
{
public:
    DataElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::PARALLELOGRAM) : FlowchartElement(parent, type) {}
    ~DataElement() {}

private:
    virtual void paintChart(QPainter &p);
    virtual void updateMagPointInfo();
};

#endif // DATAELEMENT_H
