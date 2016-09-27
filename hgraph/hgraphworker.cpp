#include "hgraphworker.h"

size_t HGraphWorker::minVerticesNumber;
size_t HGraphWorker::maxVerticesNumber;
size_t HGraphWorker::actualMaxEdgesNumber;

void HGraphWorker::generateGraph(HGraph* graph, size_t verticesNumber, size_t minEdgesNumber, size_t maxEdgesNumber,
                      size_t minVerticesNumber, size_t maxVerticesNumber)
{
    createVertices (graph, verticesNumber, minEdgesNumber, maxEdgesNumber);
    createEdges(graph, minVerticesNumber, maxVerticesNumber);
}

void HGraphWorker::createVertices (HGraph* graph, size_t verticesNumber, size_t minEdgesNumber, size_t maxEdgesNumber)
{
    if (graph->root)
    {
        graph->clearVertices();
        graph->vertices.resize(verticesNumber);
        graph->setVerticesNumber (verticesNumber);

        for (size_t i=0; i<verticesNumber; i++)
            graph->vertices[i] = new HVertex (i, (minEdgesNumber + rand()%(maxEdgesNumber - minEdgesNumber + 1)));
    }
}

void HGraphWorker::createEdges (HGraph* graph, size_t minVerticesNumber, size_t maxVerticesNumber)
{
    if (!graph->root) return;

    HGraphWorker::minVerticesNumber = minVerticesNumber;
    HGraphWorker::maxVerticesNumber = maxVerticesNumber;

    HGraphWorker::actualMaxEdgesNumber = graph->getMaxEdgesNumber();

    size_t remainingConnectionsNumber = graph->getTotalEdgesNumber();
    size_t logicalThreshold = maxVerticesNumber * actualMaxEdgesNumber;

    graph->clearEdges();
    graph->edges.resize(graph->verticesNumber * logicalThreshold/(maxVerticesNumber*minVerticesNumber));

    vector<HVertex*> verticesBuffer = prepareVerticesBuffer(graph);

    size_t i = 0;
    while (logicalThreshold < remainingConnectionsNumber)
    {
        remainingConnectionsNumber = connectRandomVertices(graph, verticesBuffer, remainingConnectionsNumber, i);
        i++;
    }

    connectRemainingVertices(graph, i);
}

vector <HVertex*> HGraphWorker::prepareVerticesBuffer (HGraph* graph)
{
    vector<HVertex*> verticesBuffer;

    size_t size = graph->vertices.size();
    verticesBuffer.resize(size);

    for(size_t i=0; i<size; i++)
    {
        verticesBuffer[i] = graph->vertices[i];
    }

    return verticesBuffer;
}

size_t HGraphWorker::connectRandomVertices(HGraph* graph, vector<HVertex *> &verticesBuffer, size_t remainingConnectionsNumber, const size_t i)
{
    graph->edges[i] = new HEdge (minVerticesNumber + rand()%(maxVerticesNumber+1));

    do
    {
        size_t nextVertexIndexToConnect = rand()%verticesBuffer.size();

        graph->installIncidence(verticesBuffer[nextVertexIndexToConnect], graph->edges[i]);

        if(verticesBuffer[nextVertexIndexToConnect]->isFull())
        {
            verticesBuffer.erase(verticesBuffer.begin()+nextVertexIndexToConnect);
        }

        remainingConnectionsNumber--;
    }
    while (!(graph->edges[i]->isFull()));

    return remainingConnectionsNumber;
}

void HGraphWorker::connectRemainingVertices(HGraph* graph, size_t i)
{
    size_t nonFullVerticesNumber = graph->getNonFullVerticesNumber();

    vector<vector<int>> connectionMatrix = prepareConnectionMatrix(graph, nonFullVerticesNumber);
    vector<size_t> verticesNumbers = getVerticesNumbers(connectionMatrix, nonFullVerticesNumber);

    optimizeConnectionMatrix(connectionMatrix, verticesNumbers);

    for (size_t j=0; j<actualMaxEdgesNumber; j++)
    {
        graph->edges[i] = new HEdge (verticesNumbers[j]);

        for (size_t k=0; k<nonFullVerticesNumber; k++)
            if (connectionMatrix[k][j] == 1)
                graph->installIncidence(graph->vertices[connectionMatrix[k][actualMaxEdgesNumber]], graph->edges[i]);
        i++;
    }

    if (i == 0) throw HGraphException ("Can't create edges: edges number is 0");
    graph->setEdgesNumber (i-1);
}

vector<vector<int>> HGraphWorker::prepareConnectionMatrix(HGraph* graph, const size_t nonFullVerticesNumber)
{
    vector<vector<int>> connectionMatrix (nonFullVerticesNumber);

    for (size_t i=0; i<nonFullVerticesNumber; i++)
        connectionMatrix[i].resize(actualMaxEdgesNumber + 1);

    int matrixElementIndex = 0;
    int verticesNumber = graph->getVerticesNumber();

    for (int i=0; i<verticesNumber; i++)
    {
        if (!graph->vertices[i]->isFull())
        {
            int freePlaces = graph->vertices[i]->getFreePlacesNumber();

            for(int j=0; j<freePlaces; j++)
                connectionMatrix[matrixElementIndex][j] = 1;

            connectionMatrix[matrixElementIndex][actualMaxEdgesNumber] = graph->vertices[i]->getID();

            matrixElementIndex++;
        }
    }
    return connectionMatrix;
}

vector<size_t> HGraphWorker::getVerticesNumbers(vector<vector<int> > &connectionMatrix, size_t nonFullVerticesNumber)
{
    vector<size_t> verticesNumbers (actualMaxEdgesNumber, 0);

    for (size_t i=0; i<actualMaxEdgesNumber; i++)
    {
        for (size_t j=0; j<nonFullVerticesNumber; j++)
            if (connectionMatrix[j][i] == 1)
                verticesNumbers[i]++;
    }
    return verticesNumbers;
}

// 0 - нет контакта, 1 - есть
// Надо распределить единицы по матрице
// так, чтобы в строках было минимум
// количество, равное минимальной мощности
// ребер
// Получившиеся строки - прототипы ребер
void HGraphWorker::optimizeConnectionMatrix(vector<vector<int> > &connectionMatrix, vector<size_t> &verticesNumbers)
{
    for (size_t i=0; i<actualMaxEdgesNumber; i++)
    {
        if (verticesNumbers[i] >= minVerticesNumber) continue;

        for (size_t j=0; j<actualMaxEdgesNumber; j++)
        {
            bool isEnd = false;

            if (verticesNumbers[j] > minVerticesNumber)
            {
                size_t movedVerticesNumber = verticesNumbers[j] - minVerticesNumber;
                if (movedVerticesNumber > (minVerticesNumber - verticesNumbers[i]))
                {
                    movedVerticesNumber = minVerticesNumber - verticesNumbers[i];
                    isEnd = true;
                }

                int movedVertexIndex = 0;

                for (size_t k=0; k<movedVerticesNumber; k++)
                {
                    if (connectionMatrix[movedVertexIndex][j] == 1 &&
                        connectionMatrix[movedVertexIndex][i] == 0)
                    {
                        connectionMatrix[movedVertexIndex][j] = 0;
                        connectionMatrix[movedVertexIndex][i] = 1;
                        verticesNumbers[j]--;
                        verticesNumbers[i]++;
                    }
                    else
                        k--;

                    movedVertexIndex++;
                }
            }
            if (isEnd)
                break;
        }
    }
}

void HGraphWorker::randomSplit (HGraph* graph, size_t subGraphsNumber, int startID)
{
    graph->subGraphsNumber = subGraphsNumber;

    vector<size_t> subGraphsVerticesNumbers (subGraphsNumber);

    for (size_t i=0; i<subGraphsNumber; i++)
        subGraphsVerticesNumbers[i] = graph->verticesNumber/subGraphsNumber;

    for (size_t i=0; i<graph->verticesNumber % subGraphsNumber; i++)
        subGraphsVerticesNumbers[i]++;

    for (size_t i=0; i<subGraphsNumber; i++)
        for (size_t j=0; j<subGraphsVerticesNumbers[i]; j++)
        {
            size_t nextVertexIndex;

            do
            {
                nextVertexIndex = rand()%graph->verticesNumber;
            }
            while(graph->vertices[nextVertexIndex]->getGraphID() != graph->ID);

            graph->vertices[nextVertexIndex]->setGraphID(i+startID);
        }

    subGraphsVerticesNumbers.clear();
}

void HGraphWorker::gravitySplit (HGraph* graph, size_t subGraphsNumber, int startID)
{
    graph->subGraphsNumber = subGraphsNumber;

    vector<size_t> subGraphVerticesNumbers (subGraphsNumber);

    for (size_t i=0; i<subGraphsNumber; i++)
        subGraphVerticesNumbers[i] = graph->verticesNumber/subGraphsNumber;

    for (size_t i=0; i<graph->verticesNumber % subGraphsNumber; i++)
        subGraphVerticesNumbers[i]++;

    for (size_t i=0; i<subGraphsNumber; i++)
        dragEdgeInSubGraph(graph, subGraphVerticesNumbers[i], i+startID);

    subGraphVerticesNumbers.clear();
}

void HGraphWorker::dragEdgeInSubGraph (HGraph* graph, size_t subGraphVerticesNumber, int subGraphID)
{
    size_t minEdgeSubGraphsNumber = 1; // Нижняя граница оценки числа подграфов, в которые входит ребро
    size_t remainingSubGraphPlacesNumber = subGraphVerticesNumber;

    bool hasAvailableEdge = false; // Сигнал для поднятия нижней границы

    do
    {
        hasAvailableEdge = false;
        bool hasAnyAvailableEdge = false;

        for (size_t i=0; i<graph->edgesNumber; i++)
        {
            for (size_t j=0; j<graph->edges[i]->getMaxVerticesNumber(); j++)
            {
                if (graph->edges[i]->getIncidentVertexByIndex(j) == nullptr) continue;

                if (graph->edges[i]->getIncidentVertexByIndex(j)->getGraphID() == graph->ID)
                    hasAnyAvailableEdge = true;
            }

            if (graph->edges[i]->getSubGraphsNumber() <= minEdgeSubGraphsNumber)
            {
                for (size_t j=0; j<graph->edges[i]->getMaxVerticesNumber(); j++)
                {
                    if (graph->edges[i]->getIncidentVertexByIndex(j) == nullptr) continue;
                    if (graph->edges[i]->getIncidentVertexByIndex(j)->getGraphID() != graph->ID) continue;

                    hasAvailableEdge = true;

                    // Включаю в подграф столько, сколько влезет :)
                    graph->edges[i]->getIncidentVertexByIndex(j)->setGraphID(subGraphID);
                    remainingSubGraphPlacesNumber--;

                    if (remainingSubGraphPlacesNumber == 0)
                    {
                        break;
                    }
                }

                if (remainingSubGraphPlacesNumber == 0)
                {
                    break;
                }
            }
        }
        if (!hasAvailableEdge)
            minEdgeSubGraphsNumber++;

        if (!hasAnyAvailableEdge)
        {
           throw HGraphException ("Can't split hypergraph: too few vertices.\nNo one edge is available for splitting."
                                  " Increase the number of vertices or try again.");
        }
    }
    while (remainingSubGraphPlacesNumber > 0);
}

void HGraphWorker::resetSplitting(HGraph* graph)
{
    for (size_t i=0; i<graph->verticesNumber; i++)
        graph->vertices[i]->setGraphID(-1);
}
