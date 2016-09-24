#ifndef HGRAPHWORKER_H
#define HGRAPHWORKER_H

#include "hgraph.h"

/**
 * @brief The HGraphWorker class
 * Represents the hypergraph generating and splitting algorithms.
 */
class HGraphWorker
{
private:
    static size_t minVerticesNumber, maxVerticesNumber;
    static size_t actualMaxEdgesNumber;

    static void dragEdgeInSubGraph (HGraph* graph, size_t subGraphVerticesNumber, int subGraphID);

    static void createVertices (HGraph* graph, size_t verticesNumber, size_t minEdgesNumber, size_t actualMaxEdgesNumber);
    static void createEdges (HGraph* graph, size_t minVerticesNumber, size_t maxVerticesNumber);

    static size_t connectRandomVertices (HGraph* graph, vector <HVertex*> &temp, size_t remainingConnectionsNumber, const size_t i);
    static void connectRemainingVertices(HGraph* graph, size_t i);

    static vector<vector<int>> prepareConnectionMatrix (HGraph* graph, const size_t nonFullVerticesNumber);
    static vector <size_t> getVerticesNumbers (vector<vector<int>> &connectionMatrix, size_t nonFullVerticesNumber);
    static void optimizeConnectionMatrix (vector<vector<int>> &connectionMatrix, vector<size_t> &verticesNumbers);

public:
    static void generateGraph(HGraph *graph, size_t verticesNumber, size_t minEdgesNumber, size_t actualMaxEdgesNumber,
                          size_t minVerticesNumber, size_t maxVerticesNumber);

    static void randomSplit (HGraph* graph, size_t subGraphsNumber, int startID);
    static void gravitySplit (HGraph* graph, size_t subGraphsNumber, int startID);

    static void resetSplitting(HGraph *graph);
};

#endif // HGRAPHWORKER_H
