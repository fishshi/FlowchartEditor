#ifndef DOCUMENTELEMENT_H
#define DOCUMENTELEMENT_H

#include "flowchartelement.h"

class DocumentElement : public FlowchartElement
{
private:
    virtual void paintChart(QPainter & p);
    virtual void updateMagPointInfo();

public:
    DocumentElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::DOCUMENTELEMENT) : FlowchartElement(parent,type){}
    ~DocumentElement(){}
};

#endif // DOCUMENTELEMENT_H
