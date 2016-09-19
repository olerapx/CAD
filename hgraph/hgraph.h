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

    int verticesNumber;
    int edgesNumber;
    int subGraphsNumber;

    bool isRoot;

    void fillSubGraph (vector <HVertex*> graphVertices, int graphID, vector<HEdge *> newEdges);

    void dragEdgeInSubGraph (int subGraphVerticesNumber, int subGraphID);

    void createVertices (int verticesNumber, int minEdgesNumber, int maxEdgesNumber);

    int getTotalEdgesNumber ();
    int getMaxEdgesNumber ();

    int getNonFullVerticesNumber();

    void createEdges (int minVerticesNumber, int maxVerticesNumber);

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

    void randomSplit (int subGraphsNumber, int startID);
    void gravitySplit (int subGraphsNumber, int startID);

    void resetSplitting();

    // Генератор гиперграфа
    void HGraphGenerator (int verticesNumber, int minEdgesNumber, int maxEdgesNumber,
                          int minVerticesNumber, int maxVerticesNumber);

    int getID();

    int getExternalEdgesNumber ();

    bool isIncident (HVertex* vertex, HEdge* edge);
    bool installIncidence (HVertex* vertex, HEdge* edge);
    void uninstallIncidence (HVertex* vertex, HEdge* edge);

    int getVerticesNumber ();
    bool setVerticesNumber (int number);

    int getEdgesNumber ();
    bool setEdgesNumber (int number);

    int getFragmentsNumber();
};

#endif // HGRAPH_H
