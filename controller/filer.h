#ifndef FILER_H
#define FILER_H

#include "../view/canvas.h"
#include <QFile>
#include <QSvgGenerator>
#include <QMessageBox>
#include <QString>

class Filer
{
public:
    Filer(Canvas *canvas);
    void openFile(QString filePath);
    void saveFile(QString filePath);
    void saveAsSVG(QString filePath);
    void saveAsPNG(QString filePath);
    void saveAsElse(QString filePath);

private:
    Canvas *canvas;
};

#endif // FILER_H
