#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QFileDialog>
#include "../mainwindow.h"

#include "model/config.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller(MainWindow *w);
    ~Controller();

private:
    MainWindow *w;
    void initConnections();

public slots:
    void sendSelChartLineColor();
    void sendSelChartFillColor();
    void showRrightClickMenu(const QPoint &pos);
};

#endif // CONTROLLER_H
