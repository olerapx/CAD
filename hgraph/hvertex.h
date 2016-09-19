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
    int maxEdgesNumber;
    int graphID; // Номер подграфа, -1, если не принадлежит ни к одному
    bool full;

    // Поиск свободного места и изменение состояния заполненности, если такого нет
    size_t getFreePlaceIndex();

public:
    HVertex ();
    HVertex (int id, int maxEdgesNumber);

    int getGraphID ();
    void setGraphID (int id);

    bool tryConnectEdge (HEdge* newEdge);
    void disconnectEdge (HEdge* oldEdge);

    bool isFull ();

    int getID ();
    bool setID (int id);

    int getMaxEdgesNumber ();
    bool setMaxEdgesNumber (int number);

    bool isInEdge (HEdge *edge);

    int getFreePlacesNumber();

    HEdge* getIncidentEdgeByIndex (int index);
};

#endif // HVERTEX_H
