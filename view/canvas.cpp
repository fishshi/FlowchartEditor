#include "canvas.h"

Canvas::Canvas(QWidget *parent, Qt::WindowFlags f) : QWidget(parent,f)
{
    this->grabKeyboard();
    setContextMenuPolicy(Qt::CustomContextMenu);
    setMouseTracking(true);
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter p = QPainter(this);
    p.setPen(QColor(150,150,150));
    const int gap = 30;
    for(int i = 0;i * gap < width(); ++i)
        for(int j = 0; j * gap < height(); ++j)
            p.drawPoint(i * gap, j * gap);
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
