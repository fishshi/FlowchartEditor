#include "label.h"


void Label::mousePressEvent(QMouseEvent *event)
{
    setFrameShape(QFrame::Panel);
    adjustSize();
    emit setTypeChangeTextPos(CHART_LABEL_MOUSE_TYPE::CHANGE_POS,event->pos().rx(),event->pos().ry());
}

void Label::mouseReleaseEvent(QMouseEvent *event)
{
    setFrameShape(QFrame::NoFrame);
    event->ignore();
}
