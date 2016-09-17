#ifndef HGVERTEX_H
#define HGVERTEX_H

#include <vector>

#include "hgedge.h"

class HGEdge;

using namespace std;

/**
 * @brief The HGVertex class
 * Вершина гиперграфа.
 */
class HGVertex
{
private:
    vector <HGEdge*> edges;                               // Инцидентные гиперребра

    int ID;                                  // Собственный номер вершины
    int maxEdgesNumber;                                // Максимально возможное число инцидентных гиперребер
    int graphID;                               // Номер подграфа, -1, если не принадлежит ни к одному
    bool full;                                    // Состояние заполненности

    // Поиск свободного места и изменение состояния заполненности, если такого нет
    int freePlaceForConnect();

public:
    HGVertex ();
    HGVertex (int nV, int maxD);

    int getGraphID ();
    void setGraphID (int id);

    // Попытка включения вершины в ребро
    bool connectEdge (HGEdge *newEdge);

    // Отключение вершины от ребра
    void disconnectEdge (HGEdge *oldEdge);

    // Геттер заполненности
    bool isFull ();

    int getID ();
    bool setID (int id);

    int getMaxEdgesNumber ();
    bool setMaxEdgesNumber (int mD);

    // Проверка принадлежности
    bool isInEdge (HGEdge *e);
    // вершины ребру

    // Геттер числа свободных мест
    int getFreePlacesCount ();
    // для подключений

    // Геттер инцидентного ребра по номеру
    HGEdge* getEdge (int numberOfEdge);
};

#endif // HGVERTEX_H
