#ifndef DOCUMENTELEMENT_H
#define DOCUMENTELEMENT_H

#include "flowchartelement.h"

class DocumentElement : public FlowchartElement
{
public:
    DocumentElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::DOCUMENTELEMENT) : FlowchartElement(parent, type) {}

private:
    virtual void paintChart(QPainter &p);
    virtual void updateMagPointInfo();
};

#endif // DOCUMENTELEMENT_H
