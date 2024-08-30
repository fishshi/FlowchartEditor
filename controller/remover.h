#ifndef REMOVER_H
#define REMOVER_H
#include "../view/canvas.h"

class Remover
{
public:
    Remover(Canvas *canvas);

    void delChart(FlowchartElement *&cb);      // 删除图形
    void delLine(FlowchartElement *&cb);       // 删除线
private:
    Canvas *canvas;
};

#endif // REMOVER_H
