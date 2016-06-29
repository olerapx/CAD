#ifndef HGRAPH_H
#define HGRAPH_H

#include <cmath>
#include <cstdlib>
#include <stdint.h>

#include "hgvertex.h"
#include "hgedge.h"

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

    int numberOfHG;                               // Номер подграфа (у главного -1)
    int countOfVertices;                          // Число вершин
    int countOfEdges;                             // Число ребер
    int splitResultCount;                         // Число подграфов
    bool mainGraph;                               // Является ли корневым графом

    //Алгоритм "затягивания" ребер в один подграф.
    void gravityEdge (int powerOfSubHG, int numberOfSubHG);

    void createVertices (int countVertices, int minDegree, int maxDegree);

    //Общее число возможных подключений ребер к вершинам.
    int getSummaryDegree ();

    //Число вершин с возможностью подключения.
    int getCountOfFreeVertices ();

    //Максимум возможных подключений ребер к вершинам.
    int getGlobalMaxDegree ();

    void createEdges (int minCountOfVertices, int maxCountOfVertices);

public:
    HGraph ();
    ~HGraph();

    //Конструктор подграфа.
    HGraph (vector<HGVertex*>& masOfVertex, int numberSubG);

    //Создание подграфа.
    HGraph* createSubHG (int numberOfSubHG);


    // Случайное разбиение
    void randomSplitHG (int countSubHG, int startNumberSubG);

    // Последовательное разбиение
    void gravitySplitHG (int countSubHG, int startNumberSubG);

    // Убрать разбиение
    void resetSplitHG ();

    // Генератор гиперграфа
    void HGraphGenerator (int CountOfVertices, int minDegree, int maxDegree,
                          int minPowerOfEdge, int maxPowerOfEdge);

    // Геттер номера подграфа
    int getNumberOfHG ();

    // Геттер числа внешних связей
    int getCountOfExternalEdges ();


    // Проверка инцидентности вершины и ребра
    bool isIncident (HGVertex *vertex, HGEdge *edge);

    // Установление инцидентности вершины и ребра
    bool incidenceInstall (HGVertex * vertex, HGEdge *edge);

    // Устранение инцидентности вершины и ребра
    void incidenceUninstall (HGVertex * vertex, HGEdge *edge);

    // Геттер числа вершин
    int getCountOfVertices ();

    // Сеттер числа вершин
    bool setCountOfVertices (int cV);

    // Геттер числа гиперребер
    int getCountOfEdges ();

    // Геттер числа ребер
    int getCountOfFragments();

    // Сеттер числа гиперребер
    bool setCountOfEdges (int cE);

};

#endif // HGRAPH_H
