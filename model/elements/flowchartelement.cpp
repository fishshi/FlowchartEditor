#include "flowchartelement.h"
int FlowchartElement::magPointWidth = 7;
int FlowchartElement::sizePointWidth = 9;
int FlowchartElement::pointLineWidth = 2;
int FlowchartElement::chartIDCount = 0;

QPen FlowchartElement::paintDrawPen  = QPen(QColor(150,80,80),pointLineWidth);
QBrush FlowchartElement::paintFillPen  = QBrush(QColor(255,255,255));

FlowchartElement::FlowchartElement(QWidget *parent, PaintChartType type, bool mov, int mpc, int spc):QWidget(parent),magPoint(mpc),sizePoint(spc),movable(mov),chartText()
{
    chartType = type;
    widgetPosInit();
    varInit();
    paintInit();
    pointInit();
    colorInit();
    textInit();
    setMouseTracking(true);
}

FlowchartElement::FlowchartElement( int x, int y, int w, int h, QWidget *parent, PaintChartType type):QWidget(parent),magPoint(4),sizePoint(4)
{
    chartType = type;
    widgetPosInit(x,y,w,h);
    varInit();
    paintInit();
    pointInit();
    colorInit();
    textInit();
    setMouseTracking(true);
}

FlowchartElement::FlowchartElement(FlowchartElement &cb)
{
    chartType = cb.chartType;
    paintStart = cb.paintStart;
    paintEnd = cb.paintEnd;
    magPoint = cb.magPoint;
    sizePoint = cb.sizePoint;

    varInit();
    paintInit();
    pointInit();
    colorInit();
    textInit();
    setMouseTracking(true);
}

void FlowchartElement::widgetPosInit(int x,int y,int w,int h)
{
    widgetStart.setX(x);
    widgetStart.setY(y);
    widgetEnd.setX(x+w);
    widgetEnd.setY(y+h);
    updateWidgetPosInof();
}

void FlowchartElement::varInit(int mpw,int spw,int plw,bool sa,bool smo)
{
    qDebug()<<chartIDCount;
    ID = chartIDCount++;
    FlowchartElement::magPointWidth = mpw;
    FlowchartElement::sizePointWidth = spw;
    FlowchartElement::pointLineWidth = plw;
    showAll = sa;
    showMag = smo;
}

void FlowchartElement::paintInit()
{
    updatePaintInfo();
}

void FlowchartElement::pointInit()
{
    for(auto it = magPoint.i_point.begin();it != magPoint.i_point.end(); it++)
    {
        (*it) = new i_magpointbase;
        (*it)->i_pos = new QPoint;
    }
    for(auto it = sizePoint.i_point.begin();it != sizePoint.i_point.end(); it++)
    {
        (*it) = new i_pointbase;
        (*it)->i_pos = new QPoint;
    }

    updateSizePointInfo();
    updateSizePointPath();
    updateMagPointInfo();
    updateMagPointPath();
}

void FlowchartElement::textInit()
{
    chartText.text = new Label(this);
    chartText.text->setText("双击以输入");
    chartText.text->setWordWrap(true);
    chartText.text->move(paintStart.rx() + sizePointWidth,paintStart.ry() + sizePointWidth);
    chartText.text->adjustSize();
    connect(chartText.text,SIGNAL(setTypeChangeTextPos(CHART_LABEL_MOUSE_TYPE,int,int)),this,SLOT(setTypeChangeTextPos(CHART_LABEL_MOUSE_TYPE,int,int)));
}

void FlowchartElement::colorInit()
{
    paintChartDrawPen  = QPen(QColor(0,0,0),2);
    paintChartFillPen  = QBrush(QColor(255,255,255));
}

void FlowchartElement::updateWidgetPosInof()
{
    int w, h;
    int x, y;
    if(widgetEnd.rx() < widgetStart.rx())
    {
        x = widgetEnd.rx() - borderWidth;
        w = widgetStart.rx() - widgetEnd.rx() + borderWidth + borderWidth;
    }
    else
    {
        x = widgetStart.rx() - borderWidth;
        w = widgetEnd.rx() - widgetStart.rx()  + borderWidth + borderWidth;
    }
    if(widgetEnd.ry() < widgetStart.ry())
    {
        y = widgetEnd.ry() - borderWidth;
        h = widgetStart.ry() - widgetEnd.ry()  + borderWidth + borderWidth;
    }
    else
    {
        y = widgetStart.ry() - borderWidth;
        h = widgetEnd.ry() - widgetStart.ry()  + borderWidth + borderWidth;
    }
    specialWidgetUpdate(x,y,w,h);
    setGeometry(x,y,w,h);
}
void FlowchartElement::updatePaintInfo()
{
    paintStart.setX(borderWidth);
    paintStart.setY(borderWidth);
    paintEnd.setX(borderWidth + abs(widgetEnd.rx() - widgetStart.rx()));
    paintEnd.setY(borderWidth + abs(widgetEnd.ry() - widgetStart.ry()));
    specialPaintUpdate(paintStart,paintEnd);
}

void FlowchartElement::updateSizePointInfo()
{
    if(sizePoint.i_point.size()>=4)
    {
        int x1 = paintStart.rx(),y1 = paintStart.ry();
        int x2 = paintEnd.rx(),y2 = paintEnd.ry();
        sizePoint.i_point[0]->setX(x1);
        sizePoint.i_point[0]->setY(y1);
        sizePoint.i_point[0]->setRotate(ORIENTION::NORTHWEST);
        sizePoint.i_point[1]->setX(x2);
        sizePoint.i_point[1]->setY(y1);
        sizePoint.i_point[1]->setRotate(ORIENTION::NORTHEAST);
        sizePoint.i_point[2]->setX(x2);
        sizePoint.i_point[2]->setY(y2);
        sizePoint.i_point[2]->setRotate(ORIENTION::SOUTHEAST);
        sizePoint.i_point[3]->setX(x1);
        sizePoint.i_point[3]->setY(y2);
        sizePoint.i_point[3]->setRotate(ORIENTION::SOUTHWEST);
    }
}

void FlowchartElement::updateMagPointInfo()
{
    if(magPoint.i_point.size()==4)
    {
        int x1 = paintStart.rx(),y1 = paintStart.ry();
        int x2 = paintEnd.rx(),y2 = paintEnd.ry();
        int midx = ((paintStart.rx() + paintEnd.rx())>>1);
        int midy = ((paintStart.ry() + paintEnd.ry())>>1);

        magPoint.i_point[0]->setX(midx);
        magPoint.i_point[0]->setY(y1);
        magPoint.i_point[0]->setRotate(ORIENTION::NORTH);
        magPoint.i_point[1]->setX(x2);
        magPoint.i_point[1]->setY(midy);
        magPoint.i_point[1]->setRotate(ORIENTION::EAST);
        magPoint.i_point[2]->setX(midx);
        magPoint.i_point[2]->setY(y2);
        magPoint.i_point[2]->setRotate(ORIENTION::SOUTH);
        magPoint.i_point[3]->setX(x1);
        magPoint.i_point[3]->setY(midy);
        magPoint.i_point[3]->setRotate(ORIENTION::WEST);
    }
}

void FlowchartElement::adjustPointInfo()
{
    for(auto it = magPoint.i_point.begin();it!=magPoint.i_point.end();it++)
    {
        switch((*it)->getRotate())
        {
            default:case ORIENTION::NONE:case ORIENTION::NORTHWEST:break;
            case ORIENTION::NORTH:(*it)->setX((*it)->getX() + pointLineWidth / 2);break;
            case ORIENTION::NORTHEAST:(*it)->setX((*it)->getX() + pointLineWidth);break;
            case ORIENTION::EAST:(*it)->setX((*it)->getX() + pointLineWidth);(*it)->setY((*it)->getY() + pointLineWidth / 2);break;
            case ORIENTION::SOUTHEAST:(*it)->setX((*it)->getX() + pointLineWidth);(*it)->setY((*it)->getY() + pointLineWidth);break;
            case ORIENTION::SOUTH:(*it)->setX((*it)->getX() + pointLineWidth / 2);(*it)->setY((*it)->getY() + pointLineWidth);break;
            case ORIENTION::SOUTHWEST:(*it)->setY((*it)->getY() + pointLineWidth);break;
            case ORIENTION::WEST:(*it)->setY((*it)->getY() + pointLineWidth / 2);break;
        }
    }
    for(auto it = sizePoint.i_point.begin();it!=sizePoint.i_point.end();it++)
    {
        switch((*it)->getRotate())
        {
            default:case ORIENTION::NONE:case ORIENTION::NORTHWEST:break;
            case ORIENTION::NORTH:(*it)->setX((*it)->getX() + pointLineWidth / 2);break;
            case ORIENTION::NORTHEAST:(*it)->setX((*it)->getX() + pointLineWidth);break;
            case ORIENTION::EAST:(*it)->setX((*it)->getX() + pointLineWidth);(*it)->setY((*it)->getY() + pointLineWidth / 2);break;
            case ORIENTION::SOUTHEAST:(*it)->setX((*it)->getX() + pointLineWidth);(*it)->setY((*it)->getY() + pointLineWidth);break;
            case ORIENTION::SOUTH:(*it)->setX((*it)->getX() + pointLineWidth / 2);(*it)->setY((*it)->getY() + pointLineWidth);break;
            case ORIENTION::SOUTHWEST:(*it)->setY((*it)->getY() + pointLineWidth);break;
            case ORIENTION::WEST:(*it)->setY((*it)->getY() + pointLineWidth / 2);break;
        }
    }
}
void FlowchartElement::updateMagPointPath()
{
    for(auto it = magPoint.i_point.begin(),end = magPoint.i_point.end();it != end;it++)
    {
        (*it)->newPath()->addEllipse((*it)->getX() - sizePointWidth / 2,(*it)->getY() - sizePointWidth / 2,sizePointWidth,sizePointWidth);
    }
}

void FlowchartElement::updateSizePointPath()
{
    for(auto it = sizePoint.i_point.begin(),end = sizePoint.i_point.end();it != end;it++)
    {
        (*it)->newPath()->addRect((*it)->getX() - sizePointWidth / 2,(*it)->getY() - sizePointWidth / 2,sizePointWidth,sizePointWidth);
    }
}
void FlowchartElement::updateMagPointLine()
{
    for(auto it = magPoint.i_point.begin(),end = magPoint.i_point.end();it != end;it++)
    {
        for(auto l_it = (*it)->i_lineStart.begin(),l_end = (*it)->i_lineStart.end();l_it != l_end;l_it++)
        {
            if( (*l_it)->widgetStart.rx() != (*it)->getX() + x() || (*l_it)->widgetStart.ry() != (*it)->getY() + y())
            {
                (*l_it)->setStartPos((*it)->getX() + x(),(*it)->getY() + y());

                (*l_it)->updateWidgetPosInof();
                (*l_it)->updatePaintInfo();
                (*l_it)->updateSizePointInfo();
                (*l_it)->updateSizePointPath();
                (*l_it)->updateTextInfo();
            }
        }
        for(auto l_it = (*it)->i_lineEnd.begin(),l_end = (*it)->i_lineEnd.end();l_it != l_end;l_it++)
        {
            if( (*l_it)->widgetEnd.rx() != (*it)->getX() + x() || (*l_it)->widgetEnd.ry() != (*it)->getY() + y())
            {
                (*l_it)->setEndPos((*it)->getX() + x(),(*it)->getY() + y());

                (*l_it)->updateWidgetPosInof();
                (*l_it)->updatePaintInfo();
                (*l_it)->updateSizePointInfo();
                (*l_it)->updateSizePointPath();
                (*l_it)->updateTextInfo();
            }
        }
    }
}
void FlowchartElement::updateTextInfo()
{
    int w = paintEnd.rx() - paintStart.rx();
    w = w > 0 ? w : 0;

    chartText.text->setMaximumWidth(w);
    chartText.text->setMaximumHeight(w);
    chartText.text->adjustSize();
    if(chartText.text->x() + chartText.text->width() >paintEnd.rx())
    {
        if(paintEnd.rx() - chartText.text->width()<paintStart.rx())
            chartText.text->move(paintStart.rx(),chartText.text->y());
        else
            chartText.text->move(paintEnd.rx() - chartText.text->width(),chartText.text->y());
    }
    if(chartText.text->y() + chartText.text->height() >paintEnd.ry())
    {
        if(paintEnd.ry() - chartText.text->height()<paintStart.ry())
            chartText.text->move(chartText.text->x(),paintStart.ry());
        else
            chartText.text->move(chartText.text->x(),paintEnd.ry() - chartText.text->height());
    }
}

void FlowchartElement::paintMagPoint(QPainter & p)
{
    QPen tmp = p.pen();
    p.setPen(paintDrawPen);
    QPainterPath* tmpPath;
    for(auto it = magPoint.i_point.begin(),end = magPoint.i_point.end(); it != end;it++)
    {
        tmpPath = (*it)->getPath();
        p.drawPath(*tmpPath);
        p.fillPath(*tmpPath,paintFillPen);
    }
    p.setPen(tmp);
}

void FlowchartElement::paintSizePoint(QPainter & p)
{
    QPen tmp = p.pen();
    p.setPen(paintDrawPen);

    QPainterPath* tmpPath;

    for(auto it = sizePoint.i_point.begin(),end = sizePoint.i_point.end(); it != end;it++)
    {
        tmpPath = (*it)->getPath();
        p.drawPath(*tmpPath);
        p.fillPath(*tmpPath,paintFillPen);
    }
    p.setPen(tmp);
}

void FlowchartElement::paintSizeEdge(QPainter & p)
{
    QPen tmp = p.pen();
    p.setPen(paintDrawPen);

    int sx = paintStart.rx(),sy = paintStart.ry(),ex = paintEnd.rx(), ey = paintEnd.ry();
    int sx2 = sx+sizePointWidth / 2, sy2 = sy+sizePointWidth / 2;
    int ex1 = ex-sizePointWidth / 2, ey1 = ey-sizePointWidth / 2;

    p.drawLine(sx2,sy,ex1,sy);
    p.drawLine(ex,sy2,ex,ey1);
    p.drawLine(ex1,ey,sx2,ey);
    p.drawLine(sx,ey1,sx,sy2);

    p.setPen(tmp);
}

bool FlowchartElement::inPath(const QPointF &p)
{
    for(auto it = magPoint.i_point.begin(),end = magPoint.i_point.end();it!=end;it++)
    {
        if((*it)->inPath(p))return true;
    }
    for(auto it = sizePoint.i_point.begin(),end = sizePoint.i_point.end();it!=end;it++)
    {
        if((*it)->inPath(p))return true;
    }
    return inGraphisPath(p);
}
bool FlowchartElement::inMagPath(const QPointF &p, ORIENTION &b, int &index) const
{
    int i = 0;
    for(auto it = magPoint.i_point.begin(),end = magPoint.i_point.end();it!=end;it++,i++)
    {
        if((*it)->inPath(p))
        {
            b = (*it)->getRotate();
            index = i;
            return true;
        }
    }
    return false;
}
bool FlowchartElement::inSizePath(const QPointF &p, ORIENTION &b) const
{
    int x, y;
    for(auto it = sizePoint.i_point.begin(),end = sizePoint.i_point.end();it!=end;it++)
    {
        if((*it)->inPath(p))
        {
            b = (*it)->getRotate();
            return true;
        }
    }
    return false;
}
void FlowchartElement::setStartPos(int x,int y)
{
    widgetStart.setX(x);
    widgetStart.setY(y);
}
void FlowchartElement::setEndPos(int x,int y)
{
    widgetEnd.setX(x);
    widgetEnd.setY(y);
}

void FlowchartElement::setXY(int x, int y)
{
    int *x1,*y1;
    int *x2,*y2;
    if(widgetEnd.rx()<widgetStart.rx())
    {
        x1 = &widgetEnd.rx();
        x2 = &widgetStart.rx();
    }else{
        x1 = &widgetStart.rx();
        x2 = &widgetEnd.rx();
    }
    if(widgetEnd.ry()<widgetStart.ry())
    {
        y1 = &widgetEnd.ry();
        y2 = &widgetStart.ry();
    }else{
        y1 = &widgetStart.ry();
        y2 = &widgetEnd.ry();
    }
    *x2 = x + *x2 - *x1;
    *y2 = y + *y2 - *y1;
    *x1 = x;
    *y1 = y;

    updateWidgetPosInof();
    updateMagPointLine();
}

void FlowchartElement::setWidthHeight(int x, int y)
{
    widgetEnd.setX(x);
    widgetEnd.setY(y);

    updateWidgetPosInof();
    updatePaintInfo();
    updateSizePointInfo();
    updateSizePointPath();
    updateMagPointInfo();
    updateMagPointPath();
    updateMagPointLine();
    updateTextInfo();
}
void FlowchartElement::setWidthHeight(int x, int y, ORIENTION type)
{
    int *x1,*y1;
    int *x2,*y2;
    if(widgetEnd.rx()<widgetStart.rx())
    {
        x1 = &widgetEnd.rx();
        x2 = &widgetStart.rx();
    }else{
        x1 = &widgetStart.rx();
        x2 = &widgetEnd.rx();
    }
    if(widgetEnd.ry()<widgetStart.ry())
    {
        y1 = &widgetEnd.ry();
        y2 = &widgetStart.ry();
    }else{
        y1 = &widgetStart.ry();
        y2 = &widgetEnd.ry();
    }
    switch(type)
    {
        case ORIENTION::NORTHWEST:
        {
            if(*x2-x<minSizeW) *x1 = *x2-minSizeW;
            else *x1 = x;
            if(*y2-y<minSizeH) *y1 = *y2-minSizeH;
            else *y1 = y;
            //widgetPos = widgetStart;
        }break;
        case ORIENTION::NORTHEAST:
        {
            if(x-*x1<minSizeW) *x2 = *x1+minSizeW;
            else *x2 = x;
            if(*y2-y<minSizeH) *y1 = *y2-minSizeH;
            else *y1 = y;
            //widgetPos = widgetStart;
        }break;
        case ORIENTION::SOUTHEAST:
        {
            if(x-*x1<minSizeW) *x2 = *x1+minSizeW;
            else *x2 = x;
            if(y-*y1<minSizeH) *y2 = *y1+minSizeH;
            else *y2 = y;
            //widgetPos = widgetStart;
        }break;
        case ORIENTION::SOUTHWEST:
        {
            if(*x2-x<minSizeW) *x1 = *x2-minSizeW;
            else *x1 = x;
            if(y-*y1<minSizeH) *y2 = *y1+minSizeH;
            else *y2 = y;
            //widgetPos = widgetStart;
        }break;
        case ORIENTION::STARTPOINT:{
            if(x<widgetEnd.rx())
                widgetStart.setX(x );
            else
                widgetStart.setX(x );
            if(y<widgetEnd.ry())
                widgetStart.setY(y );
            else
                widgetStart.setY(y );
        }break;
        case ORIENTION::ENDPOINT:{
            if(x<widgetStart.rx())
                widgetEnd.setX(x );
            else
                widgetEnd.setX(x );
            if(y<widgetStart.ry())
                widgetEnd.setY(y );
            else
                widgetEnd.setY(y );
        }break;
        default:{
        }
    }

    updateWidgetPosInof();
    updatePaintInfo();
    updateSizePointInfo();
    updateSizePointPath();
    updateMagPointInfo();
    updateMagPointPath();
    updateMagPointLine();
    updateTextInfo();
}
void FlowchartElement::applyWidthHeight()
{
    updateWidgetPosInof();
    updatePaintInfo();
    updateSizePointInfo();
    updateSizePointPath();
    updateMagPointInfo();
    updateMagPointPath();
    updateMagPointLine();
    updateTextInfo();
}

bool FlowchartElement::autoSetMagi(int &abcx,int &abcy, int &index)
{
    int mx = abcx;
    int my = abcy;
    if(mx >= x() && my >= y() && mx-x()<=width() && my-y()<=width())
    {
        showMagOnly();
        int x1,y1;
        mx -= x();
        my -= y();
        int i = 0;
        for(auto it = magPoint.i_point.begin(),end = magPoint.i_point.end();it != end;it++,i++)
        {
            x1 = (*it)->getX() +  magPointWidth / 2;
            y1 = (*it)->getY() +  magPointWidth / 2;
            if(abs(x1-mx) <= magPointAutoMagiRange && abs(y1-my) <= magPointAutoMagiRange)
            {
                abcx = x() + (*it)->getX();
                abcy = y() + (*it)->getY();
                index = i;
                return true;
            }
        }
    }
    else
    {
        hideMagOnly();
    }

    return false;
}
void FlowchartElement::overlapChartMousePressed(QMouseEvent *event)
{
    int mx = event->pos().rx();
    int my = event->pos().ry();
    if(mx >= x() && my >= y() && mx-x()<=width() && my-y()<=width())
    {
        QPointF tmp(mx-this->x(),my-this->y());
        ORIENTION direct = ORIENTION::NONE;

        if(inGraphisPath(tmp))
        {
            emit sendThisClass(this,tmp.rx()-borderWidth,tmp.ry()-borderWidth);
            curFlag = MOUSE_EVENT_TYPE::RUNTIME_CHANGE_POS;
            event->accept();
            raise();
        }
    }
}
void FlowchartElement::overlapChartMouseMove(QMouseEvent *event)
{
    int mx = event->pos().rx();
    int my = event->pos().ry();

    if( mx >= x() && my >= y() && mx-x()<=width() && my-y()<=height())
    {
        ORIENTION direct = ORIENTION::NONE;
        int index;
        QPointF tmp(mx-this->x(),my-this->y());
        if(showMag)
        {
            if(inMagPath(tmp,direct,index))
            {
                setCursor(QCursor(Qt::SizeAllCursor));
                event->accept();
                raise();
            }
            else if(inGraphisPath(tmp))
            {
                setCursor(QCursor(Qt::SizeAllCursor));
                event->accept();
                raise();
            }
            else
            {
                hideMagOnly();
                setCursor(QCursor(Qt::ArrowCursor));
            }
        }
        else
        {
            if(inGraphisPath(tmp))
            {
                setCursor(QCursor(Qt::SizeAllCursor));
                event->accept();
                raise();
            }
            else
            {
                setCursor(QCursor(Qt::ArrowCursor));
            }
        }
    }
}

void FlowchartElement::showMagSize()
{
    showAll = true;
    showMag = false;
    update();
}
void FlowchartElement::hideMagSize()
{
    if(showAll)
    {
        this->releaseKeyboard();
        showAll = false;

        if(chartText.tmpEdit1)
        {
            chartText.text->setText(chartText.tmpEdit1->text());
            chartText.text->adjustSize();
            delete chartText.tmpEdit1;
            chartText.tmpEdit1 = nullptr;
        }
        update();
    }
}

void FlowchartElement::showMagOnly()
{
    if(showMag == false)
    {
        showMag = true;
        showAll = false;
        update();
    }
}
void FlowchartElement::hideMagOnly()
{
    if(showMag)
    {
        this->releaseKeyboard();
        showMag = false;
        if(chartText.tmpEdit1)
        {
            chartText.text->setText(chartText.tmpEdit1->text());
            chartText.text->adjustSize();
            delete chartText.tmpEdit1;
            chartText.tmpEdit1 = nullptr;
        }
        update();
    }
}

void FlowchartElement::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    paintChart(p);
    if(showAll)
    {
        paintSizeEdge(p);
        paintMagPoint(p);
        paintSizePoint(p);
    }else if(showMag)
    {
        paintMagPoint(p);
    }
    event->accept();
}

void FlowchartElement::mousePressEvent(QMouseEvent *event)
{
    ORIENTION direct = ORIENTION::NONE;
    int index;

    if(showAll&&inSizePath(event->pos(),direct))
    {
        emit setTypeChangeSize(direct);
        curFlag = MOUSE_EVENT_TYPE::CHANGE_SIZE;
        curIndex = direct;
        raise();

    }
    else if((showAll||showMag)&&inMagPath(event->pos(),direct,index))
    {
        emit setTypeCreateMagPoint(this,direct,index);
        curFlag = MOUSE_EVENT_TYPE::RUNTIME_CREATE_MAGPOINT;
        curIndex = direct;
        showMagOnly();
        raise();
    }
    else if(inGraphisPath(event->pos()) )
    {
        emit sendThisClass(this,event->pos().rx()-borderWidth,event->pos().ry()-borderWidth);
        curFlag = MOUSE_EVENT_TYPE::RUNTIME_CHANGE_POS;
        curIndex = direct;
        raise();
    }
    else
    {
        curFlag = MOUSE_EVENT_TYPE::NONE;
        showAll = false;
        event->ignore();
    }
}

void FlowchartElement::mouseMoveEvent(QMouseEvent *event)
{
    if(curFlag == MOUSE_EVENT_TYPE::NONE)
    {
        ORIENTION direct = ORIENTION::NONE;
        int index;

        if(showAll)
        {
            if(inSizePath(event->pos(),direct))
            {
                switch(ORIENTION(direct))
                {
                    case ORIENTION::NORTHWEST:case ORIENTION::SOUTHEAST:{
                        setCursor(QCursor(Qt::SizeFDiagCursor));
                    }break;
                    case ORIENTION::NORTHEAST:case ORIENTION::SOUTHWEST:{
                        setCursor(QCursor(Qt::SizeBDiagCursor));
                    }break;
                    case ORIENTION::STARTPOINT:case ORIENTION::ENDPOINT:{
                        setCursor(QCursor(Qt::SizeAllCursor));
                    }break;
                    default:
                        break;
                }
                raise();
            }
            else if(inMagPath(event->pos(),direct,index))
            {
                setCursor(QCursor(Qt::CrossCursor));
                raise();
            }
            else if(inGraphisPath(event->pos()))
            {
                setCursor(QCursor(Qt::SizeAllCursor));
                raise();
            }
            else
            {
                event->ignore();
            }
        }
        else if(showMag)
        {
            if(inMagPath(event->pos(),direct,index))
            {
                setCursor(QCursor(Qt::CrossCursor));
                raise();
            }
            else if(inGraphisPath(event->pos()))
            {
                setCursor(QCursor(Qt::SizeAllCursor));
                raise();
            }
            else
            {
                hideMagOnly();
                event->ignore();
            }
        }
        else
        {
            if(inGraphisPath(event->pos()))
            {
                setCursor(QCursor(Qt::SizeAllCursor));
                showMagOnly();
                raise();
            }
            else
            {
                event->ignore();
            }
        }
        if(chartText.textMouseT1 == CHART_LABEL_MOUSE_TYPE::CHANGE_POS)
        {
            emit sendThisClass(this,event->pos().rx()-borderWidth,event->pos().ry()-borderWidth);

            chartText.text->move(event->pos().rx()-chartText.chartTextMousePos.rx(),event->pos().ry()-chartText.chartTextMousePos.ry());
            chartText.text->adjustSize();
        }
    }
    else
    {
        event->ignore();
    }
}

void FlowchartElement::mouseReleaseEvent(QMouseEvent *event)
{
    chartText.textMouseT1 = CHART_LABEL_MOUSE_TYPE::NONE;

    curFlag = MOUSE_EVENT_TYPE::NONE;

    event->ignore();
}

void FlowchartElement::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(inPath(event->pos()))
    {
        emit sendThisClass(this,event->pos().rx()-borderWidth,event->pos().ry()-borderWidth);

        chartText.tmpEdit1 = new QLineEdit(chartText.text->text(),this);
        chartText.text->setText("");
        chartText.tmpEdit1->adjustSize();
        chartText.tmpEdit1->setStyleSheet("background:transparent;");
        chartText.tmpEdit1->setGeometry(chartText.text->x(),chartText.text->y(),chartText.text->width() + (textBorderWidth<<1),chartText.text->height() + (textBorderWidth<<1));
        chartText.tmpEdit1->show();
        chartText.tmpEdit1->setFocus();

        this->grabKeyboard();
    }else
    {
        event->ignore();
    }
}
