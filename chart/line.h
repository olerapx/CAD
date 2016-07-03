#ifndef LINE_H
#define LINE_H

#include <QVector>

class Line
{
public:
    Line();

    QVector<double> x, y;

    double getMaxX();
    double getMaxY();

    void clear();
};

#endif // LINE_H
