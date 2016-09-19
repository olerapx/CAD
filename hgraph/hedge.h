#ifndef HEDGE_H
#define HEDGE_H

#include <vector>

#include "hvertex.h"

class HVertex;

using namespace std;

/**
 * @brief The HEdge class
 * A hypergraph edge.
 */
class HEdge
{
private:
    vector<HVertex*> vertices;

    int maxVerticesNumber;
    bool full;

    //Поиск свободного места и изменение состояния заполненности, если такого нет.
    size_t getFreePlaceIndex ();

public:
    HEdge ();
    HEdge (int maxVertexCount);

    bool tryConnectVertex (HVertex* newVertex);
    void disconnectVertex (HVertex* oldVertex);

    int getFreePlacesNumber();

    bool isFull();

    bool setMaxVerticesNumber (int number);
    int getMaxVerticesNumber();

    HVertex* getIncidentVertexByIndex (size_t index);

    bool isInEdge (HVertex* vertex);

    int getCommonVerticesNumber (HEdge* otherEdge);

    //Геттер числа подграфов, в которые входит ребро.
    int getSubGraphsNumber ();
};

#endif // HEDGE_H
