#include "processelement.h"


void ProcessElement::paintChart(QPainter & p)
{
    QPen tmp = p.pen();
    p.setPen(paintChartDrawPen);

    int sx = paintStart.rx(),sy = paintStart.ry(),ex = paintEnd.rx(),ey = paintEnd.ry();

    if(graphPath) delete graphPath;
    graphPath = new QPainterPath;
    graphPath->addRect(sx,sy,ex-sx,ey-sy);

    p.fillPath(*graphPath,paintChartFillPen);
    p.drawPath(*graphPath);

    p.setPen(tmp);
}
