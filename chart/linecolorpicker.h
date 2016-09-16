#ifndef LINECOLORPICKER_H
#define LINECOLORPICKER_H

#include <vector>
#include <QColor>

class LineColorPicker
{
private:
    size_t counter;

    std::vector <QColor> colors;

    QColor getRandomColor();

public:
    LineColorPicker();

    QColor getColor();
    bool doesRepeat(QColor color);
};

#endif // LINECOLORPICKER_H
