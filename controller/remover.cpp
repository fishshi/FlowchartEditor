#include "remover.h"
#include <QDir>
#include <QDebug>

Remover::Remover(Canvas *canvas) {
    this->canvas = canvas;
}

void Remover::delChart(FlowchartElement *&fce)
{
    for(int i = 0; i < canvas->charts.size(); ++i)
    {
        if(canvas->charts[i] == fce)
        {
            FlowchartElement *tmp = fce;
            canvas->charts.erase(canvas->charts.begin() + i);
            --i;
            for(auto magit = tmp->magPoint.i_point.begin();magit!=tmp->magPoint.i_point.end();magit++)
            {
                for(auto magLineStIt = (*magit)->i_lineStart.begin();magLineStIt != (*magit)->i_lineStart.end();magLineStIt++)
                {
                    dynamic_cast<Line*>(*magLineStIt) ->resetEndChart();
                    delLine(*magLineStIt);
                }
                for(auto magLineEnIt = (*magit)->i_lineEnd.begin();magLineEnIt != (*magit)->i_lineEnd.end();magLineEnIt++)
                {
                    dynamic_cast<Line*>(*magLineEnIt) ->resetStartChart();
                    delLine(*magLineEnIt);
                }
            }
            delete tmp;
            break;
        }
    }
}

void Remover::delLine(FlowchartElement *&fce)
{
    for(auto it = canvas->line.begin();it != canvas->line.end();it++)
    {
        if(*it == fce)
        {
            Line *tmp = dynamic_cast<Line*>(*it);
            fce = nullptr;
            canvas->line.erase(it);
            tmp->resetEndChart();
            tmp->resetStartChart();
            delete tmp;
            break;
        }
    }
}

void Remover::clear()
{
    for(auto it = canvas->charts.begin();it!= canvas->charts.end();it++) {
        if(*it) delete *it;
        *it = nullptr;
    }
    for(auto it = canvas->line.begin();it!= canvas->line.end();it++) {
        if(*it) delete *it;
        *it = nullptr;
    }
    canvas->charts.clear();
     qDebug()<<6;
    //qDebug() << "Redo directory:" << QDir::current().filePath("../../assets/cashe/Redo");
    //qDebug() << "Undo directory:" << QDir::current().filePath("../../assets/cashe/Uedo");
    canvas->line.clear();
}

void Remover::clearCasheRe()
{
    QDir dir(QDir::current().filePath("../../assets/cashe/Redo"));
    foreach (QFileInfo file, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot)) {
        QFile::remove(file.absoluteFilePath());
    }

}

void Remover::clearCasheUn()
{
    QDir dir2(QDir::current().filePath("../../assets/cashe/Undo"));
    foreach (QFileInfo file, dir2.entryInfoList(QDir::Files | QDir::NoDotAndDotDot)) {
        QFile::remove(file.absoluteFilePath());
    }
}
