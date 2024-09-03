#ifndef REMOVER_H
#define REMOVER_H
#include "../view/canvas.h"

class Remover
{
public:
    Remover(Canvas *canvas);

    void delChart(FlowchartElement *&fce);      // 删除图形
    void delLine(FlowchartElement *&fce);       // 删除线
    void clear();                              //清屏
    void clearCacheRe();
    void clearCacheUn();
private:
    Canvas *canvas;
};

#endif // REMOVER_H
