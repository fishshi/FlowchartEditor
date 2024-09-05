#include "documentelement.h"

void DocumentElement::paintChart(QPainter &p)
{
    QPen originalPen = p.pen();
    p.setPen(paintChartDrawPen);

    int sx = paintStart.rx(), sy = paintStart.ry(), ex = paintEnd.rx(), ey = paintEnd.ry();

    // 删除旧的路径对象
    if (graphPath)
        delete graphPath;
    graphPath = new QPainterPath;

    // 创建一个带有波浪底边的矩形
    graphPath->moveTo(sx, sy);
    graphPath->lineTo(ex, sy);
    graphPath->lineTo(ex, sy + (ey - sy) / 20 * 17);
    graphPath->quadTo(sx + (ex - sx) / 4 * 3, sy + (ey - sy) / 5 * 4, (ex + sx) / 2, sy + (ey - sy) / 10 * 9);
    graphPath->quadTo(sx + (ex - sx) / 4, ey, sx, sy + (ey - sy) / 20 * 19);
    graphPath->lineTo(sx, sy);

    // 填充和绘制路径
    p.fillPath(*graphPath, paintChartFillPen);
    p.drawPath(*graphPath);

    p.setPen(originalPen);
}

void DocumentElement::updateMagPointInfo()
{
    int x1 = paintStart.rx(), y1 = paintStart.ry();
    int x2 = paintEnd.rx(), y2 = paintEnd.ry();
    int midx = ((paintStart.rx() + paintEnd.rx()) >> 1), midy = ((paintStart.ry() + paintEnd.ry()) >> 1);

    magPoint.i_point[0]->setX(midx);
    magPoint.i_point[0]->setY(y1);
    magPoint.i_point[0]->setRotate(DIRECTION::NORTH);
    magPoint.i_point[1]->setX(x2);
    magPoint.i_point[1]->setY(midy);
    magPoint.i_point[1]->setRotate(DIRECTION::EAST);
    magPoint.i_point[2]->setX(midx);
    magPoint.i_point[2]->setY(y1 + (y2 - y1) / 10 * 9);
    magPoint.i_point[2]->setRotate(DIRECTION::SOUTH);
    magPoint.i_point[3]->setX(x1);
    magPoint.i_point[3]->setY(midy);
    magPoint.i_point[3]->setRotate(DIRECTION::WEST);
}
