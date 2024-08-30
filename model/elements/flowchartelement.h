#ifndef FLOWCHARTELEMENT_H
#define FLOWCHARTELEMENT_H

#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QPolygon>

#include <QWidget>
#include <QPoint>
#include <QDebug>
#include <QMouseEvent>
#include <QLabel>
#include <QLineEdit>
#include <QSizePolicy>

#include <vector>

#include "../config.h"

#include "label.h"

class FlowchartElement :public QWidget
{
    Q_OBJECT
    friend class Canvas;
    friend class Controller;
    friend class Drawer;
    friend class Updater;
    friend class Remover;
private:
    static int magPointWidth;                   // Padding信息-磁力点宽度
    static int sizePointWidth;                  // Padding信息-大小点宽度
    static int pointLineWidth;                  // Padding信息-大小点、磁力点线宽度
    static int chartIDCount;                    // 总ID计数
    static const int magPointAutoMagiRange = 10;// 磁力点自动吸附范围
    static const int borderWidth = 20;          // Border信息-禁止绘画区域宽度
    static const int minSizeWH = 40;            // 图形最小宽高
    static const int textBorderWidth = 10;      // 文件边界最小宽高

    bool showAll = true;    // 显示大小控制点和磁力点？ ###
    bool showMag = false;   // 显示磁力点？ ###
    ORIENTION lastType = ORIENTION::NONE;               // 改变大小状态切换
    MOUSE_EVENT_TYPE curFlag = MOUSE_EVENT_TYPE::NONE;  // 鼠标事件类型
    ORIENTION curIndex = ORIENTION::NONE;               // 当前选中大小点、磁力点方向
    PaintChartType chartType = PaintChartType::NONE;    // 图形类型
    int ID;         // 图形编号

    static void resetStaticVal(){
        FlowchartElement::magPointWidth = 7;
        FlowchartElement::sizePointWidth = 9;
        FlowchartElement::pointLineWidth = 2;
        FlowchartElement::chartIDCount = 0;
    }
    void varInit(int mpw=7,int spw=9,int plw=2,bool sa=true,bool smo=false);// 变量默认初始化函数
    void widgetPosInit(int x=0,int y=0,int w=minSizeWH,int h=minSizeWH);    // 窗体位置信息默认初始化函数
    void paintInit();        // 图形绘制信息默认初始化
    void pointInit();        // 大小点和控制点默认初始化
    void textInit();         // 文字内容默认初始化
    void colorInit();        // 颜色默认初始化

    void updateWidgetPosInof();         // 更新Widget位置信息
    virtual void specialWidgetUpdate(int &x,int &y, int &w, int &h){}   // 特殊图形边界范围更新函数
    void updatePaintInfo();             // 更新绘图范围信息
    virtual void specialPaintUpdate(QPoint &s, QPoint &e){}             // 特殊绘图边界范围设置函数
    virtual void updateSizePointInfo(); // 更新大小点位置信息
    virtual void updateMagPointInfo();  // 更新磁力点的位置信息
    void adjustPointInfo();             // 调整磁力点和大小点的位置信息
    void updateSizePointPath();         // 更新大小点绘制范围信息
    void updateMagPointPath();          // 更新磁力点的绘制范围信息
    virtual void updateMagPointLine();  // 更新磁力点上连线的位置信息
    void updateTextInfo();              // 更新显示文本的信息

    virtual void paintChart(QPainter & p) = 0;  // 绘制图形
    void paintSizePoint(QPainter & p);          // 绘制大小点
    virtual void paintSizeEdge(QPainter & p);   // 绘制大小边界
    void paintMagPoint(QPainter & p);           // 绘制磁力点

    bool inPath(const QPointF &p);                                                                              // 是否可选的图形范围内，调用了下方3个函数
    virtual bool inGraphisPath(const QPointF &p) {if(graphPath)return graphPath->contains(p);else return false;}// 是否在图形范围内
    bool inMagPath(const QPointF &p, ORIENTION &b, int &index) const;                                           // 是否在磁力点范围内
    bool inSizePath(const QPointF &p, ORIENTION &b) const;                                                      // 是否在大小点范围内

    void setStartPos(int x,int y);
    void setEndPos(int x,int y);

protected:
    class TextBase {
    public:
        TextBase():textType1(nullptr),tmpEdit1(nullptr){

        }
        ~TextBase(){
            if(textType1){
                delete textType1;
                textType1 = nullptr;
            }
            if(tmpEdit1){
                delete tmpEdit1;
                tmpEdit1 = nullptr;
            }
        }
        bool textType = 1;
        QPoint chartTextMousePos;           // 保存文字移动时点击的位置
        union{// 图形显示的文字控件
            Label *textType1 = nullptr;
        };
        union{// 图形编辑时文字输入控件
            QLineEdit *tmpEdit1 = nullptr;
        };
        union{// 鼠标移动标识符
            CHART_LABEL_MOUSE_TYPE textMouseT1 = CHART_LABEL_MOUSE_TYPE::NONE;
        };
    }chartText; // 文本控件

    class i_pointbase   // 点基本信息
    {
    public:
        QPoint *i_pos = nullptr;            // 点位置
        QPainterPath *i_path = nullptr;     // 点范围
        ORIENTION rotate = ORIENTION::NONE; // 点方向

        i_pointbase():i_pos(nullptr),i_path(nullptr),rotate(ORIENTION::NONE){

        }
        ~i_pointbase(){
            if(i_pos){
                delete i_pos;
                i_pos = nullptr;
            }
            if(i_path){
                delete i_path;
                i_path = nullptr;
            }
        }

        void setX(int x){i_pos->setX(x);}
        void setY(int y){i_pos->setY(y);}
        int getX() const{return i_pos->rx();}
        int getY() const{return i_pos->ry();}
        void setRotate(ORIENTION r){rotate = r;}
        const ORIENTION & getRotate()const {return rotate;}
        void setPath(const QPainterPath& p){if(i_path) delete i_path;i_path = new QPainterPath(p);}
        QPainterPath* newPath(){if(i_path) delete i_path;i_path = new QPainterPath;return i_path;}
        QPainterPath* getPath() const{return i_path;}
        bool inPath(const QPointF a){ return i_path->contains(a);}
    };
    class i_magpointbase : public i_pointbase    // 磁力点信息类结构基础
    {
    public:
        std::vector<FlowchartElement*> i_lineStart; // 每个磁力点连接的线的指针容器
        std::vector<FlowchartElement*> i_lineEnd; // 每个磁力点连接的线的指针容器
        i_magpointbase() :i_pointbase(){}
    };

    class i_sizepoint{   // 大小点信息类结构
    public:
        std::vector<i_pointbase*> i_point;  // 大小点指针容器

        explicit i_sizepoint(int c=0):i_point(c){}
        ~i_sizepoint()
        {
            for(auto it = i_point.begin();it!=i_point.end();it++) {
                if(*it) delete *it;
                *it = nullptr;
            }
        }
    };

    class i_magpoint{    // 磁力点信息类结构
    public:
        std::vector<i_magpointbase*> i_point;   // 磁力点指针容器

        explicit i_magpoint(int c=0):i_point(c){}
        ~i_magpoint()
        {
            for(auto it = i_point.begin();it!=i_point.end();it++) {
                if(*it) delete *it;
                *it = nullptr;
            }
        }
    };

    i_magpoint magPoint;       // 磁力点信息
    i_sizepoint sizePoint;     // 大小点信息

    QPainterPath *graphPath = nullptr;  // 图形绘制范围

    QPoint paintStart;      // 图形绘制范围起点
    QPoint paintEnd;        // 图形绘制范围终点
    QPoint widgetStart;     // widget在画布上的起始位置：易变
    QPoint widgetEnd;       // widget在画布上的结束位置：易变

    QPen paintChartDrawPen;     // 图形画笔
    QBrush paintChartFillPen;   // 图形填充

    static QPen paintDrawPen;       // 图形大小点、磁力点画笔
    static QBrush paintFillPen;     // 图形大小点、磁力点填充

    bool movable = true;    // 图形位置可改变

public:
    explicit FlowchartElement(QWidget *parent = nullptr, PaintChartType type = PaintChartType::NONE, bool mov = true, int mpc=4, int spc=4);
    FlowchartElement( int x, int y, int w, int h, QWidget *parent = nullptr, PaintChartType type = PaintChartType::NONE);
    explicit FlowchartElement(FlowchartElement &);
    virtual ~FlowchartElement()
    {
        if(graphPath)
        {
            delete graphPath;
            graphPath = nullptr;
        }
    }

    void setXY(int x, int y);                           // 设置位置
    void setWidthHeight(int x, int y);                  // 设置大小、更新数据，用于创建时
    void setWidthHeight(int x, int y, ORIENTION type);  // 设置大小、更新数据，用于创建好之后
    void applyWidthHeight();                            // 更新数据，用于读取时
    bool autoSetMagi(int &x, int &y, int &index);   // 磁力点吸附函数
    int getMagiPointAbsX(int i){;return magPoint.i_point[i]->getX() + x();} // 获取磁力点绝对坐标
    int getMagiPointAbsY(int i){;return magPoint.i_point[i]->getY() + y();} // 获取磁力点相对坐标
    void addMagiPointStartLine(int i,FlowchartElement *cb){magPoint.i_point[i]->i_lineStart.push_back(cb);}   // 增加连线到相应的磁力点的起始连线容器
    void delMagiPointStartLine(int i,FlowchartElement *cb)                                                    // 删除磁力点的起始连线容器中相应的连线
    {
        for(auto it = magPoint.i_point[i]->i_lineStart.begin(),end = magPoint.i_point[i]->i_lineStart.end();it!=end;it++)
        {
            if((*it) == cb)
            {
                qDebug()<<"test1"<<magPoint.i_point[i]->i_lineStart.size();
                magPoint.i_point[i]->i_lineStart.erase(it);
                qDebug()<<"test2"<<magPoint.i_point[i]->i_lineStart.size();
                break;
            }
        }
    }
    void addMagiPointEndLine(int i,FlowchartElement *cb){magPoint.i_point[i]->i_lineEnd.push_back(cb);}       // 增加连线到相应的磁力点的终点连线容器
    void delMagiPointEndLine(int i,FlowchartElement *cb)                                                      // 删除磁力点的终点连线容器中相应的连线
    {
        for(auto it = magPoint.i_point[i]->i_lineEnd.begin(),end = magPoint.i_point[i]->i_lineEnd.end();it!=end;it++)
        {
            if((*it) == cb)
            {
                magPoint.i_point[i]->i_lineEnd.erase(it);
                break;
            }
        }
    }
    ORIENTION getMagiPointDirect(int i){return magPoint.i_point[i]->getRotate();}       // 获取磁力点的索引位置
    void overlapChartMousePressed(QMouseEvent *event);  // 鼠标点击事件Z-index检测
    void overlapChartMouseMove(QMouseEvent *event);     // 鼠标移动事件Z-index检测
    void setMovalbe(bool f);            // 设置可否移动位置
    int & getID(void){return ID;}       // 获得唯一ID值

signals:
    void sendThisClass(FlowchartElement *,int x,int y);       // 发送自己给画布

    void setTypeChangeSize(ORIENTION i);                            // 设置画布中的鼠标状态为改变大小
    void setTypeCreateMagPoint(FlowchartElement *,ORIENTION d, int i);    // 设置画布中的鼠标状态为创建连线

public slots:

    void showMagSize(); // 显示磁力点、大小点
    void hideMagSize(); // 隐藏磁力点、大小点
    void showMagOnly(); // 显示磁力点
    void hideMagOnly(); // 隐藏磁力点
    void setTypeChangeTextPos(CHART_LABEL_MOUSE_TYPE t,int x,int y){chartText.textMouseT1 = t;chartText.chartTextMousePos.setX(x);chartText.chartTextMousePos.setY(y);} // 设置文本位置

protected:// 事件
    virtual void paintEvent(QPaintEvent *event);            // 绘制控件事件

    virtual void mousePressEvent(QMouseEvent *event);       // 鼠标按下事件
    virtual void mouseMoveEvent(QMouseEvent *event);        // 鼠标移动事件
    virtual void mouseReleaseEvent(QMouseEvent *event);     // 鼠标释放事件
    virtual void mouseDoubleClickEvent(QMouseEvent *event); // 鼠标双击事件
};
#endif // FLOWCHARTELEMENT_H
