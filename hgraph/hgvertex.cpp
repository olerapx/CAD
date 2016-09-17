#include "hgvertex.h"

HGVertex::HGVertex ()
{
    ID = 0;
    maxEdgesNumber = 0;
    full = true;
    graphID = -1;
}

HGVertex::HGVertex (int nV, int maxD)
{
    graphID = -1;
    ID = nV;
    maxEdgesNumber = maxD;
    full = false;

    edges.resize(maxD, nullptr);
}

int HGVertex::freePlaceForConnect ()
{
    if (full)
        return -1;
    else
    {
        int countFree = 0;
        int numberFree = -1;
        for (int i=0; i<maxEdgesNumber; i++)
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

int HGVertex::getGraphID ()
{
    return graphID;
}

void HGVertex::setGraphID (int id)
{
    graphID = id;
}

bool HGVertex::connectEdge (HGEdge *newEdge)
{
    if (full)
        return false;
    else
    {
        int numberFree = freePlaceForConnect();
        if (numberFree > -1)
        {
            edges[numberFree] = newEdge;
            return true;
        }
        else
            return false;
    }
}

void HGVertex::disconnectEdge (HGEdge *oldEdge)
{
    for (int i=0; i<maxEdgesNumber; i++)
        if (edges[i] == oldEdge)
        {
            edges[i] = nullptr;
            if (full)
                full = false;
            return;
        }
}

bool HGVertex::isFull ()
{
    return full;
}

int HGVertex::getID ()
{
    return ID;
}

bool HGVertex::setID (int id)
{
    if (id <= 0)
        return false;
    else
    {
        ID = id;
        return true;
    }
}

int HGVertex::getMaxEdgesNumber ()
{
    return maxEdgesNumber;
}

bool HGVertex::setMaxEdgesNumber (int mD)
{
    if (mD > 0)
    {
        maxEdgesNumber = mD;
        return true;
    }
    else
        return false;
}

bool HGVertex::isInEdge (HGEdge *e)
{
    if (maxEdgesNumber == 0)
        return false;

    for (int i=0; i<maxEdgesNumber; i++)
        if (edges[i] == e)
            return true;

    return false;
}

int HGVertex::getFreePlacesCount ()
{
    if (full)
        return 0;

    int countOfFreePlaces = 0;
    for (int i=0; i<maxEdgesNumber; i++)
        if (edges[i] == nullptr)
            countOfFreePlaces++;

    return countOfFreePlaces;
}

HGEdge * HGVertex::getEdge (int numberOfEdge)
{
    if (maxEdgesNumber <= numberOfEdge)
        return nullptr;
    return edges[numberOfEdge];
}
