#include "hedge.h"

HEdge::HEdge()
{
    maxVerticesNumber = 0;
    full = true;
}

HEdge::HEdge (int maxVertexCount)
{
    maxVerticesNumber = maxVertexCount;
    full = false;

    vertices.resize(maxVertexCount);
}

int HEdge::getFreePlacesNumber()
{
    if (full)
        return 0;
    int countOfFreePlaces = 0;

    for (int i=0; i<maxVerticesNumber; i++)
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
        for (int i=0; i<maxVerticesNumber; i++)
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
    for (int i=0; i<maxVerticesNumber; i++)
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

bool HEdge::setMaxVerticesNumber (int number)
{
    if (number >= 0)
    {
        maxVerticesNumber = number;
        return true;
    }
    else
        return false;
}

int HEdge::getMaxVerticesNumber ()
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
    for (int i=0; i<maxVerticesNumber; i++)
        if (vertices[i] == vertex)
            return true;
    return false;
}

int HEdge::getCommonVerticesNumber (HEdge *otherEdge)
{
    int countOfCommonVertices = 0;
    for (int i=0; i<getMaxVerticesNumber(); i++)
        if (vertices[i] != nullptr)
            if (otherEdge->isInEdge(vertices[i]))
                countOfCommonVertices++;
    return countOfCommonVertices;
}

int HEdge::getSubGraphsNumber ()
{
    int countSubHG = 1;
    for (int i=0; i<maxVerticesNumber; i++)
        if (vertices[i] != nullptr && vertices[i]->getGraphID() != -1)
            for (int j=0; j<i; j++)
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
