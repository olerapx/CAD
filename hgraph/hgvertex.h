#ifndef HGVERTEX_H
#define HGVERTEX_H

#include "hgedge.h"

class HGEdge;

/**
 * @brief The HGVertex class
 * Вершина гиперграфа.
 */
class HGVertex
{
private:
    int numberV;                                  // Собственный номер вершины
    int maxDegree;                                // Максимально возможное число инцидентных гиперребер
    int numberOfHG;                               // Номер подграфа, -1, если не принадлежит ни к одному
    HGEdge **edges;                               // Инцидентные гиперребра
    bool full;                                    // Состояние заполненности

    // Поиск свободного места и изменение состояния заполненности, если такого нет
    int freePlaceForConnect();

public:
    HGVertex ();
    HGVertex (int nV, int maxD);

    ~HGVertex ();

    // Геттер номера подграфа
    int getNumberOfHG ();

    // Сеттер номера подграфа
    void setNumberOfHG (int numberSubHG);

    // Попытка включения вершины в ребро
    bool connectVertex (HGEdge *newEdge);

    // Отключение вершины от ребра
    void disconnectVertex (HGEdge *oldEdge);

    // Геттер заполненности
    bool isFull ();

    // Геттер собственного номера вершины
    int getNumberV ();

    // Сеттер собственного номера вершины
    bool setNumberV (int nV);

    // Геттер максимальной ЛС
    int getMaxDegree ();

    // Сеттер максимальной ЛС
    bool setMaxDegree (int mD);

    // Проверка принадлежности
    bool isInEdge (HGEdge *e);
    // вершины ребру

    // Геттер числа свободных мест
    int getCountOfFreePlaces ();
    // для подключений

    // Геттер инцидентного ребра по номеру
    HGEdge * getEdge (int numberOfEdge);
};

#endif // HGVERTEX_H
