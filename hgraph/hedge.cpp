#include "hedge.h"

HEdge::HEdge()
{
    maxVerticesNumber = 0;
    full = true;
}

HEdge::HEdge (size_t maxVerticesNumber)
{
    this->maxVerticesNumber = maxVerticesNumber;
    full = false;

    vertices.resize(maxVerticesNumber);
}

size_t HEdge::getFreePlacesNumber()
{
    if (full)
        return 0;
    size_t countOfFreePlaces = 0;

    for (size_t i=0; i<maxVerticesNumber; i++)
        if (vertices[i] == nullptr)
            countOfFreePlaces++;
    return countOfFreePlaces;
}

size_t HEdge::getFreePlaceIndex()
{
    if (full)
        return -1;
    else
    {
        size_t countFree = 0;
        size_t numberFree = -1;
        for (size_t i=0; i<maxVerticesNumber; i++)
            if (vertices[i] == nullptr)
            {
                numberFree = i;
                countFree++;
            }
        if(countFree == 1)                        // Состояние изменяется, т.к. данный метод вызывается
            full = true;                            // непосредственно перед включением вершины в ребро,
        return numberFree;                        // и значит, единственное свободное место будет ей
    }                                           // занято, а ребро станет заполненным
}

void HEdge::disconnectVertex (HVertex *oldVertex)
{
    for (size_t i=0; i<maxVerticesNumber; i++)
        if (vertices[i] == oldVertex)
        {
            vertices[i] = nullptr;
            if (full)
                full = false;
            return;
        }
}

bool HEdge::tryConnectVertex (HVertex *newVertex)
{
    if (full)
        return false;
    else
    {
        int numberFree = getFreePlaceIndex ();
        if (numberFree > -1)
        {
            vertices[numberFree] = newVertex;
            return true;
        }
        else
            return false;
    }
}

bool HEdge::isFull ()
{
    return full;
}

void HEdge::setMaxVerticesNumber (size_t number)
{
    maxVerticesNumber = number;
}

size_t HEdge::getMaxVerticesNumber()
{
    return maxVerticesNumber;
}

HVertex* HEdge::getIncidentVertexByIndex (size_t index)
{
    if (maxVerticesNumber <= index)
        return nullptr;
    return vertices[index];
}

bool HEdge::isInEdge (HVertex *vertex)
{
    if (maxVerticesNumber == 0)
        return false;
    for (size_t i=0; i<maxVerticesNumber; i++)
        if (vertices[i] == vertex)
            return true;
    return false;
}

size_t HEdge::getCommonVerticesNumber(HEdge *otherEdge)
{
    size_t countOfCommonVertices = 0;
    for (size_t i=0; i<getMaxVerticesNumber(); i++)
        if (vertices[i] != nullptr)
            if (otherEdge->isInEdge(vertices[i]))
                countOfCommonVertices++;
    return countOfCommonVertices;
}

size_t HEdge::getSubGraphsNumber()
{
    size_t countSubHG = 1;
    for (size_t i=0; i<maxVerticesNumber; i++)
        if (vertices[i] != nullptr && vertices[i]->getGraphID() != -1)
            for (size_t j=0; j<i; j++)
            {
                if (vertices[j]!= nullptr)
                    if (vertices[j]->getGraphID() ==
                            vertices[i]->getGraphID())
                        break;
                if (j == i-1)
                    countSubHG++;
            }
    return countSubHG;
}
