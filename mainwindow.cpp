#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionkaiyuanxieyi, &QAction::triggered, [=](){
        QProcess::startDetached("notepad.exe", QStringList() << "../../LICENSE");
    });
    connect(ui->actionReadMe, &QAction::triggered, [=](){
        QProcess::startDetached("notepad.exe", QStringList() << "../../README.md");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

