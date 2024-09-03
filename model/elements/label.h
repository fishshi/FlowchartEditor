#ifndef LABEL_H
#define LABEL_H
#include <QLabel>
#include <QWidget>
#include <QPlainTextEdit>
#include <QMouseEvent>
#include <qDebug>
#include "../config.h"

class Label : public QLabel
{
    Q_OBJECT

public:
    Label(QWidget *parent = nullptr):QLabel(parent){}
    virtual ~Label(){}

signals:
    void setTypeChangeTextPos(CHART_LABEL_MOUSE_TYPE t,int x,int y);    // 设置位置

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

};

#endif // LABEL_H
