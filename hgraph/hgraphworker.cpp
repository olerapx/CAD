#include "hgraphworker.h"

HGraphWorker::HGraphWorker()
{
    stopped = true;
    actuallyStopped = true;
}

void HGraphWorker::setGenerationParameters(size_t experimentNumber, size_t verticesNumber, size_t minEdgesNumber, size_t maxEdgesNumber,
                   size_t minVerticesNumber, size_t maxVerticesNumber)
{
    this->experimentNumber = experimentNumber;

    this->verticesNumber = verticesNumber;

    this->minEdgesNumber = minEdgesNumber;
    this->maxEdgesNumber = maxEdgesNumber;

    this->minVerticesNumber = minVerticesNumber;
    this->maxVerticesNumber = maxVerticesNumber;
}

void HGraphWorker::setRandomParameters(size_t subGraphsNumber)
{
    this->subGraphsNumber = subGraphsNumber;
}

void HGraphWorker::setSeriesParameters(size_t tracingComplexity, size_t deploymentComplexity, size_t subGraphsNumber)
{
    this->tracingComplexity = tracingComplexity;
    this->deploymentComplexity = deploymentComplexity;
    this->subGraphsNumber = subGraphsNumber;
}

void HGraphWorker::setHierarchicalParameters(vector<vector<size_t>> splittingNumbers, size_t tracingComplexity, size_t deploymentComplexity, size_t levelNumber)
{
    this->splittingNumbers = splittingNumbers;
    this->tracingComplexity = tracingComplexity;
    this->deploymentComplexity = deploymentComplexity;
    this->levelNumber = levelNumber;
}

void HGraphWorker::onGenerate()
{
    if(!actuallyStopped)
        return;

    stopped = false;
    actuallyStopped = false;

    this->experimentNumber = experimentNumber;

    this->verticesNumber = verticesNumber;

    this->minEdgesNumber = minEdgesNumber;
    this->maxEdgesNumber = maxEdgesNumber;

    this->minVerticesNumber = minVerticesNumber;
    this->maxVerticesNumber = maxVerticesNumber;

    try
    {
        if(experimentNumber < 1)
            throw HGraphException("Число экспериментов не может быть < 1.");

        clear();

        for (size_t i=0; i<experimentNumber; i++)
        {
            hGraph[i] = new HGraph;
            generateGraph(hGraph[i]);            

            if(stopped)
            {
                actuallyStopped = true;
                sendStopped();
                return;
            }
        }

        sendGenerated();
        stopped = true;
        actuallyStopped = true;
    }
    catch(HGraphException ex)
    {
        sendError(QString::fromStdString(ex.getError()));
    }
}

void HGraphWorker::clear()
{
    hGraph.clear();
    hGraph.resize(experimentNumber);

    for (size_t i=0; i<hGraphHierarchy.size(); i++)
        for (size_t j=0; j<hGraphHierarchy[i].size(); j++)
            for (size_t k=0; k<hGraphHierarchy[i][j].size(); k++)
                delete hGraphHierarchy[i][j][k];

    hGraphHierarchy.clear();
}

void HGraphWorker::generateGraph(HGraph* graph)
{
    createVertices(graph);
    createEdges(graph);
}

void HGraphWorker::createVertices (HGraph* graph)
{
    if(stopped) return;

    if (graph->root)
    {
        graph->clearVertices();
        graph->vertices.resize(verticesNumber);
        graph->setVerticesNumber (verticesNumber);

        for (size_t i=0; i<verticesNumber; i++)
        {
            if(stopped) return;
            graph->vertices[i] = new HVertex (i, (minEdgesNumber + rand()%(maxEdgesNumber - minEdgesNumber + 1)));
        }
    }
}

void HGraphWorker::createEdges (HGraph* graph)
{
    if(stopped) return;

    if (!graph->root) return;

    this->actualMaxEdgesNumber = graph->getMaxEdgesNumber();

    size_t remainingConnectionsNumber = graph->getTotalEdgesNumber();
    size_t logicalThreshold = maxVerticesNumber * actualMaxEdgesNumber;

    graph->clearEdges();
    graph->edges.resize(graph->verticesNumber * logicalThreshold/(maxVerticesNumber*minVerticesNumber));

    vector<HVertex*> verticesBuffer = prepareVerticesBuffer(graph);

    size_t i = 0;
    while (logicalThreshold < remainingConnectionsNumber)
    {
        if(stopped) return;
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

size_t HGraphWorker::connectRandomVertices(HGraph* graph, vector<HVertex *> &verticesBuffer, size_t remainingConnectionsNumber, size_t i)
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
        {
            if(stopped) return;

            if (connectionMatrix[k][j] == 1)
                graph->installIncidence(graph->vertices[connectionMatrix[k][actualMaxEdgesNumber]], graph->edges[i]);
        }
        i++;
    }

    if (i == 0) throw HGraphException ("Невозможно создать ребра: число ребер равно 0");
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
    if(stopped) return;

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

void HGraphWorker::onCalculateRandom()
{
    if(!actuallyStopped)
        return;

    stopped = false;
    actuallyStopped = false;

    try
    {
        for (size_t j=2; j<=subGraphsNumber; j++)
        {
            sendStatus(tr("Осталось частей: %1").arg(QString::number(subGraphsNumber-j)));

            size_t totalFragmentsNumber = 0;
            size_t totalExternalEdgesNumber = 0;

            for (size_t i=0; i<experimentNumber; i++)
            {
                resetSplitting(hGraph[i]);
                randomSplit(hGraph[i], j, 0);
                totalFragmentsNumber += hGraph[i]->getFragmentsNumber();
                totalExternalEdgesNumber += hGraph[i]->getExternalEdgesNumber();

                sendProgress(j*experimentNumber+1);

                if(stopped)
                {
                    actuallyStopped = true;
                    sendStopped();
                    return;
                }
            }

            sendEdgesAppend(QPointF(j, 100.0 * (double)totalExternalEdgesNumber/(double)totalFragmentsNumber));
        }

        sendProgress(experimentNumber * 2);
        sendRandomCalculated();

        stopped = true;
        actuallyStopped = true;
    }
    catch(HGraphException ex)
    {
        sendError(QString::fromStdString(ex.getError()));
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
            if(stopped) return;

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

void HGraphWorker::onCalculateSeries()
{
    if(!actuallyStopped)
        return;

    stopped = false;
    actuallyStopped = false;

    try
    {
        double countOfAllFragments = 0;

        for (size_t i=0; i<experimentNumber; i++)
            countOfAllFragments += hGraph[i]->getFragmentsNumber();
        countOfAllFragments /= experimentNumber;

        sendStepsAppend(QPointF(1, pow((double)hGraph[0]->getVerticesNumber(), deploymentComplexity) + pow(countOfAllFragments, tracingComplexity)));

        for (size_t j=2; j<=subGraphsNumber; j++)
        {
            double countOfAllExternalEdges = 0;

            sendStatus(tr("Осталось частей: %1").arg(QString::number(subGraphsNumber-j)));

            for (size_t i=0; i<experimentNumber; i++)
            {
                resetSplitting(hGraph[i]);
                gravitySplit(hGraph[i], j, 0);
                countOfAllExternalEdges += hGraph[i]->getExternalEdgesNumber();

                sendProgress(j*experimentNumber+1);

                if(stopped)
                {
                    actuallyStopped = true;
                    sendStopped();
                    return;
                }
            }
            countOfAllExternalEdges /= experimentNumber;

            sendEdgesAppend(QPointF(j, 100* (countOfAllExternalEdges/countOfAllFragments)));
            sendStepsAppend(QPointF(j, hGraph[0]->getVerticesNumber() +
                            pow((double)hGraph[0]->getVerticesNumber()/j, deploymentComplexity) +
                            pow(countOfAllExternalEdges,tracingComplexity) +
                            pow((countOfAllFragments - countOfAllExternalEdges)/j, tracingComplexity)));
        }

        sendProgress(experimentNumber * 2);
        sendSeriesCalculated();

        stopped = true;
        actuallyStopped = true;
    }
    catch(HGraphException ex)
    {
        sendError(QString::fromStdString(ex.getError()));
    }
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
    {
       if(stopped) return;
       dragEdgeInSubGraph(graph, subGraphVerticesNumbers[i], i+startID);
    }

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
                if(stopped) return;

                if (graph->edges[i]->getIncidentVertexByIndex(j) == nullptr) continue;

                if (graph->edges[i]->getIncidentVertexByIndex(j)->getGraphID() == graph->ID)
                    hasAnyAvailableEdge = true;
            }

            if (graph->edges[i]->getSubGraphsNumber() <= minEdgeSubGraphsNumber)
            {
                for (size_t j=0; j<graph->edges[i]->getMaxVerticesNumber(); j++)
                {
                    if(stopped) return;

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
           throw HGraphException ("Невозможно разбить гиперграф: слишком мало вершин.\nНет ребер, доступных для разбиения."
                                  " Увеличьте число вершин или попробуйте еще раз.");
        }
    }
    while (remainingSubGraphPlacesNumber > 0);
}

void HGraphWorker::resetSplitting(HGraph* graph)
{
    for (size_t i=0; i<graph->verticesNumber; i++)
        graph->vertices[i]->setGraphID(-1);
}

void HGraphWorker::onCalculateHierarchical()
{
    if(!actuallyStopped)
        return;

    stopped = false;
    actuallyStopped = false;

    this->splittingNumbers = splittingNumbers;
    this->tracingComplexity = tracingComplexity;
    this->deploymentComplexity = deploymentComplexity;
    this->levelNumber = levelNumber;

    try
    {
        for (size_t i=0; i< splittingNumbers.size(); i++)
        {
            calculateData(i);

            if(stopped)
            {
                actuallyStopped = true;
                sendStopped();
                return;
            }
        }

        sendHierarchicalCalculated();

        stopped = true;
        actuallyStopped = true;
    }
    catch(HGraphException ex)
    {
        sendError(QString::fromStdString(ex.getError()));
    }
}

void HGraphWorker::calculateData(size_t index)
{
    sendCreateNewSeries(index);

    initGraphHierarchy(index);
    gatheringData(index);

    if(stopped) return;

    double countAllFragments = 0.0;
    for (size_t i=0; i<experimentNumber; i++)
        countAllFragments += hGraphHierarchy[0][0][i]->getFragmentsNumber();
    countAllFragments /= experimentNumber;

    double nextIncreaseValue = 0.0;

    for (int i=0; i<levelNumber; i++)
    {
        size_t numberOfComputersOnLevel =  getNumberOfComputersOnLevel(index, i);
        for (size_t j=0; j<numberOfComputersOnLevel; j++)
            nextIncreaseValue += externalEdgesNumberIncreasing[i][j];

        sendEdgesAppend(QPointF(i+1, 100 * nextIncreaseValue/countAllFragments));
    }

    showData(index);
}

void HGraphWorker::initGraphHierarchy(size_t index)
{
    minSubGraphsNumber = 0;

    externalEdgesNumberIncreasing.clear();
    externalEdgesNumberIncreasing.resize(levelNumber);

    for (size_t i=0; i<hGraphHierarchy.size(); i++)
        for (size_t j=0; j<hGraphHierarchy[i].size(); j++)
            for (size_t k=0; k<hGraphHierarchy[i][j].size(); k++)
                if (i || j)
                    delete hGraphHierarchy[i][j][k];

    hGraphHierarchy.clear();
    hGraphHierarchy.resize(levelNumber);

    for (int i=0; i<levelNumber; i++)
    {
        int numberOfComputersOnLevel = getNumberOfComputersOnLevel(index, i);

        hGraphHierarchy[i].resize(numberOfComputersOnLevel);

        for (int j=0; j<numberOfComputersOnLevel; j++)
            hGraphHierarchy[i][j].resize(experimentNumber);

        externalEdgesNumberIncreasing[i].resize(numberOfComputersOnLevel);
    }

    sendProgress(0);

    copyGraphToHierarchy();
}

size_t HGraphWorker::getNumberOfComputersOnLevel(size_t index, size_t level)
{
    size_t res = 1;
    for (size_t i=0; i< level; i++)
        res*=splittingNumbers[index][i];

    return res;
}

void HGraphWorker::copyGraphToHierarchy()
{
    for (size_t i=0; i<experimentNumber; i++)
    {
        resetSplitting(hGraph[i]);
        hGraphHierarchy[0][0][i] = hGraph[i];

        sendProgress(i);
    }
}

void HGraphWorker::gatheringData(size_t index)
{
    int max = 0;
    for (int i=0; i<levelNumber; i++)
        max += getNumberOfComputersOnLevel(index, i) * experimentNumber;

    sendSetMaxProgress(max);
    sendProgress(0);

    int countOldExternalEdges = 0;

    for (int i=0; i<levelNumber; i++)
    {
        int numberOfComputersOnLevel = getNumberOfComputersOnLevel(index, i);

        sendStatus(tr("Осталось уровней: %1").arg(QString::number(levelNumber-i)));

        for (int j=0; j<numberOfComputersOnLevel; j++)
        {
            externalEdgesNumberIncreasing[i][j] = 0;

            for (size_t k=0; k<experimentNumber; k++)
            {
                if(stopped) return;

                gravitySplit(hGraphHierarchy[i][j][k], splittingNumbers[index][i], minSubGraphsNumber);

                externalEdgesNumberIncreasing[i][j] += hGraphHierarchy[0][0][k]->getExternalEdgesNumber();

                if (i < levelNumber-1) // Если не последняя итерация - создаю подграфы на основе разбиений
                {
                    for (size_t l=0; l<splittingNumbers[index][i]; l++)
                        hGraphHierarchy[i+1][j*splittingNumbers[index][i] + l][k] = hGraphHierarchy[i][j][k]->createSubGraph(minSubGraphsNumber + l);
                }
            }

            externalEdgesNumberIncreasing[i][j] /= experimentNumber;

            // Вычитаю уже имевшиеся связи
            // для получения прироста
            externalEdgesNumberIncreasing[i][j] -= countOldExternalEdges;
            countOldExternalEdges += externalEdgesNumberIncreasing[i][j];

            minSubGraphsNumber += splittingNumbers[index][i];

            sendAddProgress(experimentNumber);
        }
    }
}

void HGraphWorker::showData(size_t index)
{
    if(stopped) return;

    double currentCostOfTracing = 0;
    double currentCountOfInternalEdges = 0;

    for (size_t i=0; i<experimentNumber; i++)
        currentCountOfInternalEdges += hGraphHierarchy[0][0][i]->getFragmentsNumber();
    currentCountOfInternalEdges /= experimentNumber;

    sendStepsAppend(QPointF(0, pow((double)currentCountOfInternalEdges, tracingComplexity)+
                           pow(verticesNumber, deploymentComplexity)));

    for (int i=0; i<levelNumber; i++)
    {
        size_t maxCountExternalEdges = 0;
        int numberOfComputersOnLevel = getNumberOfComputersOnLevel(index, i);

        for (int j=0; j< numberOfComputersOnLevel; j++)
        {
            if(stopped) return;

            if (maxCountExternalEdges < externalEdgesNumberIncreasing[i][j])
                maxCountExternalEdges = externalEdgesNumberIncreasing[i][j];

            currentCountOfInternalEdges -= externalEdgesNumberIncreasing[i][j];
        }

        currentCostOfTracing += pow((double)maxCountExternalEdges, tracingComplexity);

        double nextValue = hGraphHierarchy[i][0][0]->getVerticesNumber() +
                pow((double)hGraphHierarchy[i][0][0]->getVerticesNumber()/splittingNumbers[index][i], deploymentComplexity) +
                currentCostOfTracing +
                pow (currentCountOfInternalEdges / getNumberOfComputersOnLevel(index, i+1),
                     tracingComplexity);

        sendStepsAppend(QPointF(i+1, nextValue));
    }

    sendPrintHierarchicalData(index);
}

void HGraphWorker::onStopped()
{
    this->stopped = true;
}
