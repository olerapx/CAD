#ifndef HGEDGE_H
#define HGEDGE_H

#include "hgvertex.h"

#include <vector>

class HGVertex;

using namespace std;

/**
 * @brief The HGEdge class
 * Ребро гиперграфа.
 */
class HGEdge
{
private:
    int maxCountOfVertex;                         // Максимальная мощность ребра
    vector<HGVertex*> vertices;                   // Указатели на вершины
    bool full;                                    // Состояние заполненности

    //Поиск свободного места и изменение состояния заполненности, если такого нет.
    int freePlaceForConnect ();

public:
    HGEdge ();
    HGEdge (int maxVertexCount);

    ~HGEdge ();

    //Исключение вершины из ребра.
    void disconnectVertex (HGVertex *oldVertex);

    int getCountOfFreePlaces();

    //Попытка включения вершины в ребро.
    bool connectVertex (HGVertex *newVertex);

    bool isFull ();

    //Сеттер/геттер максимальной мощности.
    bool setMaxCountOfVertex (int maxCV);
    int getMaxCountOfVertex();

    //Геттер вершины по номеру. Возвращает nullptr, если такой не существует.
    HGVertex* getVertex (int number);

    // Проверка принадлежности вершины ребру.
    bool isInEdge (HGVertex *v);

    //Геттер числа общих с другим ребром вершин.
    int getCountOfCommonVertices (HGEdge *otherEdge);

    //Геттер числа подграфов, в которые входит ребро.
    int getCountSubHG ();
};

#endif // HGEDGE_H
