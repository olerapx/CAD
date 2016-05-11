#include "hgraph.h"

#include <iostream>

HGraph::HGraph ()
{
  countOfVertices = 0;
  countOfEdges = 0;
  splitResultCount = 0;
  mainGraph = true;
  numberOfHG = -1;
}

HGraph::HGraph (vector<HGVertex *> &masOfVertex, int numberSubG)
{
  splitResultCount = 0;
  mainGraph = false;
  numberOfHG = numberSubG;

  int countVerticesOfSubG = 0;
  int countEdgesOfSubG = 0;
  for (int i=0; i<masOfVertex.size(); i++)           // Считаю число вершин, входящих
                                              // в подграф, и максимальное
                                              // число инцидентных им ребер
    if (masOfVertex[i]->getNumberOfHG()==numberSubG)
    {
      countVerticesOfSubG++;
      countEdgesOfSubG += masOfVertex[i]->getMaxDegree()
                              - masOfVertex[i]->getCountOfFreePlaces();
    }

  vertices.resize(countVerticesOfSubG);

  countOfVertices = countVerticesOfSubG;

                                              // Промежуточный массив под ребра
                                              // для устранения дублей
  vector<HGEdge*> newEdges;
  newEdges.resize(countEdgesOfSubG);
  countVerticesOfSubG = 0;
  countEdgesOfSubG = 0;

  for (int i=0; i<masOfVertex.size(); i++)
    if (masOfVertex[i]->getNumberOfHG()==numberSubG)
    {
                                              // Делаю собственные ссылки на
                                              // вершины у подграфа
      vertices[countVerticesOfSubG] = masOfVertex[i];
      countVerticesOfSubG++;
      for (int j=0; j<masOfVertex[i]->getMaxDegree(); j++)
        if (masOfVertex[i]->getEdge(j)!= nullptr)
        {
          bool newEdge = true;
                                              // Если такое ребро уже есть,
                                              // то включать уже не надо
          for (int k=0; k<countEdgesOfSubG; k++)
            if (newEdges[k] == masOfVertex[i]->getEdge(j))
            {
              newEdge = false;
              break;
            }
          if (newEdge)
          {
            newEdges[countEdgesOfSubG] = masOfVertex[i]->getEdge(j);
            countEdgesOfSubG++;
          }
        }
    }
                                              // Окончательное формирование
                                              // массива ссылок на ребра
                                              // подграфа
  edges.resize(countEdgesOfSubG-1);

  countOfEdges = countEdgesOfSubG-1;
  for (int i=0; i<(countEdgesOfSubG-1); i++)
  {
    edges[i] = newEdges[i];
    newEdges[i] = nullptr;
  }
}

HGraph::~HGraph()
{
    if (!mainGraph) return;

    for (int i=0;i<edges.size();i++)
    {
        for (int j=0;j<vertices.size();j++)
        {
            if (edges[i]!=nullptr && vertices[j]!= nullptr)
                edges[i]->disconnectVertex(vertices[j]);
        }
    }
    vertices.clear();
    edges.clear();
}

void HGraph::createVertices (int countVertices, int minDegree, int maxDegree)
{
    if (mainGraph)                              // Создание вершин и ребер вызывается
    {                                           // только для корневого графа
        for (int i=0;i<vertices.size();i++)
            delete vertices[i];

        vertices.clear();
        vertices.resize(countVertices);
        setCountOfVertices (countVertices);
        for (int i=0; i<countVertices; i++)
            vertices[i] = new HGVertex (i,(minDegree+rand()%(maxDegree-minDegree+1)));
    }
}

int HGraph::getSummaryDegree ()
{
    int summaryDegree = 0;
    int cOfVertices = getCountOfVertices();
    for (int i=0; i<cOfVertices; i++)
        summaryDegree += vertices[i]->getMaxDegree();
    return summaryDegree;
}

int HGraph::getCountOfFreeVertices ()
{
    int countOfFreeVertices = 0;
    int cOfVertices = getCountOfVertices();

    for (int i=0; i<cOfVertices; i++)
        if (!vertices[i]->isFull())
            countOfFreeVertices++;
    return countOfFreeVertices;
}

int HGraph::getGlobalMaxDegree ()
{
    int globalMaxDegree = 0;
    int cOfVertices = getCountOfVertices();
    for (int i=0; i<cOfVertices; i++)
        if (globalMaxDegree < vertices[i]->getMaxDegree())
            globalMaxDegree = vertices[i]->getMaxDegree();
    return globalMaxDegree;
}

void HGraph::createEdges (int minCountOfVertices, int maxCountOfVertices)
{
    if (mainGraph)                              // Создание вершин и ребер вызывается
    {                                           // только для корневого графа
        int summaryDegree = getSummaryDegree();   // Общее число возможных подключений

        // Порог, по прохождении которого
        // включается логика формирования
        // остаточных цепей
        int logicalThreshold = maxCountOfVertices*getGlobalMaxDegree();


        // Максимальное число цепей - это
        // вершины * макс. степень / мин. мощность

        for (int i=0;i<edges.size();i++)
            delete edges[i];

        edges.clear();
        edges.resize(countOfVertices*logicalThreshold/(maxCountOfVertices*minCountOfVertices));

        for (int i=0; ; i++)        // Начинаем построение ребер (заранее неизвестно, сколько их)
            if (logicalThreshold < summaryDegree)
            {
                edges[i] = new HGEdge (minCountOfVertices + rand()%(maxCountOfVertices+1));
                do{
                    int nextVertexToConnect;            // Номер очередной вершины для включения в ребро
                    do
                        nextVertexToConnect = rand()%getCountOfVertices();
                    while (vertices[nextVertexToConnect]->isInEdge(edges[i]) ||
                           vertices[nextVertexToConnect]->isFull());

                    // Установление инцидентности
                    incidenceInstall(vertices[nextVertexToConnect], edges[i]);

                    summaryDegree--;                    // Снижаем число оставшихся для
                    // обработки подключений

                }
                while (!edges[i]->isFull());
            }
            else
            {
                // Логика обработки последних подключений
                // Составляю матрицу-проекцию будущих ребер
                int countOfFreeVertices = getCountOfFreeVertices();
                int globalMaxDegree = logicalThreshold/maxCountOfVertices;
                int ** connectionMatrix = new int * [countOfFreeVertices];

                // 0 - нет контакта, 1 - есть
                // Надо распределить единицы по матрице
                // так, чтобы в строках было минимум
                // количество, равное минимальной мощности
                // ребер
                // Получившиеся строки - прототипы ребер
                for (int k=0; k<countOfFreeVertices; k++)
                {
                    connectionMatrix[k] = new int [globalMaxDegree+1];
                    for (int j=0; j<globalMaxDegree; j++)
                        connectionMatrix[k][j] = 0;
                }


                int elementOfMatrixNumber = 0;
                int cOfVertices = getCountOfVertices();


                for (int k=0; k<cOfVertices; k++)
                {
                    if (!vertices[k]->isFull())
                    {
                        int freePlaces = vertices[k]->getCountOfFreePlaces();
                        for(int j=0; j<freePlaces; j++)
                            connectionMatrix[elementOfMatrixNumber][j] = 1;
                        connectionMatrix[elementOfMatrixNumber][globalMaxDegree] =
                                vertices[k]->getNumberV();
                        elementOfMatrixNumber++;
                    }
                }                                     // Матрица готова

                int *masOfPowerEdges = new int [globalMaxDegree];
                for (int k=0; k<globalMaxDegree; k++)
                {
                    masOfPowerEdges[k] = 0;
                   for (int j=0; j<countOfFreeVertices; j++)
                        if (connectionMatrix[j][k] == 1)
                            masOfPowerEdges[k]++;
                }

                for (int k=0; k<globalMaxDegree; k++)
                {
                    if (masOfPowerEdges[k] < minCountOfVertices)
                    {
                        for (int j=0; j<globalMaxDegree; j++)
                        {
                            bool endOfMove = false;
                            if (masOfPowerEdges[j] > minCountOfVertices)
                            {
                                int countMovedVertices = masOfPowerEdges[j] - minCountOfVertices;
                                if (countMovedVertices > (minCountOfVertices - masOfPowerEdges[k]))
                                {
                                    countMovedVertices = minCountOfVertices - masOfPowerEdges[k];
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

                    edges[i] = new HGEdge (masOfPowerEdges[j]);
                    // Установление инцидентности
                    for (int k=0; k<countOfFreeVertices; k++)
                        if (connectionMatrix[k][j] == 1)
                            incidenceInstall(vertices[connectionMatrix[k][globalMaxDegree]], edges[i]);
                    i++;

                }
                setCountOfEdges (i-1);
                delete [] masOfPowerEdges;
                delete [] connectionMatrix;
                break;
            }
    }
}

int HGraph::getCountOfExternalEdges ()
{
    int countOfExternalEdges = 0;

    for (int i=0; i<countOfEdges; i++)
        countOfExternalEdges += edges[i]->getCountSubHG() -1;
    countOfExternalEdges = countOfExternalEdges/2;

    return countOfExternalEdges;
}

bool HGraph::isIncident (HGVertex *vertex, HGEdge *edge)
{
    if (vertex->isInEdge(edge) && edge->isInEdge(vertex))
        return true;
    else
        return false;
}

bool HGraph::incidenceInstall (HGVertex * vertex, HGEdge *edge)
{
    if (!vertex->isFull() && !edge->isFull())
        if (vertex->connectEdge(edge))
        {
            if (edge->connectVertex(vertex)) return true;

            vertex->disconnectEdge(edge);
        }
    return false;
}

void HGraph::incidenceUninstall (HGVertex * vertex, HGEdge *edge)
{
//    vertex->disconnectEdge(edge);
    edge->disconnectVertex(vertex);
}

int HGraph::getNumberOfHG ()
{
    return numberOfHG;
}

int HGraph::getCountOfVertices ()
{
    return countOfVertices;
}

bool HGraph::setCountOfVertices (int cV)
{
    if (cV > 0)
    {
        countOfVertices = cV;
        return true;
    }
    else
        return false;
}

int HGraph::getCountOfEdges ()
{
    return countOfEdges;
}

int HGraph::getCountOfFragments ()
{
    int countOfFragments = 0;
    for (int i=0; i<getCountOfEdges(); i++)
        countOfFragments += edges[i]->getMaxCountOfVertex()-1;

    return countOfFragments;
}

bool HGraph::setCountOfEdges (int cE)
{
    if (cE > 0)
    {
        countOfEdges = cE;
        return true;
    }
    else
        return false;
}

void HGraph::HGraphGenerator (int CountOfVertices, int minDegree, int maxDegree,
                              int minPowerOfEdge, int maxPowerOfEdge)
{
    createVertices (CountOfVertices, minDegree, maxDegree);
    createEdges(minPowerOfEdge, maxPowerOfEdge);
}

void HGraph::gravityEdge (int powerOfSubHG, int numberOfSubHG)
{
    int minSplitOfEdge = 1;                     // Нижняя граница оценки
    // числа подграфов, в которые
    // входит ребро

    // Число вершин для включения
    // в подграф
    int countPlacesInSubHG = powerOfSubHG;
    bool noCorrectEdges;                        // Сигнал для поднятия нижней
    // границы
    do
    {
        noCorrectEdges = true;
        for (int i=0; i<countOfEdges; i++)
        {
            // Нашел подходящее ребро
            if (edges[i]->getCountSubHG() <= minSplitOfEdge)
            {
                // Ищу свободные вершины
                for (int j=0; j<edges[i]->getMaxCountOfVertex(); j++)
                    if (edges[i]->getVertex(j) != nullptr)
                        if (edges[i]->getVertex(j)->getNumberOfHG() == numberOfHG)
                        {
                            noCorrectEdges = false;
                            // Включаю в подграф
                            // столько, сколько влезет :)
                            edges[i]->getVertex(j)->setNumberOfHG(numberOfSubHG);
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
        if (noCorrectEdges)                   // Когда не нашел подходящих ребер
            minSplitOfEdge++;                   // поднимаем нижнюю границу
    }
    while (countPlacesInSubHG > 0);
}

void HGraph::randomSplitHG (int countSubHG, int startNumberSubG)
{
    splitResultCount = countSubHG;
    // Мощности подграфов
    int *masPowerOfSubHG = new int [countSubHG];
    for (int i=0; i<countSubHG; i++)
        masPowerOfSubHG[i] = countOfVertices/countSubHG;
    for (int i=0; i<countOfVertices%countSubHG; i++)
        masPowerOfSubHG[i]++;

    for (int i=0; i<countSubHG; i++)
        for (int j=0; j<masPowerOfSubHG[i]; j++)
        {
            int numberNextVertex;
            do
            {
                numberNextVertex = rand()%countOfVertices;
            }
            while(vertices[numberNextVertex]->getNumberOfHG() != numberOfHG);
            vertices[numberNextVertex]->setNumberOfHG(i+startNumberSubG);
        }
}

void HGraph::gravitySplitHG (int countSubHG, int startNumberSubG)
{
    splitResultCount = countSubHG;
    // Мощности подграфов
    int *masPowerOfSubHG = new int [countSubHG];
    for (int i=0; i<countSubHG; i++)
        masPowerOfSubHG[i] = countOfVertices/countSubHG;
    for (int i=0; i<countOfVertices%countSubHG; i++)
        masPowerOfSubHG[i]++;

    for (int i=0; i<countSubHG; i++)
        gravityEdge(masPowerOfSubHG[i], i+startNumberSubG);
}

HGraph * HGraph::createSubHG (int numberOfSubHG)
{
    if (numberOfSubHG >= 0)
    {
        HGraph * subHG = new HGraph(vertices, numberOfSubHG);
        return subHG;
    }
    else
        return nullptr;
}

void HGraph::resetSplitHG ()
{
    for (int i=0; i<countOfVertices; i++)
        vertices[i]->setNumberOfHG(-1);
}
