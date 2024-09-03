#ifndef FILER_H
#define FILER_H

#include "../view/canvas.h"
#include <QFile>
#include <QSvgGenerator>

class Filer
{
public:
    Filer(Canvas *canvas);
    void openFile(QString filePath);
    void saveFile(QString filePath);
    void saveAsSVG(QString filePath);

private:
    Canvas *canvas;
};

#endif // FILER_H
