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
private:
    vector<HVertex*> vertices;
    vector<HEdge*> edges;

    int ID; // Номер подграфа (у главного -1)

    size_t verticesNumber;
    size_t edgesNumber;
    size_t subGraphsNumber;

    bool isRoot;

    void fillSubGraph (vector <HVertex*> graphVertices, int graphID, vector<HEdge *> newEdges);

    void dragEdgeInSubGraph (size_t subGraphVerticesNumber, int subGraphID);

    void createVertices (size_t verticesNumber, size_t minEdgesNumber, size_t maxEdgesNumber);

    size_t getTotalEdgesNumber ();
    size_t getMaxEdgesNumber ();

    size_t getNonFullVerticesNumber();

    void createEdges (size_t minVerticesNumber, size_t maxVerticesNumber);

public:
    HGraph ();
    ~HGraph();

    /**
     * @brief HGraph
     * A sub graph constructor.
     */
    HGraph (vector<HVertex*>& graphVertices, int graphID);

    /**
     * @brief createSubHG
     * Creates an empty subgraph.
     * @param subGraphID - the subgraph number.
     * @return The created graph.
     */
    HGraph* createSubHG (int subGraphID);

    void randomSplit (size_t subGraphsNumber, int startID);
    void gravitySplit (size_t subGraphsNumber, int startID);

    void resetSplitting();

    // Генератор гиперграфа
    void HGraphGenerator (size_t verticesNumber, size_t minEdgesNumber, size_t maxEdgesNumber,
                          size_t minVerticesNumber, size_t maxVerticesNumber);

    int getID();

    size_t getExternalEdgesNumber ();

    bool isIncident (HVertex* vertex, HEdge* edge);
    bool installIncidence (HVertex* vertex, HEdge* edge);
    void uninstallIncidence (HVertex* vertex, HEdge* edge);

    size_t getVerticesNumber ();
    void setVerticesNumber(size_t number);

    size_t getEdgesNumber ();
    void setEdgesNumber(size_t number);

    size_t getFragmentsNumber();
};

#endif // HGRAPH_H
