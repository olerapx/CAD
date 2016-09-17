#ifndef HGRAPH_H
#define HGRAPH_H

#include <cmath>
#include <cstdlib>

#include "hgvertex.h"
#include "hgedge.h"
#include "hgraphexception.h"

using namespace std;

/**
 * @brief The HGraph class
 * Гиперграф.
 */
class HGraph
{
private:
    vector<HGVertex*> vertices;                   // Вершины
    vector<HGEdge*>   edges;                      // Гиперребра

    int ID;                               // Номер подграфа (у главного -1)
    int verticesNumber;                          // Число вершин
    int edgesNumber;                             // Число ребер
    int subGraphsNumber;                         // Число подграфов
    bool isRoot;                               // Является ли корневым графом

    void fillSubGraph (vector <HGVertex*> graphVertices, int graphID, vector<HGEdge *> newEdges);

    //Алгоритм "затягивания" ребер в один подграф.
    void gravityEdge (int subGraphVerticesNumber, int subGraphID);

    void createVertices (int verticesNumber, int minEdgesNumber, int maxEdgesNumber);

    //Общее число возможных подключений ребер к вершинам.
    int getSummaryDegree ();

    //Число вершин с возможностью подключения.
    int getCountOfFreeVertices();

    //Максимум возможных подключений ребер к вершинам.
    int getGlobalMaxDegree ();

    void createEdges (int minVerticesNumber, int maxVerticesNumber);

public:
    HGraph ();
    ~HGraph();

    /**
     * @brief HGraph
     * A sub graph constructor.
     */
    HGraph (vector<HGVertex*>& graphVertices, int graphID);

    //Создание подграфа.
    HGraph* createSubHG (int subGraphID);

    void randomSplit (int subGraphsNumber, int startID);
    void gravitySplit (int subGraphsNumber, int startID);

    void resetSplitting();

    // Генератор гиперграфа
    void HGraphGenerator (int CountOfVertices, int minDegree, int maxDegree,
                          int minPowerOfEdge, int maxPowerOfEdge);

    int getID();

    int getCountOfExternalEdges ();

    bool isIncident (HGVertex *vertex, HGEdge *edge);
    bool incidenceInstall (HGVertex * vertex, HGEdge *edge);
    void incidenceUninstall (HGVertex * vertex, HGEdge *edge);

    int getVerticesNumber ();
    bool setVerticesNumber (int number);

    int getEdgesNumber ();
    bool setEdgesNumber (int number);

    int getCountOfFragments();
};

#endif // HGRAPH_H
