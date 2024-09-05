#include "dataelement.h"

void DataElement::paintChart(QPainter &p)
{
    QPen tmp = p.pen();
    p.setPen(paintChartDrawPen);
    int sx = paintStart.rx(), sy = paintStart.ry(), ex = paintEnd.rx(), ey = paintEnd.ry();
    if (graphPath)
        delete graphPath;
    graphPath = new QPainterPath;
    graphPath->moveTo(sx + ((ex - sx) / 4), sy);
    graphPath->lineTo(ex, sy);
    graphPath->lineTo(ex - ((ex - sx) / 4), ey);
    graphPath->lineTo(sx, ey);
    graphPath->lineTo(sx + ((ex - sx) / 4), sy);
    p.fillPath(*graphPath, paintChartFillPen);
    p.drawPath(*graphPath);
    p.setPen(tmp);
}

void DataElement::updateMagPointInfo()
{
    int x1 = paintStart.rx(), y1 = paintStart.ry();
    int x2 = paintEnd.rx(), y2 = paintEnd.ry();
    int midx = ((paintStart.rx() + paintEnd.rx()) >> 1), midy = ((paintStart.ry() + paintEnd.ry()) >> 1);
    int ext = (x2 - x1) >> 2;

    magPoint.i_point[0]->setX(midx);
    magPoint.i_point[0]->setY(y1);
    magPoint.i_point[0]->setRotate(DIRECTION::NORTH);
    magPoint.i_point[1]->setX((x2 + x2 - ext) >> 1);
    magPoint.i_point[1]->setY(midy);
    magPoint.i_point[1]->setRotate(DIRECTION::EAST);
    magPoint.i_point[2]->setX(midx);
    magPoint.i_point[2]->setY(y2);
    magPoint.i_point[2]->setRotate(DIRECTION::SOUTH);
    magPoint.i_point[3]->setX((x1 + x1 + ext) >> 1);
    magPoint.i_point[3]->setY(midy);
    magPoint.i_point[3]->setRotate(DIRECTION::WEST);
}
