#include "subprocesselement.h"

void SubprocessElement::paintChart(QPainter & p)
{
    QPen tmp = p.pen();
    p.setPen(paintChartDrawPen);

    int sx = paintStart.rx(),sy = paintStart.ry(),ex = paintEnd.rx(),ey = paintEnd.ry();

    if(graphPath) delete graphPath;
    graphPath = new QPainterPath;
    graphPath->addRect(sx,sy,ex-sx,ey-sy);

    int x1 = sx + (ex - sx) / 10;
    graphPath->moveTo(x1, sy);
    graphPath->lineTo(x1, ey);

    int x2 = sx + 9 * (ex - sx) / 10;
    graphPath->moveTo(x2, sy);
    graphPath->lineTo(x2, ey);

    p.fillPath(*graphPath,paintChartFillPen);
    p.drawPath(*graphPath);

    p.setPen(tmp);
}
