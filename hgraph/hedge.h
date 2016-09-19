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

    size_t maxVerticesNumber;
    bool full;

    //Поиск свободного места и изменение состояния заполненности, если такого нет.
    size_t getFreePlaceIndex ();

public:
    HEdge ();
    HEdge (size_t maxVerticesNumber);

    bool tryConnectVertex (HVertex* newVertex);
    void disconnectVertex (HVertex* oldVertex);

    size_t getFreePlacesNumber();

    bool isFull();

    void setMaxVerticesNumber(size_t number);
    size_t getMaxVerticesNumber();

    HVertex* getIncidentVertexByIndex (size_t index);

    bool isInEdge (HVertex* vertex);

    size_t getCommonVerticesNumber (HEdge* otherEdge);

    //Геттер числа подграфов, в которые входит ребро.
    size_t getSubGraphsNumber ();
};

#endif // HEDGE_H
