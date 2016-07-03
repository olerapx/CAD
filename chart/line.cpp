#include "line.h"

Line::Line()
{

}

double Line::getMaxX()
{
    if (x.size()==0) return 0.0;

    double max = 0.0;

    for (int i=0;i<x.size();i++)
        if (x[i]>max) max = x[i];

    return max;
}

double Line::getMaxY()
{
    if (y.size()==0) return 0.0;

    double max = 0.0;

    for (int i=0;i<y.size();i++)
        if (y[i]>max) max = y[i];

    return max;
}

void Line::clear()
{
    x.clear();
    y.clear();
}
