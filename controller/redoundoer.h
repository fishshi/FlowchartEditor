#ifndef REDOUNDOER_H
#define REDOUNDOER_H

#include <QFile>
#include <QDir>

#include "view/canvas.h"

class RedoUndoer
{
    friend class Controller;
public:
    RedoUndoer(Canvas *canvas);

    void undo();
    void redo();

private:
    Canvas *canvas;
    int reNo = -1;
    int unNo = -1;
};

#endif // REDOUNDOER_H
