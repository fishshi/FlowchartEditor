#ifndef CONFIG_H
#define CONFIG_H
    // 鼠标状态常量               无      创建     运行时改变大小         运行时创建连线             运行时改变位置
enum class MOUSE_EVENT_TYPE {NONE,CREATING,CHANGE_SIZE,RUNTIME_CREATE_MAGPOINT,RUNTIME_CHANGE_POS,FRAME_SELECTING, CHANGE_FRAME_POS};
    // 方向常量         无     西北       北      东北      东        东南      南      西南     西   起点方向随意点  终点方向随意点
enum class ORIENTION{NONE, NORTHWEST, NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, STARTPOINT, ENDPOINT};
    // 箭头类型             0默认无  1箭头
enum class LINE_HEAD_TYPE{ARROW0,ARROW1};
    // 图形类型             无   矩形  菱形      圆角矩形    圆形  线性    平行四边形
enum class PaintChartType{NONE,RECT,DIAMOND,ROUNDRECT,ELLIPSE,LINE,PARALLELOGRAM, SUBPROCESSELEMENT};

enum class CHART_LABEL_MOUSE_TYPE{NONE,CHANGE_POS};

#endif // CONFIG_H
