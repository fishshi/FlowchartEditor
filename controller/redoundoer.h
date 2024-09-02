#ifndef REDOUNDOER_H
#define REDOUNDOER_H

#include "view/canvas.h"

class RedoUndoer
{
public:
    RedoUndoer(Canvas *canvas);

private:
    Canvas *canvas;
};

#endif // REDOUNDOER_H
