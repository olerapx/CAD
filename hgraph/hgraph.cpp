#include "hgraph.h"

HGraph::HGraph ()
{
    verticesNumber = 0;
    edgesNumber = 0;
    subGraphsNumber = 0;
    isRoot = true;
    ID = -1;
}

HGraph::HGraph (vector<HVertex*> &graphVertices, int graphID)
{
    subGraphsNumber = 0;
    isRoot = false;
    ID = graphID;

    int subGraphVerticesCount = 0;
    int subGraphEdgesCount = 0;

    for (size_t i=0; i<graphVertices.size(); i++)
    {
        // Считаю число вершин, входящих
        // в подграф, и максимальное
        // число инцидентных им ребер
        if (graphVertices[i]->getGraphID() == graphID)
        {
            subGraphVerticesCount++;
            subGraphEdgesCount += graphVertices[i]->getMaxEdgesNumber() - graphVertices[i]->getFreePlacesNumber();
        }
    }

    vertices.resize(subGraphVerticesCount);
    verticesNumber = subGraphVerticesCount;

    // Промежуточный массив под ребра
    // для устранения дублей
    vector<HEdge*> newEdges;
    newEdges.resize(subGraphEdgesCount);

    fillSubGraph(graphVertices, graphID, newEdges);
}

void HGraph::fillSubGraph (vector <HVertex*> graphVertices, int graphID, vector <HEdge*> newEdges)
{
    int subGraphVerticesCount = 0;
    int subGraphEdgesCount = 0;

    for (size_t i=0; i<graphVertices.size(); i++)
    {
        if (graphVertices[i]->getGraphID() == graphID)
        {
            vertices[subGraphVerticesCount] = graphVertices[i];
            subGraphVerticesCount++;

            for (int j=0; j<graphVertices[i]->getMaxEdgesNumber(); j++)
            {
                if (graphVertices[i]->getIncidentEdgeByIndex(j)!= nullptr)
                {
                    bool newEdge = true;
                    // Если такое ребро уже есть,
                    // то включать уже не надо
                    for (int k=0; k<subGraphEdgesCount; k++)
                        if (newEdges[k] == graphVertices[i]->getIncidentEdgeByIndex(j))
                        {
                            newEdge = false;
                            break;
                        }
                    if (newEdge)
                    {
                        newEdges[subGraphEdgesCount] = graphVertices[i]->getIncidentEdgeByIndex(j);
                        subGraphEdgesCount++;
                    }
                }
            }
        }
    }
    edgesNumber = subGraphEdgesCount-1;

    if (edgesNumber < 0)
    {
            edgesNumber = 0; /// dangerous
            verticesNumber = 0;
            vertices.clear();
    }

    edges.resize(edgesNumber);

    for (int i=0; i<(subGraphEdgesCount-1); i++)
    {
        edges[i] = newEdges[i];
    }
}

HGraph::~HGraph()
{
    if (isRoot)
    {
        for (size_t i=0;i<edges.size();i++)
            delete edges[i];

        for (size_t i=0;i<vertices.size();i++)
            delete vertices[i];
    }
}

void HGraph::createVertices (int verticesNumber, int minEdgesNumber, int maxEdgesNumber)
{
    if (isRoot)
    {
        for (size_t i=0;i<vertices.size();i++)
            delete vertices[i];

        vertices.clear();
        vertices.resize(verticesNumber);
        setVerticesNumber (verticesNumber);

        for (int i=0; i<verticesNumber; i++)
            vertices[i] = new HVertex (i, (minEdgesNumber  +  rand() % (maxEdgesNumber - minEdgesNumber + 1)));
    }
}

int HGraph::getTotalEdgesNumber ()
{
    int summaryDegree = 0;
    int count = getVerticesNumber();

    for (int i=0; i<count; i++)
        summaryDegree += vertices[i]->getMaxEdgesNumber();

    return summaryDegree;
}

int HGraph::getNonFullVerticesNumber ()
{
    int countOfFreeVertices = 0;
    int cOfVertices = getVerticesNumber();

    for (int i=0; i<cOfVertices; i++)
        if (!vertices[i]->isFull())
            countOfFreeVertices++;
    return countOfFreeVertices;
}

int HGraph::getMaxEdgesNumber ()
{
    int globalMaxDegree = 0;
    int cOfVertices = getVerticesNumber();

    for (int i=0; i<cOfVertices; i++)
        if (globalMaxDegree < vertices[i]->getMaxEdgesNumber())
            globalMaxDegree = vertices[i]->getMaxEdgesNumber();

    return globalMaxDegree;
}

void HGraph::createEdges (int minVerticesNumber, int maxVerticesNumber)
{
    // Создание вершин и ребер вызывается
    // только для корневого графа
    if (isRoot)
    {
        int summaryDegree = getTotalEdgesNumber();   // Общее число возможных подключений

        // Порог, по прохождении которого
        // включается логика формирования
        // остаточных цепей
        int logicalThreshold = maxVerticesNumber*getMaxEdgesNumber();


        // Максимальное число цепей - это
        // вершины * макс. степень / мин. мощность

        for (size_t i=0;i<edges.size();i++)
            delete edges[i];

        edges.clear();
        edges.resize(verticesNumber * logicalThreshold/(maxVerticesNumber*minVerticesNumber));

        // to pick random vertice correctly
        vector<HVertex*> temp;
        temp.resize(vertices.size());
        for(size_t i=0; i<temp.size(); i++)
        {
            temp[i] = vertices[i];
        }

        for (int i=0; ; i++)        // Начинаем построение ребер (заранее неизвестно, сколько их)
        {
            if (logicalThreshold < summaryDegree)
            {
                edges[i] = new HEdge (minVerticesNumber + rand()%(maxVerticesNumber+1));

                do
                {

                    // Номер очередной вершины для включения в ребро
                    int nextVertexToConnect = rand()%temp.size();

                    // Установление инцидентности
                    installIncidence(temp[nextVertexToConnect], edges[i]);

                    if(temp[nextVertexToConnect]->isFull())
                    {
                        temp.erase(temp.begin()+nextVertexToConnect);
                    }

                    summaryDegree--;                    // Снижаем число оставшихся для обработки подключений

                } while (!(edges[i]->isFull()));


            }
            else
            {
                // Логика обработки последних подключений
                // Составляю матрицу-проекцию будущих ребер
                int countOfFreeVertices = getNonFullVerticesNumber();
                int globalMaxDegree = logicalThreshold/maxVerticesNumber;

                vector<vector<int>> connectionMatrix (countOfFreeVertices);

                // 0 - нет контакта, 1 - есть
                // Надо распределить единицы по матрице
                // так, чтобы в строках было минимум
                // количество, равное минимальной мощности
                // ребер
                // Получившиеся строки - прототипы ребер
                for (int k=0; k<countOfFreeVertices; k++)
                    connectionMatrix[k].resize(globalMaxDegree+1, 0);


                int elementOfMatrixNumber = 0;
                int cOfVertices = getVerticesNumber();


                for (int k=0; k<cOfVertices; k++)
                {
                    if (!vertices[k]->isFull())
                    {
                        int freePlaces = vertices[k]->getFreePlacesNumber();
                        for(int j=0; j<freePlaces; j++)
                            connectionMatrix[elementOfMatrixNumber][j] = 1;
                        connectionMatrix[elementOfMatrixNumber][globalMaxDegree] =
                                vertices[k]->getID();
                        elementOfMatrixNumber++;
                    }
                }                                     // Матрица готова

                vector<int> masOfPowerEdges (globalMaxDegree, 0);
                for (int k=0; k<globalMaxDegree; k++)
                {
                    for (int j=0; j<countOfFreeVertices; j++)
                        if (connectionMatrix[j][k] == 1)
                            masOfPowerEdges[k]++;
                }

                for (int k=0; k<globalMaxDegree; k++)
                {
                    if (masOfPowerEdges[k] < minVerticesNumber)
                    {
                        for (int j=0; j<globalMaxDegree; j++)
                        {
                            bool endOfMove = false;
                            if (masOfPowerEdges[j] > minVerticesNumber)
                            {
                                int countMovedVertices = masOfPowerEdges[j] - minVerticesNumber;
                                if (countMovedVertices > (minVerticesNumber - masOfPowerEdges[k]))
                                {
                                    countMovedVertices = minVerticesNumber - masOfPowerEdges[k];
                                    endOfMove = true;
                                }
                                int numberMovedVertex = 0;
                                for (int kk=0; kk<countMovedVertices; kk++)
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

                for (int j=0; j<globalMaxDegree; j++)
                {

                    edges[i] = new HEdge (masOfPowerEdges[j]);
                    // Установление инцидентности
                    for (int k=0; k<countOfFreeVertices; k++)
                        if (connectionMatrix[k][j] == 1)
                            installIncidence(vertices[connectionMatrix[k][globalMaxDegree]], edges[i]);
                    i++;

                }
                setEdgesNumber (i-1);

                connectionMatrix.clear();
                masOfPowerEdges.clear();
                break;
            }
        }
    }
}

int HGraph::getExternalEdgesNumber ()
{
    int countOfExternalEdges = 0;

    for (int i=0; i<edgesNumber; i++)
        countOfExternalEdges += edges[i]->getSubGraphsNumber() - 1;
    countOfExternalEdges = countOfExternalEdges/2;

    return countOfExternalEdges;
}

bool HGraph::isIncident (HVertex *vertex, HEdge *edge)
{
    if (vertex->isInEdge(edge) && edge->isInEdge(vertex))
        return true;
    else
        return false;
}

bool HGraph::installIncidence (HVertex * vertex, HEdge *edge)
{
    if (!vertex->isFull() && !edge->isFull())
        if (vertex->tryConnectEdge(edge))
        {
            if (edge->tryConnectVertex(vertex)) return true;

            vertex->disconnectEdge(edge);
        }
    return false;
}

void HGraph::uninstallIncidence (HVertex * vertex, HEdge *edge)
{
    vertex->disconnectEdge(edge);
    edge->disconnectVertex(vertex);
}

int HGraph::getID ()
{
    return ID;
}

int HGraph::getVerticesNumber ()
{
    return verticesNumber;
}

bool HGraph::setVerticesNumber (int number)
{
    if (number > 0)
    {
        verticesNumber = number;
        return true;
    }
    else
        return false;
}

int HGraph::getEdgesNumber ()
{
    return edgesNumber;
}

int HGraph::getFragmentsNumber ()
{
    int countOfFragments = 0;
    for (int i=0; i<getEdgesNumber(); i++)
        countOfFragments += edges[i]->getMaxVerticesNumber()-1;

    return countOfFragments;
}

bool HGraph::setEdgesNumber (int number)
{
    if (number > 0)
    {
        edgesNumber = number;
        return true;
    }
    else
        return false;
}

void HGraph::HGraphGenerator (int verticesNumber, int minEdgesNumber, int maxEdgesNumber,
                              int minVerticesNumber, int maxVerticesNumber)
{
    createVertices (verticesNumber, minEdgesNumber, maxEdgesNumber);
    createEdges(minVerticesNumber, maxVerticesNumber);
}

void HGraph::randomSplit (int subGraphsNumber, int startID)
{
    this->subGraphsNumber = subGraphsNumber;
    // Мощности подграфов
    vector<int> masPowerOfSubHG (subGraphsNumber);

    for (int i=0; i<subGraphsNumber; i++)
        masPowerOfSubHG[i] = verticesNumber/subGraphsNumber;

    for (int i=0; i<verticesNumber%subGraphsNumber; i++)
        masPowerOfSubHG[i]++;

    for (int i=0; i<subGraphsNumber; i++)
        for (int j=0; j<masPowerOfSubHG[i]; j++)
        {
            int numberNextVertex;
            do
            {
                numberNextVertex = rand()%verticesNumber;
            }
            while(vertices[numberNextVertex]->getGraphID() != ID);
            vertices[numberNextVertex]->setGraphID(i+startID);
        }

    masPowerOfSubHG.clear();
}

void HGraph::gravitySplit (int subGraphsNumber, int startID)
{
    this->subGraphsNumber = subGraphsNumber;
    // Мощности подграфов
    vector<int> subGraphVerticesNumbers (subGraphsNumber);

    for (int i=0; i<subGraphsNumber; i++)
        subGraphVerticesNumbers[i] = verticesNumber/subGraphsNumber;

    for (int i=0; i<verticesNumber % subGraphsNumber; i++)
        subGraphVerticesNumbers[i]++;

    for (int i=0; i<subGraphsNumber; i++)
        dragEdgeInSubGraph(subGraphVerticesNumbers[i], i+startID);

    subGraphVerticesNumbers.clear();
}

void HGraph::dragEdgeInSubGraph (int subGraphVerticesNumber, int subGraphID)
{
    int minSplitOfEdge = 1; // Нижняя граница оценки числа подграфов, в которые входит ребро

    // Число вершин для включения
    // в подграф
    int countPlacesInSubHG = subGraphVerticesNumber;

    bool hasAvailableEdge = false; // Сигнал для поднятия нижней границы
    do
    {
        hasAvailableEdge = false;
        bool hasAnyAvailableEdge = false;

        for (size_t i=0; i<edgesNumber; i++)
        {
            for (size_t j=0; j<edges[i]->getMaxVerticesNumber(); j++)
                if (edges[i]->getIncidentVertexByIndex(j) != nullptr)
                    if (edges[i]->getIncidentVertexByIndex(j)->getGraphID() == ID)
                        hasAnyAvailableEdge = true;

            // Нашел подходящее ребро
            if (edges[i]->getSubGraphsNumber() <= minSplitOfEdge)
            {
                // Ищу свободные вершины
                for (int j=0; j<edges[i]->getMaxVerticesNumber(); j++)
                    if (edges[i]->getIncidentVertexByIndex(j) != nullptr)
                        if (edges[i]->getIncidentVertexByIndex(j)->getGraphID() == ID)
                        {
                            hasAvailableEdge = true;
                            // Включаю в подграф
                            // столько, сколько влезет :)
                            edges[i]->getIncidentVertexByIndex(j)->setGraphID(subGraphID);
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


HGraph *HGraph::createSubHG(int subGraphID)
{
    if (subGraphID >= 0)
        return new HGraph(vertices, subGraphID);
    else
        throw HGraphException ("Hypergraph ID cannot be lesser than 0.");
}

void HGraph::resetSplitting ()
{
    for (int i=0; i<verticesNumber; i++)
        vertices[i]->setGraphID(-1);
}
