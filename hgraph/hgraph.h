#ifndef HGRAPH_H
#define HGRAPH_H

#include <cmath>
#include <cstdlib>

#include "hvertex.h"
#include "hedge.h"
#include "hgraphexception.h"

using namespace std;

/**
 * @brief The HGraph class
 * A hypergraph.
 */
class HGraph
{
    friend class HGraphWorker;

private:
    vector<HVertex*> vertices;
    vector<HEdge*> edges;

    int ID; // Номер подграфа (у главного -1)

    size_t verticesNumber;
    size_t edgesNumber;
    size_t subGraphsNumber;

    bool root;

    void fillSubGraph (vector <HVertex*> graphVertices, int graphID, vector<HEdge *> newEdges);

    size_t getTotalEdgesNumber ();
    size_t getMaxEdgesNumber ();

    size_t getNonFullVerticesNumber();

public:
    HGraph ();
    ~HGraph();

    /**
     * @brief HGraph
     * A sub graph constructor.
     */
    HGraph (vector<HVertex*>& graphVertices, int graphID);

    int getID();

    size_t getVerticesNumber ();
    void setVerticesNumber(size_t number);

    size_t getEdgesNumber ();
    void setEdgesNumber(size_t number);

    size_t getFragmentsNumber();

    size_t getExternalEdgesNumber ();

    bool isRoot();

    bool isIncident (HVertex* vertex, HEdge* edge);
    bool installIncidence (HVertex* vertex, HEdge* edge);
    void uninstallIncidence (HVertex* vertex, HEdge* edge);

    /**
     * @brief createSubHG
     * Creates an empty subgraph.
     * @param subGraphID - the subgraph number.
     * @return The created graph.
     */
    HGraph* createSubHG (int subGraphID);
};

#endif // HGRAPH_H
