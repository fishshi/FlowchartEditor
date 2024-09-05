#include "line.h"

void Line::drawLineHead(const DIRECTION direction, const LINE_HEAD_TYPE lht, const int x, const int y, QPainter &p, QPainterPath &linePath, QPainterPath &graphPath)
{
    switch (direction)
    {
    case DIRECTION::NORTH:
    {
        linePath.moveTo(x, y - extendWidth);
        linePath.lineTo(x, y - arrayLength);
        graphPath.addRect(x - containsWidth1_2, y, containsWidth, -extendWidth);
        switch (lht)
        {
        case LINE_HEAD_TYPE::ARROW:
            linePath.lineTo(x, y);
            linePath.lineTo(x - arrayWidth, y - arrayLength);
            linePath.moveTo(x, y);
            linePath.lineTo(x + arrayWidth, y - arrayLength);
            linePath.moveTo(x, y);
            break;
        case LINE_HEAD_TYPE::NOARROW:
            linePath.lineTo(x, y);
            break;
        }
    }
    break;
    case DIRECTION::EAST:
    {
        linePath.moveTo(x + extendWidth, y);
        linePath.lineTo(x + arrayLength, y);
        graphPath.addRect(x, y - containsWidth1_2, extendWidth, containsWidth);
        switch (lht)
        {
        case LINE_HEAD_TYPE::ARROW:
            linePath.lineTo(x, y);
            linePath.lineTo(x + arrayLength, y + arrayWidth);
            linePath.moveTo(x, y);
            linePath.lineTo(x + arrayLength, y - arrayWidth);
            linePath.moveTo(x, y);
            break;
        case LINE_HEAD_TYPE::NOARROW:
            linePath.lineTo(x, y);
            break;
        }
    }
    break;
    case DIRECTION::SOUTH:
    {
        linePath.moveTo(x, y + extendWidth);
        linePath.lineTo(x, y + arrayLength);
        graphPath.addRect(x - containsWidth1_2, y, containsWidth, extendWidth);
        switch (lht)
        {
        case LINE_HEAD_TYPE::ARROW:
            linePath.lineTo(x, y);
            linePath.lineTo(x - arrayWidth, y + arrayLength);
            linePath.moveTo(x, y);
            linePath.lineTo(x + arrayWidth, y + arrayLength);
            linePath.moveTo(x, y);
            break;
        case LINE_HEAD_TYPE::NOARROW:
            linePath.lineTo(x, y);
            break;
        }
    }
    break;
    case DIRECTION::WEST:
    {
        linePath.moveTo(x - extendWidth, y);
        linePath.lineTo(x - arrayLength, y);
        graphPath.addRect(x, y - containsWidth1_2, -extendWidth, containsWidth);
        switch (lht)
        {
        case LINE_HEAD_TYPE::ARROW:
            linePath.lineTo(x, y);
            linePath.lineTo(x - arrayLength, y + arrayWidth);
            linePath.moveTo(x, y);
            linePath.lineTo(x - arrayLength, y - arrayWidth);
            linePath.moveTo(x, y);
            break;
        case LINE_HEAD_TYPE::NOARROW:
            linePath.lineTo(x, y);
            break;
        }
    }
    break;
    }
}

void Line::drawStraightLine(int sx, int sy, int ex, int ey, QPainterPath &linePath, QPainterPath &graphPath)
{
    linePath.moveTo(sx, sy);
    linePath.lineTo(ex, ey);
    if (sx < ex && sy < ey)
    {
        QPolygonF tmp;
        tmp << QPointF(sx, sy) << QPointF(sx + containsWidth1_2, sy) << QPointF(ex, ey - containsWidth1_2) << QPointF(ex, ey) << QPointF(ex - containsWidth1_2, ey) << QPointF(sx, sy + containsWidth1_2) << QPointF(sx, sy);
        graphPath.addPolygon(tmp);
    }
    else if (sx > ex && sy > ey)
    {
        QPolygonF tmp;
        tmp << QPointF(ex, ey) << QPointF(ex + containsWidth1_2, ey) << QPointF(sx, sy - containsWidth1_2) << QPointF(sx, sy) << QPointF(sx - containsWidth1_2, sy) << QPointF(ex, ey + containsWidth1_2) << QPointF(ex, ey);
        graphPath.addPolygon(tmp);
    }
    else if (sx < ex)
    {
        QPolygonF tmp;
        tmp << QPointF(sx, sy) << QPointF(sx + containsWidth1_2, sy) << QPointF(ex, ey + containsWidth1_2) << QPointF(ex, ey) << QPointF(ex - containsWidth1_2, ey) << QPointF(sx, sy - containsWidth1_2) << QPointF(sx, sy);
        graphPath.addPolygon(tmp);
    }
    else
    {
        QPolygonF tmp;
        tmp << QPointF(ex, ey) << QPointF(ex + containsWidth1_2, ey) << QPointF(sx, sy + containsWidth1_2) << QPointF(sx, sy) << QPointF(sx - containsWidth1_2, sy) << QPointF(ex, ey - containsWidth1_2) << QPointF(ex, ey);
        graphPath.addPolygon(tmp);
    }
}

void Line::paintChart(QPainter &p)
{
    QPen tmp = p.pen();
    p.setPen(paintChartDrawPen);

    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), (height() >> 1) - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
    int midx, midy;
    int sx, sy, ex, ey;
    QPainterPath linePath;

    if (graphPath)
        delete graphPath;
    graphPath = new QPainterPath;

    switch (startDirect)
    {
    case DIRECTION::NORTH:
    {
        sx = startPos.rx();
        sy = startPos.ry() - extendWidth;
        drawLineHead(startDirect, startLineHeadType, startPos.rx(), startPos.ry(), p, linePath, *graphPath);
        linePath.moveTo(sx, sy);
        switch (endDirect)
        {
        case DIRECTION::NORTH:
        {
            ex = endPos.rx();
            ey = endPos.ry() - extendWidth;
            if (sy > ey)
            {
                linePath.lineTo(sx, ey);
                linePath.lineTo(ex, ey);
                graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, ey - sy);
                graphPath->addRect(sx, ey - containsWidth1_2, ex - sx, containsWidth);
                chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), ey - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
            }
            else
            {
                linePath.lineTo(ex, sy);
                linePath.lineTo(ex, ey);
                graphPath->addRect(sx, sy + containsWidth1_2, ex - sx, -containsWidth);
                graphPath->addRect(ex - containsWidth1_2, sy, containsWidth, ey - sy);
                chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), sy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
            }
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        break;
        case DIRECTION::EAST:
        {
            ex = endPos.rx() + extendWidth;
            ey = endPos.ry();
            if (sy > ey)
            {
                if (sx > ex)
                {
                    linePath.lineTo(sx, ey);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, ey - sy);
                    graphPath->addRect(sx, ey - containsWidth1_2, ex - sx, containsWidth);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), ey - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
                else
                {
                    midy = (sy + ey) >> 1;
                    linePath.lineTo(sx, midy);
                    linePath.lineTo(ex, midy);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, midy - sy);
                    graphPath->addRect(sx, midy - containsWidth1_2, ex - sx, containsWidth);
                    graphPath->addRect(ex - containsWidth1_2, midy, containsWidth, ey - midy);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), midy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
            }
            else
            {
                if (sx > ex)
                {
                    midx = (sx + ex) >> 1;
                    linePath.lineTo(midx, sy);
                    linePath.lineTo(midx, ey);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx, sy - containsWidth1_2, midx - sx, containsWidth);
                    graphPath->addRect(midx - containsWidth1_2, sy, containsWidth, ey - sy);
                    graphPath->addRect(midx, ey - containsWidth1_2, ex - midx, containsWidth);
                    chartText.text->setGeometry(midx - (chartText.text->width() >> 1), (height() >> 1) - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
                else
                {
                    linePath.lineTo(ex, sy);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx, sy - containsWidth1_2, ex - sx, containsWidth);
                    graphPath->addRect(ex - containsWidth1_2, sy, containsWidth, ey - sy);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), sy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
            }
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        break;
        case DIRECTION::SOUTH:
        {
            ex = endPos.rx();
            ey = endPos.ry() + extendWidth;
            if (sy > ey)
            {
                midy = (sy + ey) >> 1;
                linePath.lineTo(sx, midy);
                linePath.lineTo(ex, midy);
                linePath.lineTo(ex, ey);
                graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, midy - sy);
                graphPath->addRect(sx, midy - containsWidth1_2, ex - sx, containsWidth);
                graphPath->addRect(ex - containsWidth1_2, midy, containsWidth, ey - midy);
                chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), midy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
            }
            else
            {
                midx = (sx + ex) >> 1;
                linePath.lineTo(midx, sy);
                linePath.lineTo(midx, ey);
                linePath.lineTo(ex, ey);
                graphPath->addRect(sx, sy - containsWidth1_2, midx - sx, containsWidth);
                graphPath->addRect(midx - containsWidth1_2, sy, containsWidth, ey - sy);
                graphPath->addRect(midx, ey - containsWidth1_2, ex - midx, containsWidth);
                chartText.text->setGeometry(midx - (chartText.text->width() >> 1), (height() >> 1) - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
            }
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        break;
        case DIRECTION::WEST:
        {
            ex = endPos.rx() - extendWidth;
            ey = endPos.ry();
            if (sy > ey)
            {
                if (sx > ex)
                {
                    midy = (sy + ey) >> 1;
                    linePath.lineTo(sx, midy);
                    linePath.lineTo(ex, midy);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, midy - sy);
                    graphPath->addRect(sx, midy - containsWidth1_2, ex - sx, containsWidth);
                    graphPath->addRect(ex - containsWidth1_2, midy, containsWidth, ey - midy);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), midy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
                else
                {
                    linePath.lineTo(sx, ey);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, ey - sy);
                    graphPath->addRect(sx, ey - containsWidth1_2, ex - sx, containsWidth);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), ey - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
            }
            else
            {
                if (sx > ex)
                {
                    linePath.lineTo(ex, sy);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx, sy - containsWidth1_2, ex - sx, containsWidth);
                    graphPath->addRect(ex - containsWidth1_2, sy, containsWidth, ey - sy);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), sy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
                else
                {
                    midx = (sx + ex) >> 1;
                    linePath.lineTo(midx, sy);
                    linePath.lineTo(midx, ey);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx, sy - containsWidth1_2, midx - sx, containsWidth);
                    graphPath->addRect(midx - containsWidth1_2, sy, containsWidth, ey - sy);
                    graphPath->addRect(midx, ey - containsWidth1_2, ex - midx, containsWidth);
                    chartText.text->setGeometry(midx - (chartText.text->width() >> 1), (height() >> 1) - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
            }
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        break;
        case DIRECTION::ENDPOINT:
        {
            drawStraightLine(sx, sy, endPos.rx(), endPos.ry(), linePath, *graphPath);
        }
        break;
        }
    }
    break;
    case DIRECTION::EAST:
    {

        sx = startPos.rx() + extendWidth;
        sy = startPos.ry();
        drawLineHead(startDirect, startLineHeadType, startPos.rx(), startPos.ry(), p, linePath, *graphPath);
        linePath.moveTo(sx, sy);

        switch (endDirect)
        {
        case DIRECTION::NORTH:
        {
            ex = endPos.rx();
            ey = endPos.ry() - extendWidth;
            if (sy > ey)
            {
                if (sx > ex)
                {
                    linePath.lineTo(sx, ey);
                    linePath.lineTo(ex, ey);
                    linePath.lineTo(endPos.rx(), endPos.ry());
                    graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, ey - sy);
                    graphPath->addRect(sx, ey - containsWidth1_2, ex - sx, containsWidth);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), ey - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
                else
                {
                    midx = (sx + ex) >> 1;
                    linePath.lineTo(midx, sy);
                    linePath.lineTo(midx, ey);
                    linePath.lineTo(ex, ey);
                    linePath.lineTo(endPos.rx(), endPos.ry());
                    graphPath->addRect(sx, sy - containsWidth1_2, midx - sx, containsWidth);
                    graphPath->addRect(midx - containsWidth1_2, sy, containsWidth, ey - sy);
                    graphPath->addRect(midx, ey - containsWidth1_2, ex - midx, containsWidth);
                    chartText.text->setGeometry(midx - (chartText.text->width() >> 1), (height() >> 1) - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
            }
            else
            {
                if (sx > ex)
                {
                    midy = (sy + ey) >> 1;
                    linePath.lineTo(sx, midy);
                    linePath.lineTo(ex, midy);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, midy - sy);
                    graphPath->addRect(sx, midy - containsWidth1_2, ex - sx, containsWidth);
                    graphPath->addRect(ex - containsWidth1_2, midy, containsWidth, ey - midy);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), midy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
                else
                {
                    linePath.lineTo(ex, sy);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx, sy - containsWidth1_2, ex - sx, containsWidth);
                    graphPath->addRect(ex - containsWidth1_2, sy, containsWidth, ey - sy);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), sy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
            }
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        break;
        case DIRECTION::EAST:
        {
            ex = endPos.rx() + extendWidth;
            ey = endPos.ry();
            if (sx > ex)
            {
                linePath.lineTo(sx, ey);
                linePath.lineTo(ex, ey);
                graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, ey - sy);
                graphPath->addRect(sx, ey - containsWidth1_2, ex - sx, containsWidth);
                chartText.text->setGeometry(sx - (chartText.text->width() >> 1), (height() >> 1) - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
            }
            else
            {
                linePath.lineTo(ex, sy);
                linePath.lineTo(ex, ey);
                graphPath->addRect(sx, sy - containsWidth1_2, ex - sx, containsWidth);
                graphPath->addRect(ex - containsWidth1_2, sy, containsWidth, ey - sy);
                chartText.text->setGeometry(ex - (chartText.text->width() >> 1), (height() >> 1) - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
            }
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        break;
        case DIRECTION::SOUTH:
        {
            ex = endPos.rx();
            ey = endPos.ry() + extendWidth;
            if (sy > ey)
            {
                if (sx > ex)
                {
                    midy = (sy + ey) >> 1;
                    linePath.lineTo(sx, midy);
                    linePath.lineTo(ex, midy);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, midy - sy);
                    graphPath->addRect(sx, midy - containsWidth1_2, ex - sx, containsWidth);
                    graphPath->addRect(ex - containsWidth1_2, midy, containsWidth, ey - midy);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), midy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
                else
                {
                    linePath.lineTo(ex, sy);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx, sy + containsWidth1_2, ex - sx, -containsWidth);
                    graphPath->addRect(ex - containsWidth1_2, sy, containsWidth, ey - sy);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), ey - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
            }
            else
            {
                if (sx > ex)
                {
                    linePath.lineTo(sx, ey);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, ey - sy);
                    graphPath->addRect(sx, ey - containsWidth1_2, ex - sx, containsWidth);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), ey - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
                else
                {
                    midx = (sx + ex) >> 1;
                    linePath.lineTo(midx, sy);
                    linePath.lineTo(midx, ey);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx, sy - containsWidth1_2, midx - sx, containsWidth);
                    graphPath->addRect(midx - containsWidth1_2, sy, containsWidth, ey - sy);
                    graphPath->addRect(midx, ey - containsWidth1_2, ex - midx, containsWidth);
                    chartText.text->setGeometry(midx - (chartText.text->width() >> 1), (height() >> 1) - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
            }
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        break;
        case DIRECTION::WEST:
        {
            ex = endPos.rx() - extendWidth;
            ey = endPos.ry();
            if (sx > ex)
            {
                midy = (sy + ey) >> 1;
                linePath.lineTo(sx, midy);
                linePath.lineTo(ex, midy);
                linePath.lineTo(ex, ey);
                graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, midy - sy);
                graphPath->addRect(sx, midy - containsWidth1_2, ex - sx, containsWidth);
                graphPath->addRect(ex - containsWidth1_2, midy, containsWidth, ey - midy);
                chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), midy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
            }
            else
            {
                midx = (sx + ex) >> 1;
                linePath.lineTo(midx, sy);
                linePath.lineTo(midx, ey);
                linePath.lineTo(ex, ey);
                graphPath->addRect(sx, sy - containsWidth1_2, midx - sx, containsWidth);
                graphPath->addRect(midx - containsWidth1_2, sy, containsWidth, ey - sy);
                graphPath->addRect(midx, ey - containsWidth1_2, ex - midx, containsWidth);
                chartText.text->setGeometry(midx - (chartText.text->width() >> 1), (height() >> 1) - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
            }
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        break;
        case DIRECTION::ENDPOINT:
        {
            drawStraightLine(sx, sy, endPos.rx(), endPos.ry(), linePath, *graphPath);
        }
        break;
        }
    }
    break;
    case DIRECTION::SOUTH:
    {
        sx = startPos.rx();
        sy = startPos.ry() + extendWidth;
        drawLineHead(startDirect, startLineHeadType, startPos.rx(), startPos.ry(), p, linePath, *graphPath);
        linePath.moveTo(sx, sy);
        switch (endDirect)
        {
        case DIRECTION::NORTH:
        {
            ex = endPos.rx();
            ey = endPos.ry() - extendWidth;
            if (sy > ey)
            {
                midx = (sx + ex) >> 1;
                linePath.lineTo(midx, sy);
                linePath.lineTo(midx, ey);
                linePath.lineTo(ex, ey);
                graphPath->addRect(sx, sy - containsWidth1_2, midx - sx, containsWidth);
                graphPath->addRect(midx - containsWidth1_2, sy, containsWidth, ey - sy);
                graphPath->addRect(midx, ey - containsWidth1_2, ex - midx, containsWidth);
                chartText.text->setGeometry(midx - (chartText.text->width() >> 1), (height() >> 1) - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
            }
            else
            {
                midy = (sy + ey) >> 1;
                linePath.lineTo(sx, midy);
                linePath.lineTo(ex, midy);
                linePath.lineTo(ex, ey);
                graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, midy - sy);
                graphPath->addRect(sx, midy - containsWidth1_2, ex - sx, containsWidth);
                graphPath->addRect(ex - containsWidth1_2, midy, containsWidth, ey - midy);
                chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), midy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
            }
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        break;
        case DIRECTION::EAST:
        {
            ex = endPos.rx() + extendWidth;
            ey = endPos.ry();
            if (sy > ey)
            {
                if (sx > ex)
                {
                    midx = (sx + ex) >> 1;
                    linePath.lineTo(midx, sy);
                    linePath.lineTo(midx, ey);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx, sy - containsWidth1_2, midx - sx, containsWidth);
                    graphPath->addRect(midx - containsWidth1_2, sy, containsWidth, ey - sy);
                    graphPath->addRect(midx, ey - containsWidth1_2, ex - midx, containsWidth);
                    chartText.text->setGeometry(midx - (chartText.text->width() >> 1), (height() >> 1) - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
                else
                {
                    linePath.lineTo(ex, sy);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx, sy + containsWidth1_2, ex - sx, -containsWidth);
                    graphPath->addRect(ex - containsWidth1_2, sy, containsWidth, ey - sy);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), sy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
            }
            else
            {
                if (sx > ex)
                {
                    linePath.lineTo(sx, ey);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, ey - sy);
                    graphPath->addRect(sx, ey - containsWidth1_2, ex - sx, containsWidth);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), ey - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
                else
                {
                    midy = (sy + ey) >> 1;
                    linePath.lineTo(sx, midy);
                    linePath.lineTo(ex, midy);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, midy - sy);
                    graphPath->addRect(sx, midy - containsWidth1_2, ex - sx, containsWidth);
                    graphPath->addRect(ex - containsWidth1_2, midy, containsWidth, ey - midy);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), midy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
            }
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        break;
        case DIRECTION::SOUTH:
        {
            ex = endPos.rx();
            ey = endPos.ry() + extendWidth;
            if (sy > ey)
            {
                linePath.lineTo(ex, sy);
                linePath.lineTo(ex, ey);
                graphPath->addRect(sx, sy - containsWidth1_2, ex - sx, containsWidth);
                graphPath->addRect(ex - containsWidth1_2, sy, containsWidth, ey - sy);
                chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), sy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
            }
            else
            {
                linePath.lineTo(sx, ey);
                linePath.lineTo(ex, ey);
                graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, ey - sy);
                graphPath->addRect(sx, ey - containsWidth1_2, ex - sx, containsWidth);
                chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), ey - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
            }
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        break;
        case DIRECTION::WEST:
        {
            ex = endPos.rx() - extendWidth;
            ey = endPos.ry();
            if (sy > ey)
            {
                if (sx > ex)
                {
                    linePath.lineTo(ex, sy);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx, sy - containsWidth1_2, ex - sx, containsWidth);
                    graphPath->addRect(ex - containsWidth1_2, sy, containsWidth, ey - sy);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), sy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
                else
                {
                    midx = (sx + ex) >> 1;
                    linePath.lineTo(midx, sy);
                    linePath.lineTo(midx, ey);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx, sy - containsWidth1_2, midx - sx, containsWidth);
                    graphPath->addRect(midx - containsWidth1_2, sy, containsWidth, ey - sy);
                    graphPath->addRect(midx, ey - containsWidth1_2, ex - midx, containsWidth);
                    chartText.text->setGeometry(midx - (chartText.text->width() >> 1), (height() >> 1) - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
            }
            else
            {
                if (sx > ex)
                {
                    midy = (sy + ey) >> 1;
                    linePath.lineTo(sx, midy);
                    linePath.lineTo(ex, midy);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, midy - sy);
                    graphPath->addRect(sx, midy - containsWidth1_2, ex - sx, containsWidth);
                    graphPath->addRect(ex - containsWidth1_2, midy, containsWidth, ey - midy);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), midy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
                else
                {
                    linePath.lineTo(sx, ey);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, ey - sy);
                    graphPath->addRect(sx, ey - containsWidth1_2, ex - sx, containsWidth);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), ey - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
            }
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        break;
        case DIRECTION::ENDPOINT:
        {
            drawStraightLine(sx, sy, endPos.rx(), endPos.ry(), linePath, *graphPath);
        }
        break;
        }
    }
    break;
    case DIRECTION::WEST:
    {
        sx = startPos.rx() - extendWidth;
        sy = startPos.ry();
        drawLineHead(startDirect, startLineHeadType, startPos.rx(), startPos.ry(), p, linePath, *graphPath);
        linePath.moveTo(sx, sy);
        switch (endDirect)
        {
        case DIRECTION::NORTH:
        {
            ex = endPos.rx();
            ey = endPos.ry() - extendWidth;
            if (sy > ey)
            {
                if (sx > ex)
                {
                    midx = (sx + ex) >> 1;
                    linePath.lineTo(midx, sy);
                    linePath.lineTo(midx, ey);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx, sy - containsWidth1_2, midx - sx, containsWidth);
                    graphPath->addRect(midx - containsWidth1_2, sy, containsWidth, ey - sy);
                    graphPath->addRect(midx, ey - containsWidth1_2, ex - midx, containsWidth);
                    chartText.text->setGeometry(midx - (chartText.text->width() >> 1), (height() >> 1) - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
                else
                {
                    linePath.lineTo(sx, ey);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, ey - sy);
                    graphPath->addRect(sx, ey - containsWidth1_2, ex - sx, containsWidth);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), ey - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
            }
            else
            {
                if (sx > ex)
                {
                    linePath.lineTo(ex, sy);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, ey - sy);
                    graphPath->addRect(sx, ey - containsWidth1_2, ex - sx, containsWidth);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), sy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
                else
                {
                    midy = (sy + ey) >> 1;
                    linePath.lineTo(sx, midy);
                    linePath.lineTo(ex, midy);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, midy - sy);
                    graphPath->addRect(sx, midy - containsWidth1_2, ex - sx, containsWidth);
                    graphPath->addRect(ex - containsWidth1_2, midy, containsWidth, ey - midy);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), midy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
            }
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        break;
        case DIRECTION::EAST:
        {
            ex = endPos.rx() + extendWidth;
            ey = endPos.ry();
            if (sx > ex)
            {
                midx = (sx + ex) >> 1;
                linePath.lineTo(midx, sy);
                linePath.lineTo(midx, ey);
                linePath.lineTo(ex, ey);
                graphPath->addRect(sx, sy - containsWidth1_2, midx - sx, containsWidth);
                graphPath->addRect(midx - containsWidth1_2, sy, containsWidth, ey - sy);
                graphPath->addRect(midx, ey - containsWidth1_2, ex - midx, containsWidth);
                chartText.text->setGeometry(midx - (chartText.text->width() >> 1), (height() >> 1) - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
            }
            else
            {
                midy = (sy + ey) >> 1;
                linePath.lineTo(sx, midy);
                linePath.lineTo(ex, midy);
                linePath.lineTo(ex, ey);
                graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, midy - sy);
                graphPath->addRect(sx, midy - containsWidth1_2, ex - sx, containsWidth);
                graphPath->addRect(ex - containsWidth1_2, midy, containsWidth, ey - midy);
                chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), midy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
            }
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        break;
        case DIRECTION::SOUTH:
        {
            ex = endPos.rx();
            ey = endPos.ry() + extendWidth;
            if (sy > ey)
            {
                if (sx > ex)
                {
                    linePath.lineTo(ex, sy);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx, sy - containsWidth1_2, ex - sx, containsWidth);
                    graphPath->addRect(ex - containsWidth1_2, sy, containsWidth, ey - sy);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), sy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
                else
                {
                    midy = (sy + ey) >> 1;
                    linePath.lineTo(sx, midy);
                    linePath.lineTo(ex, midy);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, midy - sy);
                    graphPath->addRect(sx, midy - containsWidth1_2, ex - sx, containsWidth);
                    graphPath->addRect(ex - containsWidth1_2, midy, containsWidth, ey - midy);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), midy - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
            }
            else
            {
                if (sx > ex)
                {
                    midx = (sx + ex) >> 1;
                    linePath.lineTo(midx, sy);
                    linePath.lineTo(midx, ey);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx, sy - containsWidth1_2, midx - sx, containsWidth);
                    graphPath->addRect(midx - containsWidth1_2, sy, containsWidth, ey - sy);
                    graphPath->addRect(midx, ey - containsWidth1_2, ex - midx, containsWidth);
                    chartText.text->setGeometry(midx - (chartText.text->width() >> 1), (height() >> 1) - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
                else
                {
                    linePath.lineTo(sx, ey);
                    linePath.lineTo(ex, ey);
                    graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, ey - sy);
                    graphPath->addRect(sx, ey - containsWidth1_2, ex - sx, containsWidth);
                    chartText.text->setGeometry((width() >> 1) - (chartText.text->width() >> 1), ey - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
                }
            }
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        break;
        case DIRECTION::WEST:
        {
            ex = endPos.rx() - extendWidth;
            ey = endPos.ry();
            if (sx > ex)
            {
                linePath.lineTo(ex, sy);
                linePath.lineTo(ex, ey);
                graphPath->addRect(sx, sy - containsWidth1_2, ex - sx, containsWidth);
                graphPath->addRect(ex - containsWidth1_2, sy, containsWidth, ey - sy);
                chartText.text->setGeometry(ex - (chartText.text->width() >> 1), (height() >> 1) - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
            }
            else
            {
                linePath.lineTo(sx, ey);
                linePath.lineTo(ex, ey);
                graphPath->addRect(sx - containsWidth1_2, sy, containsWidth, ey - sy);
                graphPath->addRect(sx, ey - containsWidth1_2, ex - sx, containsWidth);
                chartText.text->setGeometry(sx - (chartText.text->width() >> 1), (height() >> 1) - (chartText.text->height() >> 1), chartText.text->width(), chartText.text->height());
            }
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        break;
        case DIRECTION::ENDPOINT:
        {
            drawStraightLine(sx, sy, endPos.rx(), endPos.ry(), linePath, *graphPath);
        }
        break;
        default:
        {
        }
        }
    }
    break;
    case DIRECTION::STARTPOINT:
    {
        sx = startPos.rx();
        sy = startPos.ry();
        ex = endPos.rx();
        ey = endPos.ry();
        if (endDirect == DIRECTION::NORTH)
        {
            drawStraightLine(sx, sy, ex, ey - extendWidth, linePath, *graphPath);
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        else if (endDirect == DIRECTION::EAST)
        {
            drawStraightLine(sx, sy, ex + extendWidth, ey, linePath, *graphPath);
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        else if (endDirect == DIRECTION::SOUTH)
        {
            drawStraightLine(sx, sy, ex, ey + extendWidth, linePath, *graphPath);
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        else if (endDirect == DIRECTION::WEST)
        {
            drawStraightLine(sx, sy, ex - extendWidth, ey, linePath, *graphPath);
            drawLineHead(endDirect, endLineHeadType, endPos.rx(), endPos.ry(), p, linePath, *graphPath);
        }
        else if (endDirect == DIRECTION::ENDPOINT)
        {
            drawStraightLine(sx, sy, ex, ey, linePath, *graphPath);
        }
    }
    }
    p.drawPath(linePath);
    p.setPen(QPen(QColor(0, 0, 0, 0), 0));
    graphPath->setFillRule(Qt::WindingFill);
    p.drawPath(*graphPath);
    p.setPen(tmp);
}

void Line::updateSizePointInfo()
{
    if (widgetStart.rx() > widgetEnd.rx())
    {
        startPos.setX(paintEnd.rx());
        endPos.setX(paintStart.rx());
    }
    else
    {
        startPos.setX(paintStart.rx());
        endPos.setX(paintEnd.rx());
    }
    if (widgetStart.ry() > widgetEnd.ry())
    {
        startPos.setY(paintEnd.ry());
        endPos.setY(paintStart.ry());
    }
    else
    {
        startPos.setY(paintStart.ry());
        endPos.setY(paintEnd.ry());
    }

    sizePoint.i_point[0]->setX(startPos.rx());
    sizePoint.i_point[0]->setY(startPos.ry());
    sizePoint.i_point[0]->setRotate(DIRECTION::STARTPOINT);
    sizePoint.i_point[1]->setX(endPos.rx());
    sizePoint.i_point[1]->setY(endPos.ry());
    sizePoint.i_point[1]->setRotate(DIRECTION::ENDPOINT);
}

void Line::specialWidgetUpdate(int &x, int &y, int &w, int &h)
{
    x -= extendWidth;
    y -= extendWidth;
    w += extendWidth << 1;
    h += extendWidth << 1;
}

void Line::specialPaintUpdate(QPoint &s, QPoint &e)
{
    s.setX(s.rx() + extendWidth);
    s.setY(s.ry() + extendWidth);
    e.setX(e.rx() + extendWidth);
    e.setY(e.ry() + extendWidth);
}
