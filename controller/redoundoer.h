#ifndef REDOUNDOER_H
#define REDOUNDOER_H

#include<QStack>

#include "view/canvas.h"

class RedoUndoer
{
public:
    RedoUndoer(Canvas *canvas);

    QStack<QString> done;

    void undo();
    void redo();

private:
    Canvas *canvas;
};

#endif // REDOUNDOER_H
