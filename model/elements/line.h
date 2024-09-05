#ifndef LINE_H
#define LINE_H

#include "flowchartelement.h"

class Line : public FlowchartElement
{
    friend QDataStream &operator<<(QDataStream &fout, const Line &cl)
    {
        fout << cl.startPos << cl.endPos;
        fout.writeRawData(reinterpret_cast<const char *>(&cl.startDirect), sizeof(DIRECTION));
        fout.writeRawData(reinterpret_cast<const char *>(&cl.endDirect), sizeof(DIRECTION));
        fout.writeRawData(reinterpret_cast<const char *>(&cl.startMagIndex), sizeof(int));
        fout.writeRawData(reinterpret_cast<const char *>(&cl.endMagIndex), sizeof(int));
        fout.writeRawData(reinterpret_cast<const char *>(&cl.startLineHeadType), sizeof(LINE_HEAD_TYPE));
        fout.writeRawData(reinterpret_cast<const char *>(&cl.endLineHeadType), sizeof(LINE_HEAD_TYPE));
        if (cl.startChartMag)
            fout.writeRawData(reinterpret_cast<const char *>(&cl.startChartMag->getID()), sizeof(int));
        else
        {
            int i = -1;
            fout.writeRawData(reinterpret_cast<const char *>(&i), sizeof(int));
        }
        if (cl.endChartMag)
            fout.writeRawData(reinterpret_cast<const char *>(&cl.endChartMag->getID()), sizeof(int));
        else
        {
            int i = -1;
            fout.writeRawData(reinterpret_cast<const char *>(&i), sizeof(int));
        }
        return fout;
    }
    friend QDataStream &operator>>(QDataStream &fin, Line &cl)
    {
        fin >> cl.startPos >> cl.endPos;
        fin.readRawData(reinterpret_cast<char *>(&cl.startDirect), sizeof(DIRECTION));
        fin.readRawData(reinterpret_cast<char *>(&cl.endDirect), sizeof(DIRECTION));
        fin.readRawData(reinterpret_cast<char *>(&cl.startMagIndex), sizeof(int));
        fin.readRawData(reinterpret_cast<char *>(&cl.endMagIndex), sizeof(int));
        fin.readRawData(reinterpret_cast<char *>(&cl.startLineHeadType), sizeof(LINE_HEAD_TYPE));
        fin.readRawData(reinterpret_cast<char *>(&cl.endLineHeadType), sizeof(LINE_HEAD_TYPE));
        return fin;
    }

private:
    const static int containsWidth = 20;                    // 可选范围长度
    const static int containsWidth1_2 = containsWidth >> 1; // 可选范围一般长度
    const static int extendWidth = 20;                      // 边界线段延伸长度
    const static int arrayLength = 18;                      // 线头绘制范围 - 长度
    const static int arrayWidth = 9;                        // 线头绘制范围 - 宽度

    virtual void paintChart(QPainter &p);
    virtual void paintSizeEdge(QPainter &p) {}
    virtual void updateSizePointInfo();
    virtual void updateMagPointLine() {}
    virtual void specialWidgetUpdate(int &x, int &y, int &w, int &h);
    virtual void specialPaintUpdate(QPoint &s, QPoint &e);

    QPoint startPos; // 开始坐标
    QPoint endPos;   // 结束坐标

    DIRECTION startDirect = DIRECTION::STARTPOINT;             // 开始点方向
    DIRECTION endDirect = DIRECTION::ENDPOINT;                 // 结束点方向
    int startMagIndex = 0;                                     // 起始点索引值
    int endMagIndex = 0;                                       // 结束点索引值
    FlowchartElement *startChartMag = nullptr;                 // 起始图形
    FlowchartElement *endChartMag = nullptr;                   // 终止图形
    LINE_HEAD_TYPE startLineHeadType = LINE_HEAD_TYPE::NOARROW; // 起始点线头
    LINE_HEAD_TYPE endLineHeadType = LINE_HEAD_TYPE::ARROW;   // 结束点线头

    void drawLineHead(const DIRECTION direction, const LINE_HEAD_TYPE lht, const int x, const int y, QPainter &p, QPainterPath &linePath, QPainterPath &graphPath); // 绘制线头
    void drawStraightLine(int sx, int sy, int ex, int ey, QPainterPath &linePath, QPainterPath &graphPath);                                                 // 直连线绘制

public:
    Line(QWidget *parent = nullptr, PaintChartType type = PaintChartType::LINE) : FlowchartElement(parent, type, 0, 2)
    {
        chartText.text->setText("1");
    }
    ~Line() {}

    void setStartChart(FlowchartElement *cb) { startChartMag = cb; } // 设置起点指向的图形
    void resetStartChart()                                           // 重置起点指向的图形
    {
        if (startChartMag)
            startChartMag->delMagiPointStartLine(startMagIndex, this);
        startChartMag = nullptr;
    }
    FlowchartElement *getStartChart() { return startChartMag; }  // 获取起点指向的图形
    void setEndChart(FlowchartElement *cb) { endChartMag = cb; } // 设置终点指向的图形
    void resetEndChart()                                         // 重置终点指向的图形
    {
        if (endChartMag)
            endChartMag->delMagiPointEndLine(endMagIndex, this);
        endChartMag = nullptr;
    }
    FlowchartElement *getEndChart() { return endChartMag; } // 获取终点指向的图形

    LINE_HEAD_TYPE &getStartLineHeadType() { return startLineHeadType; } // 获取起点线头类型
    LINE_HEAD_TYPE &getEndLineHeadType() { return endLineHeadType; } // 获取终点线头类型


    void setStartMagIndex(int i) { startMagIndex = i; } // 设置起点指向的图形的磁力点索引
    int getStartMagIndex() { return startMagIndex; }    // 获取起点指向的图形的磁力点索引
    void setEndMagIndex(int i) { endMagIndex = i; }     // 设置终点指向的图形的磁力点索引
    int getEndMagIndex() { return endMagIndex; }        // 获取终点指向的图形的磁力点索引

    void setStartDirect(DIRECTION direct) { startDirect = direct; } // 设置连线起点的朝向
    void resetStartDirect() { startDirect = DIRECTION::NONE; }      // 重置连线起点的朝向
    void setEndDirect(DIRECTION direct) { endDirect = direct; }     // 设置连线终点的朝向
    void resetEndDirect() { endDirect = DIRECTION::NONE; }          // 重置连线终点的朝向

signals:
    void sendLineStyle(QPen &qp, QBrush &qb, LINE_HEAD_TYPE &startLineHeadType, LINE_HEAD_TYPE &endLineHeadType); // 发送连线信息
};

#endif // LINE_H
