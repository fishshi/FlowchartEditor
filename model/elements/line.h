#ifndef LINE_H
#define LINE_H
#include "flowchartelement.h"
#include <QtMath>

class Line : public FlowchartElement
{
private:

    const static int containsWidth = 20;                    // 可选范围长度
    const static int containsWidth1_2 = containsWidth>>1;   // 可选范围一般长度
    const static int extendWidth = 30;              // 边界线段延伸长度
    const static int arrayLength = 18;              // 线头绘制范围 - 长度
    const static int arrayWidth = 9;                // 线头绘制范围 - 宽度

    virtual void paintChart(QPainter & p);
    virtual void paintSizeEdge(QPainter & p){}
    virtual void updateSizePointInfo();
    virtual void updateMagPointLine(){}
    virtual void specialWidgetUpdate(int &x,int &y, int &w, int &h);
    virtual void specialPaintUpdate(QPoint &s, QPoint &e);

    QPoint startPos;    // 开始坐标
    QPoint endPos;      // 结束坐标

    ORIENTION startDirect = ORIENTION::STARTPOINT;  // 开始点方向
    ORIENTION endDirect = ORIENTION::ENDPOINT;      // 结束点方向
    int startMagIndex = 0;                  // 起始点索引值
    int endMagIndex = 0;                    // 结束点索引值
    FlowchartElement *startChartMag = nullptr;    // 起始图形
    FlowchartElement *endChartMag = nullptr;      // 终止图形
    LINE_HEAD_TYPE startLineHeadType = LINE_HEAD_TYPE::ARROW0;   // 起始点线头方向
    LINE_HEAD_TYPE endLineHeadType = LINE_HEAD_TYPE::ARROW1;     // 结束点线头方向

    void drawLineHead(const ORIENTION o,const LINE_HEAD_TYPE lht,const int x,const int y,QPainter & p,QPainterPath &linePath, QPainterPath &graphPath); // 绘制线头
    void drawStraightLine(int sx, int sy, int ex, int ey,QPainterPath &linePath, QPainterPath &graphPath);          // 直连线绘制

public:
    Line(QWidget *parent = nullptr, PaintChartType type = PaintChartType::LINE) : FlowchartElement(parent,type,false,0,2){}

    Line(Line &cr):FlowchartElement(cr)
    {
        chartText.text->setAttribute(Qt::WA_StyledBackground,true);
        chartText.text->setAutoFillBackground(true);
        chartText.text->setPalette(QPalette(QPalette::Window, QColor(255,255,255,150)));
        chartText.text->setStyleSheet("QLabel{background:#00FF00;}");
    }

    Line(int x1, int y1, int x2, int y2, QWidget *parent = nullptr,PaintChartType type = PaintChartType::LINE) : FlowchartElement(x1,y1,x2,y2,parent,type)
    {
        chartText.text->setAttribute(Qt::WA_StyledBackground,true);
        chartText.text->setAutoFillBackground(true);
        chartText.text->setPalette(QPalette(QPalette::Window, QColor(255,255,255,150)));
        chartText.text->setStyleSheet("QLabel{background:#00FF00;}");
    }
    ~Line(){}

    void setStartChart(FlowchartElement* cb){startChartMag = cb;} // 设置起点指向的图形
    void resetStartChart()                                  // 重置起点指向的图形
    {
        if(startChartMag)
        {
            startChartMag->delMagiPointStartLine(startMagIndex,this);
        }
        startChartMag = nullptr;
    }
    FlowchartElement *getStartChart(){return startChartMag;}      // 获取起点指向的图形
    void setEndChart(FlowchartElement* cb){endChartMag = cb;}     // 设置终点指向的图形
    void resetEndChart()                                    // 重置终点指向的图形
    {
        if(endChartMag)
            endChartMag->delMagiPointEndLine(endMagIndex,this);
        endChartMag = nullptr;
    }
    FlowchartElement *getEndChart(){return endChartMag;}          // 获取终点指向的图形

    LINE_HEAD_TYPE & getStartLineHeadType(){return startLineHeadType;}              // 获取起点线头类型
    void setStartLineHeadType(LINE_HEAD_TYPE l){startLineHeadType = l;update();}    // 设置起点线头类型
    LINE_HEAD_TYPE & getEndLineHeadType(){return endLineHeadType;}                  // 获取终点线头类型
    void setEndLineHeadType(LINE_HEAD_TYPE l){endLineHeadType = l;update();}        // 设置终点线头类型

    void setStartMagIndex(int i){startMagIndex = i;}        // 设置起点指向的图形的磁力点索引
    int getStartMagIndex(){return startMagIndex;}           // 获取起点指向的图形的磁力点索引
    void setEndMagIndex(int i){endMagIndex = i;}            // 设置终点指向的图形的磁力点索引
    int getEndMagIndex(){return endMagIndex;}               // 获取终点指向的图形的磁力点索引

    void setStartDirect(ORIENTION direct){startDirect = direct;}    // 设置连线起点的朝向
    void resetStartDirect(){startDirect = ORIENTION::NONE;}         // 重置连线起点的朝向
    void setEndDirect(ORIENTION direct){endDirect = direct;}        // 设置连线终点的朝向
    void resetEndDirect(){endDirect = ORIENTION::NONE;}             // 重置连线终点的朝向

signals:
    void sendLineStyle(QPen &qp, QBrush &qb, LINE_HEAD_TYPE &startLineHeadType, LINE_HEAD_TYPE &endLineHeadType);   // 发送连线信息
};

#endif // LINE_H
