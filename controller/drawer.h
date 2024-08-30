#ifndef DRAWER_H
#define DRAWER_H

#include "../view/canvas.h"

class Drawer
{
public:
    Drawer(Canvas *canvas);

private:
    Canvas *canvas;
};

#endif // DRAWER_H
