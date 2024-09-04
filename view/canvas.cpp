#include "canvas.h"

Canvas::Canvas(QWidget *parent, Qt::WindowFlags f) : QWidget(parent,f)
{
    this->grabKeyboard();
    setContextMenuPolicy(Qt::CustomContextMenu);
    setMouseTracking(true);
}

void Canvas::setDrawFrame(int x1, int y1, int x2, int y2)
{
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    isframe = true;
    this->update();
}

void Canvas::moveFrameBy(int x, int y)
{
    x1 += x;
    x2 += x;
    y1 += y;
    y2 += y;
    update();
}

void Canvas::unsetDrawFrame()
{
    isframe = false;
    this->update();
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter p = QPainter(this);

    // 绘制背景图片
    if (!backgroundImage.isNull()) {
        p.drawPixmap(0, 0, backgroundImage);
    }

    p.setPen(QColor(0,0,0));
    const int gap = 30;
    for(int i = 0;i * gap < width(); ++i)
        for(int j = 0; j * gap < height(); ++j)
            p.drawPoint(i * gap, j * gap);
    if(isframe)
    {
        p.setPen(QPen(Qt::DashLine));
        QRect rect(x1, y1, x2 - x1, y2 - y1);
        p.drawRect(rect.normalized());
    }
}

void Canvas::clearChartsLine(){
    for(auto it = charts.begin();it!=charts.end();it++) {
        if(*it) delete *it;
        *it = nullptr;
    }
    for(auto it = line.begin();it!=line.end();it++) {
        if(*it) delete *it;
        *it = nullptr;
    }
}

void Canvas::hideMagSizeAll()
{
    for(auto it = charts.begin();it!=charts.end();it++)
        (*it)->hideMagSize();
    for(auto it = line.begin();it!=line.end();it++)
        (*it)->hideMagSize();
    curSelecChart = nullptr;
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    if(event->button() == Qt::LeftButton)
        emit leftPressed(event);
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
    emit mouseMoved(event);
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseReleased(event);
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    event->ignore();
    if(event->key() == Qt::Key_Escape)
        emit escPressed();
    else if (event->key() == Qt::Key_Delete)
        emit delPressed();
}
