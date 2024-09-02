#ifndef FILER_H
#define FILER_H

#include "../view/canvas.h"

class Filer
{
public:
    Filer(Canvas *canvas);
    void openFile();
    void saveFile();

private:
    Canvas *canvas;
};

#endif // FILER_H
