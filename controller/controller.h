#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QMenu>

#include "../mainwindow.h"
#include "../ui_mainwindow.h"

#include "../view/canvas.h"

#include "Drawer.h"
#include "updater.h"
#include "Remover.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller(MainWindow *w);

public slots:
    void setTypeChangeSize(ORIENTION i){mouseEventType = MOUSE_EVENT_TYPE::CHANGE_SIZE; updater->sizePointDirect = i; }                                                                                                       // 设置鼠标事件类型为改变大小
    void setTypeCreateMagPoint(FlowchartElement *cb,ORIENTION d,int i) // 设置鼠标事件类型为创建线段
    {
        drawer->newLineFromSelectChart = cb;
        mouseEventType = MOUSE_EVENT_TYPE::RUNTIME_CREATE_MAGPOINT;
        drawer->magPointDirect = d;
        drawer->magPointFromIndex = i;
        drawer->newLineChart = nullptr;
    }

private:
    void initConnections();

    MainWindow *w;
    Canvas *canvas;
    Drawer *drawer;
    Updater *updater;
    Remover *remover;

    MOUSE_EVENT_TYPE mouseEventType = MOUSE_EVENT_TYPE::NONE;   // 当前鼠标事件类型

    void showRrightClickMenu(const QPoint &pos);
    void on_escPressed();
    void on_delPressed();

    void on_leftPressed(QMouseEvent *event);
    void on_leftClickToSelect(FlowchartElement * cb, int x, int y);

    void on_mouseMoved(QMouseEvent *event);
    void on_moveToCreate(int x, int y);
    void on_moveToChangeSize(int x, int y);
    void on_moveToLink(int x, int y);
    void on_moveToChangePos(int x, int y);

    void on_mouseReleased(QMouseEvent *event);
    void on_doneCreate();
    void on_doneChangeSize();
    void on_doneLink();
};

#endif // CONTROLLER_H
