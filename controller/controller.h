#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QFileDialog>
#include "../mainwindow.h"
#include "Drawer.h"

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

    void on_escPressed();
    void on_delPressed();
};

#endif // CONTROLLER_H
