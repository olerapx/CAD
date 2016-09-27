#include "hgraph.h"

HGraph::HGraph ()
{
    verticesNumber = 0;
    edgesNumber = 0;
    subGraphsNumber = 0;
    root = true;
    ID = -1;
}

HGraph::~HGraph()
{
    clearVertices();
    clearEdges();
}

HGraph::HGraph (vector<HVertex*>& graphVertices, int graphID)
{
    subGraphsNumber = 0;
    root = false;
    ID = graphID;

    int subGraphVerticesCount = 0;
    int subGraphEdgesCount = 0;

    for (size_t i=0; i<graphVertices.size(); i++)
    {
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
    size_t subGraphVerticesCount = 0;
    size_t subGraphEdgesCount = 0;

    for (size_t i=0; i<graphVertices.size(); i++)
    {
        if (graphVertices[i]->getGraphID() != graphID) continue;

        vertices[subGraphVerticesCount] = graphVertices[i];
        subGraphVerticesCount++;

        for (size_t j=0; j<graphVertices[i]->getMaxEdgesNumber(); j++)
        {
            if (graphVertices[i]->getIncidentEdgeByIndex(j) == nullptr) continue;

            bool isNewEdge = true;

            for (size_t k=0; k<subGraphEdgesCount; k++)
            {
                if (newEdges[k] != graphVertices[i]->getIncidentEdgeByIndex(j)) continue;

                isNewEdge = false;
                break;
            }

            if (isNewEdge)
            {
                newEdges[subGraphEdgesCount] = graphVertices[i]->getIncidentEdgeByIndex(j);
                subGraphEdgesCount++;
            }
        }
    }

    if (subGraphEdgesCount == 0)
    {
            edgesNumber = 0; /// dangerous
            verticesNumber = 0;
            vertices.clear();
    }

    edgesNumber = subGraphEdgesCount-1;
    edges.resize(edgesNumber);

    for (size_t i=0; i<(subGraphEdgesCount-1); i++)
    {
        edges[i] = newEdges[i];
    }
}

int HGraph::getID ()
{
    return ID;
}

size_t HGraph::getVerticesNumber()
{
    return verticesNumber;
}

void HGraph::setVerticesNumber (size_t number)
{
    verticesNumber = number;
}

size_t HGraph::getEdgesNumber()
{
    return edgesNumber;
}

void HGraph::setEdgesNumber (size_t number)
{
    edgesNumber = number;
}


size_t HGraph::getFragmentsNumber()
{
    size_t countOfFragments = 0;
    for (size_t i=0; i<getEdgesNumber(); i++)
        countOfFragments += edges[i]->getMaxVerticesNumber()-1;

    return countOfFragments;
}

size_t HGraph::getExternalEdgesNumber()
{
    size_t countOfExternalEdges = 0;

    for (size_t i=0; i<edgesNumber; i++)
        countOfExternalEdges += edges[i]->getSubGraphsNumber() - 1;
    countOfExternalEdges = countOfExternalEdges/2;

    return countOfExternalEdges;
}

bool HGraph::isRoot()
{
    return root;
}

bool HGraph::isIncident (HVertex* vertex, HEdge* edge)
{
    if (vertex->isInEdge(edge) && edge->isInEdge(vertex))
        return true;
    else
        return false;
}

bool HGraph::installIncidence (HVertex* vertex, HEdge* edge)
{
    if (!vertex->isFull() && !edge->isFull())
        if (vertex->tryConnectEdge(edge))
        {
            if (edge->tryConnectVertex(vertex)) return true;

            vertex->disconnectEdge(edge);
        }
    return false;
}

void HGraph::uninstallIncidence (HVertex* vertex, HEdge* edge)
{
    vertex->disconnectEdge(edge);
    edge->disconnectVertex(vertex);
}

size_t HGraph::getTotalEdgesNumber()
{
    size_t summaryDegree = 0;
    size_t count = getVerticesNumber();

    for (size_t i=0; i<count; i++)
        summaryDegree += vertices[i]->getMaxEdgesNumber();

    return summaryDegree;
}

size_t HGraph::getNonFullVerticesNumber()
{
    size_t countOfFreeVertices = 0;
    size_t verticesNumber = getVerticesNumber();

    for (size_t i=0; i<verticesNumber; i++)
        if (!vertices[i]->isFull())
            countOfFreeVertices++;
    return countOfFreeVertices;
}

size_t HGraph::getMaxEdgesNumber()
{
    size_t maxEdgesNumber = 0;
    size_t verticesNumber = getVerticesNumber();

    for (size_t i=0; i<verticesNumber; i++)
        if (maxEdgesNumber < vertices[i]->getMaxEdgesNumber())
            maxEdgesNumber = vertices[i]->getMaxEdgesNumber();

    return maxEdgesNumber;
}

void HGraph::clearVertices()
{
    if (root)
    {
        size_t size = vertices.size();
        for (size_t i=0; i<size; i++)
            delete vertices[i];
    }

    vertices.clear();
    verticesNumber = 0;
}

void HGraph::clearEdges()
{
    if (root)
    {
        size_t size = edges.size();
        for (size_t i=0; i<size; i++)
            delete edges[i];
    }

    edges.clear();
    edgesNumber = 0;
}

HGraph* HGraph::createSubGraph(int subGraphID)
{
    if (subGraphID >= 0)
        return new HGraph(vertices, subGraphID);
    else
        throw HGraphException ("Hypergraph ID cannot be lesser than 0.");
}
