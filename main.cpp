#include "mainwindow.h"
#include <QApplication>
#include "./controller/controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Controller c(&w);
    // 设置窗口图标
    QIcon icon("://assets/ico.ico");
    w.setWindowIcon(icon);
    w.show();
    return a.exec();
}
