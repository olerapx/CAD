#include "linecolorpicker.h"

LineColorPicker::LineColorPicker()
{
    counter = 0;

    colors = {QColor(255, 0, 0), QColor(0, 255, 0), QColor(0, 0, 255), QColor(76, 0, 153), QColor(255, 128, 0)};
}

QColor LineColorPicker::getColor()
{
    if (counter < colors.size())
        return colors[counter ++];

    return getRandomColor();
}

QColor LineColorPicker::getRandomColor()
{
    QColor color;

    do
    {
        color = QColor(rand()%255, rand()%255, rand()%255);
    }
    while (doesRepeat(color));

    return color;
}

bool LineColorPicker::doesRepeat(QColor color)
{
    size_t size = colors.size();

    for (size_t i = 0; i<size; i++)
        if (colors[i] == color)
            return true;
    return false;
}
