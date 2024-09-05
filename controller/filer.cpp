#include "filer.h"

Filer::Filer(Canvas *canvas)
{
    this->canvas = canvas;
}

void Filer::openFile(QString filePath)
{
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QDataStream fin(&file);
    std::map<int, FlowchartElement *> chartMap;

    unsigned long long cnt;
    fin.readRawData(reinterpret_cast<char *>(&cnt), sizeof(unsigned long long));
    for (unsigned long long i = 0; i < cnt; i++)
    {
        PaintChartType tmp;
        FlowchartElement *cb;
        fin.readRawData(reinterpret_cast<char *>(&tmp), sizeof(PaintChartType));
        switch (tmp)
        {
        case PaintChartType::RECT:
            cb = new ProcessElement(canvas);
            break;
        case PaintChartType::DIAMOND:
            cb = new DecisionElement(canvas);
            break;
        case PaintChartType::ROUNDRECT:
            cb = new StartEndElement(canvas);
            break;
        case PaintChartType::ELLIPSE:
            cb = new ConnectorElement(canvas);
            break;
        case PaintChartType::PARALLELOGRAM:
            cb = new DataElement(canvas);
            break;
        case PaintChartType::SUBPROCESSELEMENT:
            cb = new SubprocessElement(canvas);
            break;
        case PaintChartType::DOCUMENTELEMENT:
            cb = new DocumentElement(canvas);
            break;
        }
        cb->chartType = tmp;
        fin >> (*cb);
        canvas->charts.push_back(cb);
        cb->applyWidthHeight();
        cb->update();
        cb->show();
        chartMap[cb->getID()] = cb;
    }
    fin.readRawData(reinterpret_cast<char *>(&cnt), sizeof(unsigned long long));
    for (unsigned long long i = 0; i < cnt; i++)
    {
        PaintChartType tmp;
        FlowchartElement *cb;
        Line *cl;
        int id;
        fin.readRawData(reinterpret_cast<char *>(&tmp), sizeof(PaintChartType));
        cb = new Line(canvas);
        canvas->line.push_back(cb);
        cl = dynamic_cast<Line *>(cb);
        fin >> (*cb) >> (*cl);

        fin.readRawData(reinterpret_cast<char *>(&id), sizeof(int));
        if (id >= 0)
        {
            FlowchartElement *cbs = chartMap.at(id);
            cbs->addMagiPointStartLine(cl->getStartMagIndex(), cl);
            cl->setStartChart(cbs);
        }

        fin.readRawData(reinterpret_cast<char *>(&id), sizeof(int));
        if (id >= 0)
        {
            FlowchartElement *cbe = chartMap.at(id);
            cbe->addMagiPointEndLine(cl->getEndMagIndex(), cl);
            cl->setEndChart(cbe);
        }

        cl->applyWidthHeight();
        cl->update();
        cl->show();
    }
    canvas->hideMagSizeAll();
}

void Filer::saveFile(QString filePath)
{
    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    QDataStream fout(&file);
    unsigned long long i;
    i = canvas->charts.size();
    fout.writeRawData(reinterpret_cast<const char *>(&i), sizeof(unsigned long long));
    for (auto it = canvas->charts.begin(); it != canvas->charts.end(); it++)
    {
        fout << *(*it);
    }
    i = canvas->line.size();
    fout.writeRawData(reinterpret_cast<const char *>(&i), sizeof(unsigned long long));
    for (auto it = canvas->line.begin(); it != canvas->line.end(); it++)
    {
        fout << *(*it) << *(reinterpret_cast<const Line *>(*it));
    }
    file.close();
}

void Filer::saveAsSVG(QString filename)
{
    // 创建 QSvgGenerator 对象
    QSvgGenerator generator;
    generator.setFileName(filename);
    int width = 0, height = 0;
    for (auto x : canvas->charts)
    {
        if (x->widgetEnd.rx() > width)
            width = x->widgetEnd.rx();
        if (x->widgetEnd.ry() > height)
            height = x->widgetEnd.ry();
    }
    generator.setSize(QSize(width + 50, height + 50));
    generator.setViewBox(QRect(0, 0, width + 50, height + 50));
    // 创建 QPainter 并设置到生成器上
    QPainter painter(&generator);
    // 渲染父部件及其子部件到 SVG 生成器
    canvas->render(&painter);
    // 结束绘图
    painter.end();
}

void Filer::saveAsPNG(QString filename)
{
    // 计算要保存的图片的宽度和高度
    int width = 0, height = 0;
    for (auto x : canvas->charts)
    {
        if (x->widgetEnd.rx() > width)
            width = x->widgetEnd.rx();
        if (x->widgetEnd.ry() > height)
            height = x->widgetEnd.ry();
    }

    // 增加边界大小
    QSize imageSize(width + 50, height + 50);

    // 创建 QPixmap 对象，设置大小和格式
    QPixmap pixmap(imageSize);
    pixmap.fill(Qt::white); // 填充背景颜色为白色（可以根据需要更改）

    // 创建 QPainter 并设置到 pixmap 上
    QPainter painter(&pixmap);

    // 渲染父部件及其子部件到 QPixmap
    canvas->render(&painter);

    // 保存 pixmap 到 PNG 文件
    pixmap.save(filename, "PNG");

    // 结束绘图
    painter.end();
}
