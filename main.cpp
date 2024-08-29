#include "mainwindow.h"
#include <QApplication>
#include "./controller/controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Controller c(&w);
    w.show();
    return a.exec();
}
