#include "label.h"

void Label::changeText()
{
}
void Label::createText()
{
}
void Label::mousePressEvent(QMouseEvent *event)
{
    setFrameShape(QFrame::Panel);
    adjustSize();
    emit setTypeChangeTextPos(CHART_LABEL_MOUSE_TYPE::CHANGE_POS,event->pos().rx(),event->pos().ry());
    //event->ignore();
}
void Label::mouseDoubleClickEvent(QMouseEvent *event)
{
    changeText();
    event->ignore();
}
void Label::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
}
void Label::mouseReleaseEvent(QMouseEvent *event)
{
    setFrameShape(QFrame::NoFrame);
    event->ignore();
}
