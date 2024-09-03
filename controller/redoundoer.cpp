#include "redoundoer.h"
#include <QDebug>

RedoUndoer::RedoUndoer(Canvas *canvas) {
    this->canvas = canvas;
}

void RedoUndoer::undo()
{
    QFile::rename(QDir::current().filePath("../../assets/cashe/Redo/")+QString::number(reNo)+".fy",
                  QDir::current().filePath("../../assets/cashe/Undo/")+QString::number(unNo+1)+".fy");
    reNo-=1;
    unNo+=1;
}

void RedoUndoer::redo()
{
    QFile::rename(QDir::current().filePath("../../assets/cashe/Undo/")+QString::number(unNo)+".fy",
                  QDir::current().filePath("../../assets/cashe/Redo/")+QString::number(reNo+1)+".fy");
    reNo+=1;
    qDebug()<<669;
    unNo-=1;
    qDebug()<<6610;
}
