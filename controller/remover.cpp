#include "remover.h"

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
    canvas->line.clear();
}
