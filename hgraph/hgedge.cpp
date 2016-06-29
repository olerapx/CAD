#include "hgedge.h"

HGEdge::HGEdge()
{
    maxCountOfVertex = 0;
    full = true;
}

HGEdge::HGEdge (int maxVertexCount)
{
    maxCountOfVertex = maxVertexCount;
    full = false;

    vertices.resize(maxVertexCount);
}

int HGEdge::getCountOfFreePlaces()
{
    if (full)
        return 0;
    int countOfFreePlaces = 0;

    for (int i=0; i<maxCountOfVertex; i++)
        if (vertices[i] == nullptr)
            countOfFreePlaces++;
    return countOfFreePlaces;
}

int HGEdge::freePlaceForConnect ()
{
    if (full)
        return -1;
    else
    {
        int countFree = 0;
        int numberFree = -1;
        for (int i=0; i<maxCountOfVertex; i++)
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

void HGEdge::disconnectVertex (HGVertex *oldVertex)
{
    for (int i=0; i<maxCountOfVertex; i++)
        if (vertices[i] == oldVertex)
        {
            vertices[i] = nullptr;
            if (full)
                full = false;
            return;
        }
}

bool HGEdge::connectVertex (HGVertex *newVertex)
{
    if (full)
        return false;
    else
    {
        int numberFree = freePlaceForConnect ();
        if (numberFree > -1)
        {
            vertices[numberFree] = newVertex;
            return true;
        }
        else
            return false;
    }
}

bool HGEdge::isFull ()
{
    return full;
}

bool HGEdge::setMaxCountOfVertex (int maxCV)
{
    if (maxCV >= 0)
    {
        maxCountOfVertex = maxCV;
        return true;
    }
    else
        return false;
}

int HGEdge::getMaxCountOfVertex ()
{
    return maxCountOfVertex;
}

HGVertex* HGEdge::getVertex (int number)
{
    if (maxCountOfVertex <= number)
        return nullptr;
    return vertices[number];
}

bool HGEdge::isInEdge (HGVertex *v)
{
    if (maxCountOfVertex == 0)
        return false;
    for (int i=0; i<maxCountOfVertex; i++)
        if (vertices[i] == v)
            return true;
    return false;
}

int HGEdge::getCountOfCommonVertices (HGEdge *otherEdge)
{
    int countOfCommonVertices = 0;
    for (int i=0; i<getMaxCountOfVertex(); i++)
        if (vertices[i] != nullptr)
            if (otherEdge->isInEdge(vertices[i]))
                countOfCommonVertices++;
    return countOfCommonVertices;
}

int HGEdge::getCountSubHG ()
{
    int countSubHG = 1;
    for (int i=0; i<maxCountOfVertex; i++)
        if (vertices[i] != nullptr && vertices[i]->getNumberOfHG() != -1)
            for (int j=0; j<i; j++)
            {
                if (vertices[j]!= nullptr)
                    if (vertices[j]->getNumberOfHG() ==
                            vertices[i]->getNumberOfHG())
                        break;
                if (j == i-1)
                    countSubHG++;
            }
    return countSubHG;
}
