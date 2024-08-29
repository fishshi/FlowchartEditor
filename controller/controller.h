#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QFileDialog>
#include "../mainwindow.h"
#include "Drawer.h"
#include "Updater.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller(MainWindow *w);

    void sendSelChartLineColor();
    void sendSelChartFillColor();
    void showRrightClickMenu(const QPoint &pos);

private:
    void initConnections();

    MainWindow *w;
    Drawer *drawer;
    Updater updater;

    MOUSE_EVENT_TYPE mouseEventType = MOUSE_EVENT_TYPE::NONE;   // 当前鼠标事件类型
};

#endif // CONTROLLER_H
