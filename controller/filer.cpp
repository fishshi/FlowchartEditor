#include "filer.h"

Filer::Filer(Canvas *canvas) {
    this->canvas = canvas;
}

void Filer::openFile(QString filePath)
{
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QDataStream fin(&file);
    std::map<int,FlowchartElement*> chartMap;

    unsigned long long cnt;
    fin.readRawData(reinterpret_cast<char*>(&cnt),sizeof(unsigned long long));
    qDebug()<<"图形个数："<<cnt;
    for(unsigned long long i = 0;i<cnt;i++)
    {
        PaintChartType tmp;
        FlowchartElement *cb;
        fin.readRawData(reinterpret_cast<char*>(&tmp),sizeof(PaintChartType));
        switch(tmp)
        {
        case PaintChartType::RECT:
        {
            cb = new ProcessElement(canvas);
        }break;
        case PaintChartType::DIAMOND:
        {
            cb = new DecisionElement(canvas);
        }break;
        case PaintChartType::ROUNDRECT:
        {
            cb = new StartEndElement(canvas);
        }break;
        case PaintChartType::ELLIPSE:
        {
            cb = new ConnectorElement(canvas);
        }break;
        case PaintChartType::PARALLELOGRAM:
        {
            cb = new DataElement(canvas);
        }break;
        default:case PaintChartType::NONE:{
            cb = nullptr;
        }break;
        }
        cb->chartType = tmp;
        fin>>(*cb);
        canvas->charts.push_back(cb);
        cb->applyWidthHeight();
        cb->update();
        cb->show();
        chartMap[cb->getID()] = cb;
    }
    fin.readRawData(reinterpret_cast<char*>(&cnt),sizeof(unsigned long long));
    qDebug()<<"连线个数："<<cnt;
    for(unsigned long long i = 0;i<cnt;i++)
    {
        PaintChartType tmp;
        FlowchartElement *cb;
        Line *cl;
        int id;
        fin.readRawData(reinterpret_cast<char*>(&tmp),sizeof(PaintChartType));
        cb = new Line(canvas);
        canvas->line.push_back(cb);
        if(nullptr == (cl = dynamic_cast<Line*>(cb))) qDebug()<<"error";
        fin>>(*cb)>>(*cl);
        try{
            fin.readRawData(reinterpret_cast<char*>(&id),sizeof(int));
            if(id>=0)
            {
                FlowchartElement *cbs = chartMap.at(id);
                cbs->addMagiPointStartLine(cl->getStartMagIndex(),cl);
                cl->setStartChart(cbs);
            }
        }catch(std::out_of_range &oor){
            qDebug()<<oor.what()<<"Not Found Start chart.";
        }
        try{
            fin.readRawData(reinterpret_cast<char*>(&id),sizeof(int));
            if(id>=0)
            {
                FlowchartElement *cbe = chartMap.at(id);
                cbe->addMagiPointEndLine(cl->getEndMagIndex(),cl);
                cl->setEndChart(cbe);
            }
        }catch(std::out_of_range &oor){
            qDebug()<<oor.what()<<"Not Found End chart.";
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
    fout.writeRawData(reinterpret_cast<const char*>(&i),sizeof(unsigned long long));
    for(auto it = canvas->charts.begin(); it!= canvas->charts.end(); it++)
    {
        fout<<*(*it);
    }
    i = canvas->line.size();
    fout.writeRawData(reinterpret_cast<const char*>(&i),sizeof(unsigned long long));
    for(auto it = canvas->line.begin(); it!= canvas->line.end(); it++)
    {
        fout<<*(*it)<<*(reinterpret_cast<const Line*>(*it));
    }
    file.close();
}
