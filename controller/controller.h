#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QFileDialog>
#include "../mainwindow.h"
#include "Drawer.h"
#include "updater.h"
#include "Remover.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller(MainWindow *w);

private:
    void initConnections();

    MainWindow *w;
    Drawer *drawer;
    Updater *updater;
    Remover *remover;

    void showRrightClickMenu(const QPoint &pos);
    void on_escPressed();
    void on_delPressed();
};

#endif // CONTROLLER_H
