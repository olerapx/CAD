#include "hgraphworker.h"

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
            graph->vertices[i] = new HVertex (i, (minEdgesNumber  +  rand() % (maxEdgesNumber - minEdgesNumber + 1)));
    }
}

void HGraphWorker::createEdges (HGraph* graph, size_t minVerticesNumber, size_t maxVerticesNumber)
{
    if (!graph->root) return;

    size_t remainingConnectionsNumber = graph->getTotalEdgesNumber();

    // Порог, по прохождении которого
    // включается логика формирования
    // остаточных цепей
    size_t maxEdgesNumber = graph->getMaxEdgesNumber();
    size_t logicalThreshold = maxVerticesNumber * maxEdgesNumber;

    graph->clearEdges();
    graph->edges.resize(graph->verticesNumber * logicalThreshold/(maxVerticesNumber*minVerticesNumber));

    // to pick random vertice correctly
    vector<HVertex*> temp;
    temp.resize(graph->vertices.size());
    for(size_t i=0; i<temp.size(); i++)
    {
        temp[i] = graph->vertices[i];
    }

    for (size_t i=0; ; i++)        // Начинаем построение ребер (заранее неизвестно, сколько их)
    {
        if (logicalThreshold < remainingConnectionsNumber)
        {
            graph->edges[i] = new HEdge (minVerticesNumber + rand()%(maxVerticesNumber+1));

            do
            {
                size_t nextVertexIndexToConnect = rand()%temp.size();

                graph->installIncidence(temp[nextVertexIndexToConnect], graph->edges[i]);

                if(temp[nextVertexIndexToConnect]->isFull())
                {
                    temp.erase(temp.begin()+nextVertexIndexToConnect);
                }

                remainingConnectionsNumber--;

            } while (!(graph->edges[i]->isFull()));


        }
        else
        {
            size_t nonFullVerticesNumber = graph->getNonFullVerticesNumber();

            // Составляю матрицу-проекцию будущих ребер
            vector<vector<int>> connectionMatrix (nonFullVerticesNumber);

            // 0 - нет контакта, 1 - есть
            // Надо распределить единицы по матрице
            // так, чтобы в строках было минимум
            // количество, равное минимальной мощности
            // ребер
            // Получившиеся строки - прототипы ребер
            for (size_t k=0; k<nonFullVerticesNumber; k++)
                connectionMatrix[k].resize(maxEdgesNumber+1, 0);


            int matrixElementIndex = 0;
            int verticesNumber = graph->getVerticesNumber();


            for (int k=0; k<verticesNumber; k++)
            {
                if (!graph->vertices[k]->isFull())
                {
                    int freePlaces = graph->vertices[k]->getFreePlacesNumber();
                    for(int j=0; j<freePlaces; j++)
                        connectionMatrix[matrixElementIndex][j] = 1;
                    connectionMatrix[matrixElementIndex][maxEdgesNumber] =
                            graph->vertices[k]->getID();
                    matrixElementIndex++;
                }
            }                                     // Матрица готова

            vector<size_t> verticesNumbers (maxEdgesNumber, 0);
            for (size_t k=0; k<maxEdgesNumber; k++)
            {
                for (size_t j=0; j<nonFullVerticesNumber; j++)
                    if (connectionMatrix[j][k] == 1)
                        verticesNumbers[k]++;
            }

            for (size_t k=0; k<maxEdgesNumber; k++)
            {
                if (verticesNumbers[k] < minVerticesNumber)
                {
                    for (size_t j=0; j<maxEdgesNumber; j++)
                    {
                        bool endOfMove = false;
                        if (verticesNumbers[j] > minVerticesNumber)
                        {
                            size_t movedVerticesNumber = verticesNumbers[j] - minVerticesNumber;
                            if (movedVerticesNumber > (minVerticesNumber - verticesNumbers[k]))
                            {
                                movedVerticesNumber = minVerticesNumber - verticesNumbers[k];
                                endOfMove = true;
                            }
                            int movedVertexIndex = 0;
                            for (size_t kk=0; kk<movedVerticesNumber; kk++)
                            {
                                if (connectionMatrix[movedVertexIndex][j] == 1 &&
                                        connectionMatrix[movedVertexIndex][k] == 0)
                                {
                                    connectionMatrix[movedVertexIndex][j] = 0;
                                    connectionMatrix[movedVertexIndex][k] = 1;
                                    verticesNumbers[j]--;
                                    verticesNumbers[k]++;
                                }
                                else
                                    kk--;
                                movedVertexIndex++;
                            }
                        }
                        if (endOfMove)
                            break;
                    }
                }
            }

            for (size_t j=0; j<maxEdgesNumber; j++)
            {
                graph->edges[i] = new HEdge (verticesNumbers[j]);

                for (size_t k=0; k<nonFullVerticesNumber; k++)
                    if (connectionMatrix[k][j] == 1)
                        graph->installIncidence(graph->vertices[connectionMatrix[k][maxEdgesNumber]], graph->edges[i]);
                i++;

            }
            if (i == 0) throw HGraphException ("Can't create edges: edges number is 0");
            graph->setEdgesNumber (i-1);

            connectionMatrix.clear();
            verticesNumbers.clear();
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
    // Мощности подграфов
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
    size_t minSplitOfEdge = 1; // Нижняя граница оценки числа подграфов, в которые входит ребро

    size_t remainingSubGraphPlacesNumber = subGraphVerticesNumber;

    bool hasAvailableEdge = false; // Сигнал для поднятия нижней границы
    do
    {
        hasAvailableEdge = false;
        bool hasAnyAvailableEdge = false;

        for (size_t i=0; i<graph->edgesNumber; i++)
        {
            for (size_t j=0; j<graph->edges[i]->getMaxVerticesNumber(); j++)
                if (graph->edges[i]->getIncidentVertexByIndex(j) != nullptr)
                    if (graph->edges[i]->getIncidentVertexByIndex(j)->getGraphID() == graph->ID)
                        hasAnyAvailableEdge = true;

            // Нашел подходящее ребро
            if (graph->edges[i]->getSubGraphsNumber() <= minSplitOfEdge)
            {
                // Ищу свободные вершины
                for (size_t j=0; j<graph->edges[i]->getMaxVerticesNumber(); j++)
                    if (graph->edges[i]->getIncidentVertexByIndex(j) != nullptr)
                        if (graph->edges[i]->getIncidentVertexByIndex(j)->getGraphID() == graph->ID)
                        {
                            hasAvailableEdge = true;
                            // Включаю в подграф
                            // столько, сколько влезет :)
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
            minSplitOfEdge++;

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
