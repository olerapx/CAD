#include "hvertex.h"

HVertex::HVertex ()
{
    ID = 0;
    maxEdgesNumber = 0;
    full = true;
    graphID = -1;
}

HVertex::HVertex (int id, size_t maxEdgesNumber)
{
    graphID = -1;
    ID = id;
    this->maxEdgesNumber = maxEdgesNumber;
    full = false;

    edges.resize(maxEdgesNumber, nullptr);
}

size_t HVertex::getFreePlaceIndex()
{
    if (full)
        return -1;
    else
    {
        size_t countFree = 0;
        size_t numberFree = -1;
        for (size_t i=0; i<maxEdgesNumber; i++)
            if (edges[i] == nullptr)
            {
                numberFree = i;
                countFree++;
            }
        if(countFree == 1)                        // Состояние изменяется, т.к. данный метод вызывается
            full = true;                            // непосредственно перед включением вершины в ребро,
        return numberFree;                        // и значит, единственное свободное место будет им
    }                                           // занято, а вершина станет заполненной
}

int HVertex::getGraphID ()
{
    return graphID;
}

void HVertex::setGraphID (int id)
{
    graphID = id;
}

bool HVertex::tryConnectEdge (HEdge *newEdge)
{
    if (full)
        return false;
    else
    {
        int numberFree = getFreePlaceIndex();
        if (numberFree > -1)
        {
            edges[numberFree] = newEdge;
            return true;
        }
        else
            return false;
    }
}

void HVertex::disconnectEdge (HEdge *oldEdge)
{
    for (size_t i=0; i<maxEdgesNumber; i++)
        if (edges[i] == oldEdge)
        {
            edges[i] = nullptr;
            if (full)
                full = false;
            return;
        }
}

bool HVertex::isFull ()
{
    return full;
}

int HVertex::getID ()
{
    return ID;
}

bool HVertex::setID (int id)
{
    if (id <= 0)
        return false;
    else
    {
        ID = id;
        return true;
    }
}

size_t HVertex::getMaxEdgesNumber()
{
    return maxEdgesNumber;
}

void HVertex::setMaxEdgesNumber (size_t number)
{
    maxEdgesNumber = number;
}

bool HVertex::isInEdge (HEdge *edge)
{
    if (maxEdgesNumber == 0)
        return false;

    for (size_t i=0; i<maxEdgesNumber; i++)
        if (edges[i] == edge)
            return true;

    return false;
}

size_t HVertex::getFreePlacesNumber()
{
    if (full)
        return 0;

    size_t countOfFreePlaces = 0;
    for (size_t i=0; i<maxEdgesNumber; i++)
        if (edges[i] == nullptr)
            countOfFreePlaces++;

    return countOfFreePlaces;
}

HEdge * HVertex::getIncidentEdgeByIndex (size_t index)
{
    if (maxEdgesNumber <= index)
        return nullptr;
    return edges[index];
}
