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
        for (size_t i=0;i<graph->vertices.size();i++)
            delete graph->vertices[i];

        graph->vertices.clear();
        graph->vertices.resize(verticesNumber);
        graph->setVerticesNumber (verticesNumber);

        for (size_t i=0; i<verticesNumber; i++)
            graph->vertices[i] = new HVertex (i, (minEdgesNumber  +  rand() % (maxEdgesNumber - minEdgesNumber + 1)));
    }
}

void HGraphWorker::createEdges (HGraph* graph, size_t minVerticesNumber, size_t maxVerticesNumber)
{
    // Создание вершин и ребер вызывается
    // только для корневого графа
    if (graph->root)
    {
        size_t summaryDegree = graph->getTotalEdgesNumber();   // Общее число возможных подключений

        // Порог, по прохождении которого
        // включается логика формирования
        // остаточных цепей
        size_t logicalThreshold = maxVerticesNumber*graph->getMaxEdgesNumber();


        // Максимальное число цепей - это
        // вершины * макс. степень / мин. мощность

        for (size_t i=0;i<graph->edges.size();i++)
            delete graph->edges[i];

        graph->edges.clear();
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
            if (logicalThreshold < summaryDegree)
            {
                graph->edges[i] = new HEdge (minVerticesNumber + rand()%(maxVerticesNumber+1));

                do
                {

                    // Номер очередной вершины для включения в ребро
                    size_t nextVertexToConnect = rand()%temp.size();

                    // Установление инцидентности
                    graph->installIncidence(temp[nextVertexToConnect], graph->edges[i]);

                    if(temp[nextVertexToConnect]->isFull())
                    {
                        temp.erase(temp.begin()+nextVertexToConnect);
                    }

                    summaryDegree--;                    // Снижаем число оставшихся для обработки подключений

                } while (!(graph->edges[i]->isFull()));


            }
            else
            {
                // Логика обработки последних подключений
                // Составляю матрицу-проекцию будущих ребер
                size_t countOfFreeVertices = graph->getNonFullVerticesNumber();
                size_t globalMaxDegree = logicalThreshold/maxVerticesNumber;

                vector<vector<int>> connectionMatrix (countOfFreeVertices);

                // 0 - нет контакта, 1 - есть
                // Надо распределить единицы по матрице
                // так, чтобы в строках было минимум
                // количество, равное минимальной мощности
                // ребер
                // Получившиеся строки - прототипы ребер
                for (size_t k=0; k<countOfFreeVertices; k++)
                    connectionMatrix[k].resize(globalMaxDegree+1, 0);


                int elementOfMatrixNumber = 0;
                int cOfVertices = graph->getVerticesNumber();


                for (int k=0; k<cOfVertices; k++)
                {
                    if (!graph->vertices[k]->isFull())
                    {
                        int freePlaces = graph->vertices[k]->getFreePlacesNumber();
                        for(int j=0; j<freePlaces; j++)
                            connectionMatrix[elementOfMatrixNumber][j] = 1;
                        connectionMatrix[elementOfMatrixNumber][globalMaxDegree] =
                                graph->vertices[k]->getID();
                        elementOfMatrixNumber++;
                    }
                }                                     // Матрица готова

                vector<size_t> masOfPowerEdges (globalMaxDegree, 0);
                for (size_t k=0; k<globalMaxDegree; k++)
                {
                    for (size_t j=0; j<countOfFreeVertices; j++)
                        if (connectionMatrix[j][k] == 1)
                            masOfPowerEdges[k]++;
                }

                for (size_t k=0; k<globalMaxDegree; k++)
                {
                    if (masOfPowerEdges[k] < minVerticesNumber)
                    {
                        for (size_t j=0; j<globalMaxDegree; j++)
                        {
                            bool endOfMove = false;
                            if (masOfPowerEdges[j] > minVerticesNumber)
                            {
                                size_t countMovedVertices = masOfPowerEdges[j] - minVerticesNumber;
                                if (countMovedVertices > (minVerticesNumber - masOfPowerEdges[k]))
                                {
                                    countMovedVertices = minVerticesNumber - masOfPowerEdges[k];
                                    endOfMove = true;
                                }
                                int numberMovedVertex = 0;
                                for (size_t kk=0; kk<countMovedVertices; kk++)
                                {
                                    if (connectionMatrix[numberMovedVertex][j] == 1 &&
                                            connectionMatrix[numberMovedVertex][k] == 0)
                                    {
                                        connectionMatrix[numberMovedVertex][j] = 0;
                                        connectionMatrix[numberMovedVertex][k] = 1;
                                        masOfPowerEdges[j]--;
                                        masOfPowerEdges[k]++;
                                    }
                                    else
                                        kk--;
                                    numberMovedVertex++;
                                }
                            }
                            if (endOfMove)
                                break;
                        }
                    }
                }

                for (size_t j=0; j<globalMaxDegree; j++)
                {
                    graph->edges[i] = new HEdge (masOfPowerEdges[j]);
                    // Установление инцидентности
                    for (size_t k=0; k<countOfFreeVertices; k++)
                        if (connectionMatrix[k][j] == 1)
                            graph->installIncidence(graph->vertices[connectionMatrix[k][globalMaxDegree]], graph->edges[i]);
                    i++;

                }
                if (i == 0) throw HGraphException ("Can't create edges: edges number is 0");
                graph->setEdgesNumber (i-1);

                connectionMatrix.clear();
                masOfPowerEdges.clear();
                break;
            }
        }
    }
}

void HGraphWorker::randomSplit (HGraph* graph, size_t subGraphsNumber, int startID)
{
    graph->subGraphsNumber = subGraphsNumber;
    // Мощности подграфов
    vector<size_t> masPowerOfSubHG (subGraphsNumber);

    for (size_t i=0; i<subGraphsNumber; i++)
        masPowerOfSubHG[i] = graph->verticesNumber/subGraphsNumber;

    for (size_t i=0; i<graph->verticesNumber%subGraphsNumber; i++)
        masPowerOfSubHG[i]++;

    for (size_t i=0; i<subGraphsNumber; i++)
        for (size_t j=0; j<masPowerOfSubHG[i]; j++)
        {
            size_t numberNextVertex;
            do
            {
                numberNextVertex = rand()%graph->verticesNumber;
            }
            while(graph->vertices[numberNextVertex]->getGraphID() != graph->ID);
            graph->vertices[numberNextVertex]->setGraphID(i+startID);
        }

    masPowerOfSubHG.clear();
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

    // Число вершин для включения
    // в подграф
    size_t countPlacesInSubHG = subGraphVerticesNumber;

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
                            countPlacesInSubHG--;
                            if (countPlacesInSubHG == 0)
                            {
                                break;                        // Если больше не влезет -
                            }                               // выхожу
                        }

                if (countPlacesInSubHG == 0)
                {
                    break;
                }
            }
        }
        if (!hasAvailableEdge)                   // Когда не нашел подходящих ребер
            minSplitOfEdge++;                   // поднимаем нижнюю границу

        if (!hasAnyAvailableEdge)
        {
           throw HGraphException ("Can't split hypergraph: too few vertices.\nNo one edge is available for splitting."
                                  " Increase the number of vertices or try again.");
        }
    }
    while (countPlacesInSubHG > 0);
}

void HGraphWorker::resetSplitting(HGraph* graph)
{
    for (size_t i=0; i<graph->verticesNumber; i++)
        graph->vertices[i]->setGraphID(-1);
}
