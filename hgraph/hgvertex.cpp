#include "hgvertex.h"

HGVertex::HGVertex ()
{
    numberV = 0;
    maxDegree = 0;
    full = true;
    edges = nullptr;
    numberOfHG = -1;
}

HGVertex::HGVertex (int nV, int maxD)
{
    numberOfHG = -1;
    numberV = nV;
    maxDegree = maxD;
    full = false;
    edges = new HGEdge * [maxD];
    for (int i=0; i<maxD; i++)
        edges[i] = nullptr;
}

HGVertex::~HGVertex()
{
    for (int i=0; i<maxDegree; i++)
        edges[i] = nullptr;
}

int HGVertex::freePlaceForConnect ()
{
    if (full)
        return -1;
    else
    {
        int countFree = 0;
        int numberFree = -1;
        for (int i=0; i<maxDegree; i++)
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

int HGVertex::getNumberOfHG ()
{
    return numberOfHG;
}

void HGVertex::setNumberOfHG (int numberSubHG)
{
    numberOfHG = numberSubHG;
}

bool HGVertex::connectVertex (HGEdge *newEdge)
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

void HGVertex::disconnectVertex (HGEdge *oldEdge)
{
    for (int i=0; i<maxDegree; i++)
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

int HGVertex::getNumberV ()
{
    return numberV;
}

bool HGVertex::setNumberV (int nV)
{
    if (nV <= 0)
        return false;
    else
    {
        numberV = nV;
        return true;
    }
}

int HGVertex::getMaxDegree ()
{
    return maxDegree;
}

bool HGVertex::setMaxDegree (int mD)
{
    if (mD > 0)
    {
        maxDegree = mD;
        return true;
    }
    else
        return false;
}

bool HGVertex::isInEdge (HGEdge *e)
{
    if (maxDegree == 0)
        return false;
    for (int i=0; i<maxDegree; i++)
        if (edges[i] == e)
            return true;
    return false;
}

int HGVertex::getCountOfFreePlaces ()
{
    if (full)
        return 0;
    int countOfFreePlaces = 0;
    for (int i=0; i<maxDegree; i++)
        if (edges[i] == nullptr)
            countOfFreePlaces++;
    return countOfFreePlaces;
}

HGEdge * HGVertex::getEdge (int numberOfEdge)
{
    if (maxDegree <= numberOfEdge)
        return nullptr;
    return edges[numberOfEdge];
}
