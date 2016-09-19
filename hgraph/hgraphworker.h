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
    static void dragEdgeInSubGraph (HGraph* graph, size_t subGraphVerticesNumber, int subGraphID);

    static void createVertices (HGraph* graph, size_t verticesNumber, size_t minEdgesNumber, size_t maxEdgesNumber);
    static void createEdges (HGraph* graph, size_t minVerticesNumber, size_t maxVerticesNumber);

public:
    static void generateGraph(HGraph *graph, size_t verticesNumber, size_t minEdgesNumber, size_t maxEdgesNumber,
                          size_t minVerticesNumber, size_t maxVerticesNumber);

    static void randomSplit (HGraph* graph, size_t subGraphsNumber, int startID);
    static void gravitySplit (HGraph* graph, size_t subGraphsNumber, int startID);

    static void resetSplitting(HGraph *graph);
};

#endif // HGRAPHWORKER_H
