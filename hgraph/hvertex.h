#ifndef HVERTEX_H
#define HVERTEX_H

#include <vector>

#include "hedge.h"

class HEdge;

using namespace std;

/**
 * @brief The HVertex class
 * A hypergraph vertex.
 */
class HVertex
{
private:
    vector <HEdge*> edges;

    int ID;
    size_t maxEdgesNumber;
    int graphID; // Номер подграфа, -1, если не принадлежит ни к одному
    bool full;

    // Поиск свободного места и изменение состояния заполненности, если такого нет
    size_t getFreePlaceIndex();

public:
    HVertex ();
    HVertex (int id, size_t maxEdgesNumber);

    int getGraphID ();
    void setGraphID (int id);

    bool tryConnectEdge (HEdge* newEdge);
    void disconnectEdge (HEdge* oldEdge);

    bool isFull ();

    int getID ();
    bool setID (int id);

    size_t getMaxEdgesNumber ();
    void setMaxEdgesNumber(size_t number);

    bool isInEdge (HEdge *edge);

    size_t getFreePlacesNumber();

    HEdge* getIncidentEdgeByIndex (size_t index);
};

#endif // HVERTEX_H
