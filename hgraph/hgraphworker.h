#ifndef HGRAPHWORKER_H
#define HGRAPHWORKER_H

#include <QObject>
#include <QPointF>

#include "hgraph.h"

/**
 * @brief The HGraphWorker class
 * Represents the hypergraph generating and splitting algorithms.
 */
class HGraphWorker: public QObject
{
    Q_OBJECT

private:
    size_t verticesNumber;
    size_t minVerticesNumber, maxVerticesNumber;
    size_t minEdgesNumber, maxEdgesNumber;
    size_t actualMaxEdgesNumber;

    size_t subGraphsNumber;
    double deploymentComplexity, tracingComplexity;

    size_t experimentNumber;

    bool stopped;
    bool actuallyStopped;

    /* single-level */
    vector<HGraph*> hGraph;
    size_t minSubGraphsNumber;

    /* hierarchical */
    vector<vector<vector<HGraph*>>> hGraphHierarchy;
    vector<vector<size_t>> externalEdgesNumberIncreasing;

    vector<vector<size_t>> splittingNumbers;
    int levelNumber;

    void calculateData(size_t index);

    void initGraphHierarchy(size_t index);
    size_t getNumberOfComputersOnLevel(size_t index, size_t level);
    void copyGraphToHierarchy();

    void gatheringData(size_t index);
    void showData(size_t index);

    void clear();

    void dragEdgeInSubGraph (HGraph* graph, size_t subGraphVerticesNumber, int subGraphID);

    void createVertices (HGraph* graph);
    void createEdges (HGraph* graph);

    vector <HVertex*> prepareVerticesBuffer (HGraph* graph);

    size_t connectRandomVertices (HGraph* graph, vector <HVertex*> &verticesBuffer, size_t remainingConnectionsNumber, size_t i);
    void connectRemainingVertices(HGraph* graph, size_t i);

    vector<vector<int>> prepareConnectionMatrix (HGraph* graph, const size_t nonFullVerticesNumber);
    vector <size_t> getVerticesNumbers (vector<vector<int>> &connectionMatrix, size_t nonFullVerticesNumber);
    void optimizeConnectionMatrix (vector<vector<int>> &connectionMatrix, vector<size_t> &verticesNumbers);

    void generateGraph(HGraph *graph);

    void randomSplit(HGraph* graph, size_t subGraphsNumber, int startID);
    void gravitySplit(HGraph* graph, size_t subGraphsNumber, int startID);

    void resetSplitting(HGraph *graph);

signals:
    void sendGenerated();
    void sendRandomCalculated();
    void sendSeriesCalculated();
    void sendHierarchicalCalculated();
    void sendPrintHierarchicalData(uint i);

    void sendStopped();

    void sendCreateNewSeries(uint index);
    void sendSetMaxProgress(int value);

    void sendStatus(QString status);
    void sendProgress(int progress);
    void sendAddProgress(int progress);

    void sendEdgesAppend(QPointF point);
    void sendStepsAppend(QPointF point);
    void sendSingleStepsAppend(double value);

    void sendError(QString error);

public slots:
    void onGenerate();

    void onCalculateRandom();
    void onCalculateSeries();
    void onCalculateHierarchical();

    void onStopped();

public:
    HGraphWorker();

    bool isStopped() { return actuallyStopped; }

    void setGenerationParameters(size_t experimentNumber, size_t verticesNumber, size_t minEdgesNumber, size_t maxEdgesNumber,
                       size_t minVerticesNumber, size_t maxVerticesNumber);

    void setRandomParameters(size_t subGraphsNumber);
    void setSeriesParameters(size_t tracingComplexity, size_t deploymentComplexity, size_t subGraphsNumber);
    void setHierarchicalParameters(vector<vector<size_t>> splittingNumbers, size_t tracingComplexity, size_t deploymentComplexity, size_t levelNumber);
};

#endif // HGRAPHWORKER_H
