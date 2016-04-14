//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
  randomize ();
  hGraph = NULL;
  hierarhyHG = NULL;
  increaseOfCountExternalEdges = NULL;
}
//---------------------------------------------------------------------------
  int HGEdge::freePlaceForConnect ()            // Поиск свободного места и изменение
  {                                             // состояния, если такого нет
    if (full)
      return -1;
    else
    {
      int countFree = 0;
      int numberFree = -1;
      for (int i=0; i<maxCountOfVertex; i++)
        if (vertices[i] == NULL)
        {
          numberFree = i;
          countFree++;
        }
      if(countFree == 1)                        // Состояние изменяется, т.к. данный метод вызывается
        full = true;                            // непосредственно перед включением вершины в ребро,
      return numberFree;                        // и значит, единственное свободное место будет ей
    }                                           // занято, а ребро станет заполненным
  }

//---------------------------------------------------------------------------
                                                // Исключение вершины из ребра
  void HGEdge::disconnectVertex (HGVertex *oldVertex)
  {
    for (int i=0; i<maxCountOfVertex; i++)
      if (vertices[i] == oldVertex)
      {
        vertices[i] = NULL;
        if (full)
          full = false;
        return;
      }
  }
//---------------------------------------------------------------------------

  bool HGEdge::connectVertex (HGVertex *newVertex)      // Попытка включения вершины в ребро
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

//---------------------------------------------------------------------------
                   //*****************************//
  bool HGEdge::isFull ()                                // Геттер заполненности
  {
    return full;
  }
//---------------------------------------------------------------------------
                   //*****************************//
  bool HGEdge::set_maxCountOfVertex (int maxCV)         // Сеттер максимальной мощности
  {
    if (maxCV >= 0)
    {
      maxCountOfVertex = maxCV;
      return true;
    }
    else
      return false;
  }

//---------------------------------------------------------------------------
                   //*****************************//
  int HGEdge::get_maxCountOfVertex ()                   // Геттер максимальной мощности
  {
    return maxCountOfVertex;
  }

//---------------------------------------------------------------------------
                   //*****************************//
  HGVertex* HGEdge::get_vertex (int number)             // Геттер вершины по номеру
  {                                             // Возвращает NULL, если такой
                                                // не существует
    if (maxCountOfVertex <= number)
      return NULL;
    return vertices[number];
  }

//---------------------------------------------------------------------------
                   //*****************************//
  bool HGEdge::isInEdge (HGVertex *v)                   // Проверка принадлежности
  {                                             // вершины ребру
    if (maxCountOfVertex == 0)
      return false;
    for (int i=0; i<maxCountOfVertex; i++)
      if (vertices[i] == v)
        return true;
    return false;
  }

//---------------------------------------------------------------------------
                   //*****************************//
                                                  // Геттер числа общих с
                                                  // другим ребром вершин
  int HGEdge::getCountOfCommonVertices (HGEdge *otherEdge)
  {
    int countOfCommonVertices = 0;
    for (int i=0; i<get_maxCountOfVertex(); i++)
      if (vertices[i] != NULL)
        if (otherEdge->isInEdge(vertices[i]))
          countOfCommonVertices++;
    return countOfCommonVertices;
  }
//---------------------------------------------------------------------------
                   //*****************************//
  int HGEdge::getCountSubHG ()                  // Геттер числа подграфов, в которые
  {                                             // входит ребро
    int countSubHG = 1;
    for (int i=0; i<maxCountOfVertex; i++)
      if (vertices[i] != NULL && vertices[i]->get_numberOfHG() != -1)
        for (int j=0; j<i; j++)
        {
          if (vertices[j]!= NULL)
            if (vertices[j]->get_numberOfHG() ==
                vertices[i]->get_numberOfHG())
              break;
          if (j == i-1)
            countSubHG++;
        }
    return countSubHG;
  }

//---------------------------------------------------------------------------
                   //*****************************//
  int HGVertex::freePlaceForConnect ()          // Поиск свободного места и изменение
  {                                             // состояния, если такого нет
    if (full)
      return -1;
    else
    {
      int countFree = 0;
      int numberFree = -1;
      for (int i=0; i<maxDegree; i++)
        if (edges[i] == NULL)
        {
          numberFree = i;
          countFree++;
        }
      if(countFree == 1)                        // Состояние изменяется, т.к. данный метод вызывается
        full = true;                            // непосредственно перед включением вершины в ребро,
      return numberFree;                        // и значит, единственное свободное место будет им
    }                                           // занято, а вершина станет заполненной
  }

//---------------------------------------------------------------------------
                   //*****************************//
  int HGVertex::get_numberOfHG ()               // Геттер номера подграфа
  {
    return numberOfHG;
  }

//---------------------------------------------------------------------------
                   //*****************************//
                                                // Сеттер номера подграфа
  void HGVertex::set_numberOfHG (int numberSubHG)
  {
    numberOfHG = numberSubHG;
  }

//---------------------------------------------------------------------------
                   //*****************************//
                                                // Попытка включения вершины в ребро
  bool HGVertex::connectVertex (HGEdge *newEdge)
  {
    if (full)
      return false;
    else
    {
      int numberFree = freePlaceForConnect ();
      if (numberFree > -1)
      {
        edges[numberFree] = newEdge;
        return true;
      }
      else
        return false;
    }
  }

//---------------------------------------------------------------------------
                   //*****************************//
                                                // Отключение вершины от ребра
  void HGVertex::disconnectVertex (HGEdge *oldEdge)
  {
    for (int i=0; i<maxDegree; i++)
      if (edges[i] == oldEdge)
      {
        edges[i] = NULL;
        if (full)
          full = false;
        return;
      }
  }

//---------------------------------------------------------------------------
                   //*****************************//
  bool HGVertex::isFull ()                      // Геттер заполненности
  {
    return full;
  }

//---------------------------------------------------------------------------
                   //*****************************//
  int HGVertex::get_numberV ()                  // Геттер собственного номера вершины
  {
    return numberV;
  }

//---------------------------------------------------------------------------
                   //*****************************//
  bool HGVertex::set_numberV (int nV)           // Сеттер собственного номера вершины
  {
    if (nV <= 0)
      return false;
    else
    {
      numberV = nV;
      return true;
    }
  }

//---------------------------------------------------------------------------
                   //*****************************//
  int HGVertex::get_maxDegree ()                // Геттер максимальной ЛС
  {
    return maxDegree;
  }

//---------------------------------------------------------------------------
                   //*****************************//
  bool HGVertex::set_maxDegree (int mD)         // Сеттер максимальной ЛС
  {
    if (mD > 0)
    {
      maxDegree = mD;
      return true;
    }
    else
      return false;
  }

//---------------------------------------------------------------------------
                   //*****************************//
  bool HGVertex::isInEdge (HGEdge *e)           // Проверка принадлежности
  {                                             // вершины ребру
    if (maxDegree == 0)
      return false;
    for (int i=0; i<maxDegree; i++)
      if (edges[i] == e)
        return true;
    return false;
  }

//---------------------------------------------------------------------------
                   //*****************************//
  int HGVertex::getCountOfFreePlaces ()         // Геттер числа свободных мест
  {                                             // для подключений
    if (full)
      return 0;
    int countOfFreePlaces = 0;
    for (int i=0; i<maxDegree; i++)
      if (edges[i] == NULL)
        countOfFreePlaces++;
    return countOfFreePlaces;
  }

//---------------------------------------------------------------------------
                   //*****************************//
                                                // Геттер инцидентного ребра по номеру
  HGEdge * HGVertex::get_edge (int numberOfEdge)
  {
    if (maxDegree <= numberOfEdge)
      return NULL;
    return edges[numberOfEdge];
  }

//---------------------------------------------------------------------------
                   //*****************************//
                                                // Создание вершин
  void HGraph::createVertices (int countVertices, int minDegree, int maxDegree)
  {
    if (mainGraph)                              // Создание вершин и ребер вызывается
    {                                           // только для корневого графа
      vertices = new HGVertex * [countVertices];
      set_countOfVertices (countVertices);
      for (int i=0; i<countVertices; i++)
        vertices[i] = new HGVertex (i,(minDegree+rand()%(maxDegree-minDegree+1)));
    }
  }

//---------------------------------------------------------------------------
                   //*****************************//
                                                // Геттер общего числа возможных
                                                // подключений ребер к вершинам
  int HGraph::getSummaryDegree ()
  {
    int summaryDegree = 0;
    int cOfVertices = get_countOfVertices();
    for (int i=0; i<cOfVertices; i++)
      summaryDegree += vertices[i]->get_maxDegree();
    return summaryDegree;
  }

//---------------------------------------------------------------------------
                   //*****************************//
                                                // Геттер числа вершин с
                                                // возможностью подключения
  int HGraph::getCountOfFreeVertices ()
  {
    int countOfFreeVertices = 0;
    int cOfVertices = get_countOfVertices();
    for (int i=0; i<cOfVertices; i++)
      if (!vertices[i]->isFull())
        countOfFreeVertices++;
    return countOfFreeVertices;
  }


//---------------------------------------------------------------------------
                   //*****************************//
                                                // Геттер максимума возможных
                                                // подключений ребер к вершинам
  int HGraph::getGlobalMaxDegree ()
  {
    int globalMaxDegree = 0;
    int cOfVertices = get_countOfVertices();
    for (int i=0; i<cOfVertices; i++)
      if (globalMaxDegree < vertices[i]->get_maxDegree())
        globalMaxDegree = vertices[i]->get_maxDegree();
    return globalMaxDegree;
  }

//---------------------------------------------------------------------------
                   //*****************************//
                                                // Создание ребер
  void HGraph::createEdges (int minCountOfVertices, int maxCountOfVertices)
  {
    if (mainGraph)                              // Создание вершин и ребер вызывается
    {                                           // только для корневого графа

      int summaryDegree = getSummaryDegree();   // Общее число возможных подключений

                                                // Порог, по прохождении которого
                                                // включается логика формирования
                                                // остаточных цепей
      int logicalThreshold = maxCountOfVertices*getGlobalMaxDegree();


                                                // Максимальное число цепей - это
                                                // вершины * макс. степень / мин. мощность
      edges = new HGEdge * [countOfVertices*logicalThreshold/(maxCountOfVertices*minCountOfVertices)];

      for (int i=0; ; i++)        // Начинаем построение ребер (заранее неизвестно, сколько их)
        if (logicalThreshold < summaryDegree)
        {
          edges[i] = new HGEdge (minCountOfVertices + rand()%(maxCountOfVertices+1));

          do{
            int nextVertexToConnect;            // Номер очередной вершины для включения в ребро
            do
              nextVertexToConnect = rand()%get_countOfVertices();
            while (vertices[nextVertexToConnect]->isInEdge(edges[i]) ||
                  vertices[nextVertexToConnect]->isFull());

                                                // Установление инцидентности
            incidenceInstall(vertices[nextVertexToConnect], edges[i]);

            summaryDegree--;                    // Снижаем число оставшихся для
                                                // обработки подключений

          }
          while (!edges[i]->isFull());
        }
        else
        {
                                                // Логика обработки последних подключений
                                                // Составляю матрицу-проекцию будущих ребер
          int countOfFreeVertices = getCountOfFreeVertices();
          int globalMaxDegree = logicalThreshold/maxCountOfVertices;
          int ** connectionMatrix = new int * [countOfFreeVertices];

                                                // 0 - нет контакта, 1 - есть
                                                // Надо распределить единицы по матрице
                                                // так, чтобы в строках было минимум
                                                // количество, равное минимальной мощности
                                                // ребер
                                                // Получившиеся строки - прототипы ребер
          for (int k=0; k<countOfFreeVertices; k++)
          {
            connectionMatrix[k] = new int [globalMaxDegree+1];
            for (int j=0; j<globalMaxDegree; j++)
              connectionMatrix[k][j] = 0;
          }


          int elementOfMatrixNumber = 0;
          int cOfVertices = get_countOfVertices();


          for (int k=0; k<cOfVertices; k++)
          {
            if (!vertices[k]->isFull())
            {
              int freePlaces = vertices[k]->getCountOfFreePlaces();
              for(int j=0; j<freePlaces; j++)
                connectionMatrix[elementOfMatrixNumber][j] = 1;
              connectionMatrix[elementOfMatrixNumber][globalMaxDegree] =
                        vertices[k]->get_numberV();
              elementOfMatrixNumber++;
            }
          }                                     // Матрица готова

          int *masOfPowerEdges = new int [globalMaxDegree];
          for (int k=0; k<globalMaxDegree; k++)
          {
            masOfPowerEdges[k] = 0;
            for (int j=0; j<countOfFreeVertices; j++)
              if (connectionMatrix[j][k] == 1)
                masOfPowerEdges[k]++;
          }

          for (int k=0; k<globalMaxDegree; k++)
          {
            if (masOfPowerEdges[k] < minCountOfVertices)
            {
              for (int j=0; j<globalMaxDegree; j++)
              {
                bool endOfMove = false;
                if (masOfPowerEdges[j] > minCountOfVertices)
                {
                  int countMovedVertices = masOfPowerEdges[j] - minCountOfVertices;
                  if (countMovedVertices > (minCountOfVertices - masOfPowerEdges[k]))
                  {
                    countMovedVertices = minCountOfVertices - masOfPowerEdges[k];
                    endOfMove = true;
                  }
                  int numberMovedVertex = 0;
                  for (int kk=0; kk<countMovedVertices; kk++)
                  {
                    if (connectionMatrix[numberMovedVertex][j] == 1 &&
                                connectionMatrix[numberMovedVertex][k] == 0)
                    {
                      connectionMatrix[numberMovedVertex][j] = 0;
                      connectionMatrix[numberMovedVertex][k] = 1;
                      masOfPowerEdges[j]--;
                      masOfPowerEdges[k]++;
                    }
                    else
                      kk--;
                    numberMovedVertex++;
                  }
                }
                if (endOfMove)
                  break;
              }
            }
          }

          for (int j=0; j<globalMaxDegree; j++)
          {

            edges[i] = new HGEdge (masOfPowerEdges[j]);
                                                // Установление инцидентности
            for (int k=0; k<countOfFreeVertices; k++)
              if (connectionMatrix[k][j] == 1)
                incidenceInstall(vertices[connectionMatrix[k][globalMaxDegree]], edges[i]);
            i++;

          }
          set_countOfEdges (i-1);
          delete [] masOfPowerEdges;
          delete [] connectionMatrix;
          break;
        }
    }
  }

//---------------------------------------------------------------------------
                   //*****************************//
  int HGraph::getCountOfExternalEdges ()                // Геттер числа внешних связей
  {
    int countOfExternalEdges = 0;
    for (int i=0; i<countOfEdges; i++)
      countOfExternalEdges += edges[i]->getCountSubHG() -1;
    countOfExternalEdges = countOfExternalEdges/2;

    return countOfExternalEdges;
  }

//---------------------------------------------------------------------------
                   //*****************************//
                                                // Проверка инцидентности вершины и ребра
  bool HGraph::isIncidet (HGVertex *vertex, HGEdge *edge)
  {
    if (vertex->isInEdge(edge) && edge->isInEdge(vertex))
      return true;
    else
      return false;
  }

//---------------------------------------------------------------------------
                   //*****************************//
                                                // Установление инцидентности вершины и ребра
  bool HGraph::incidenceInstall (HGVertex * vertex, HGEdge *edge)
  {
    if (!vertex->isFull() && !edge->isFull())
      if (vertex->connectVertex(edge))
        if (edge->connectVertex(vertex))
          return true;
        else
          vertex->disconnectVertex(edge);
    return false;
  }

//---------------------------------------------------------------------------
                   //*****************************//
                                                // Устранение инцидентности вершины и ребра
  void HGraph::incidenceUninstall (HGVertex * vertex, HGEdge *edge)
  {
    vertex->disconnectVertex(edge);
    edge->disconnectVertex(vertex);
  }

//---------------------------------------------------------------------------
  int HGraph::get_numberOfHG ()                         // Геттер числа внешних связей
  {
    return numberOfHG;
  }

//---------------------------------------------------------------------------
                   //*****************************//
  int HGraph::get_countOfVertices ()                    // Геттер числа вершин
  {
    return countOfVertices;
  }

//---------------------------------------------------------------------------
                   //*****************************//
  bool HGraph::set_countOfVertices (int cV)             // Сеттер числа вершин
  {
    if (cV > 0)
    {
      countOfVertices = cV;
      return true;
    }
    else
      return false;
  }

//---------------------------------------------------------------------------
                   //*****************************//
  int HGraph::get_countOfEdges ()                       // Геттер числа гиперребер
  {
    return countOfEdges;
  }

//---------------------------------------------------------------------------
                   //*****************************//
  int HGraph::get_countOfFragments ()                       // Геттер числа ребер
  {
    int countOfFragments = 0;
    for (int i=0; i<get_countOfEdges(); i++)
      countOfFragments += edges[i]->get_maxCountOfVertex()-1;

    return countOfFragments;
  }

//---------------------------------------------------------------------------
                   //*****************************//
  bool HGraph::set_countOfEdges (int cE)                // Сеттер числа гиперребер
  {
    if (cE > 0)
    {
      countOfEdges = cE;
      return true;
    }
    else
      return false;
  }

//---------------------------------------------------------------------------
                   //*****************************//
                                                // Генератор гиперграфа
  void HGraph::HGraphGenerator (int CountOfVertices, int minDegree, int maxDegree,
                        int minPowerOfEdge, int maxPowerOfEdge)
  {
    createVertices (CountOfVertices, minDegree, maxDegree);
    createEdges(minPowerOfEdge, maxPowerOfEdge);
  }
//---------------------------------------------------------------------------
                   //*****************************//
                                                // Алгоритм "затягивания" ребер
  void HGraph::gravityEdge (int powerOfSubHG, int numberOfSubHG)
  {
    int minSplitOfEdge = 1;                     // Нижняя граница оценки
                                                // числа подграфов, в которые
                                                // входит ребро

                                                // Число вершин для включения
                                                // в подграф
    int countPlacesInSubHG = powerOfSubHG;
    bool noCorrectEdges;                        // Сигнал для поднятия нижней
                                                // границы
    do
    {
      noCorrectEdges = true;
      for (int i=0; i<countOfEdges; i++)
      {
                                                // Нашел подходящее ребро
        if (edges[i]->getCountSubHG() <= minSplitOfEdge)
        {
                                                // Ищу свободные вершины
          for (int j=0; j<edges[i]->get_maxCountOfVertex(); j++)
            if (edges[i]->get_vertex(j) != NULL)
              if (edges[i]->get_vertex(j)->get_numberOfHG() == numberOfHG)
              {
                noCorrectEdges = false;
                                                // Включаю в подграф
                                                // столько, сколько влезет :)
                edges[i]->get_vertex(j)->set_numberOfHG(numberOfSubHG);
                countPlacesInSubHG--;
                if (countPlacesInSubHG == 0)
                {
                  break;                        // Если больше не влезет -
                }                               // выхожу
              }

          if (countPlacesInSubHG == 0)
          {
            break;
          }
        }
      }
      if (noCorrectEdges)                   // Когда не нашел подходящих ребер
        minSplitOfEdge++;                   // поднимаем нижнюю границу
    }
    while (countPlacesInSubHG > 0);
  }

//---------------------------------------------------------------------------
                   //*****************************//
                                                // Случайное разбиение
  void HGraph::randomSplitHG (int countSubHG, int startNumberSubG)
  {
    splitResultCount = countSubHG;
                                                // Мощности подграфов
    int *masPowerOfSubHG = new int [countSubHG];
    for (int i=0; i<countSubHG; i++)
      masPowerOfSubHG[i] = countOfVertices/countSubHG;
    for (int i=0; i<countOfVertices%countSubHG; i++)
      masPowerOfSubHG[i]++;

    for (int i=0; i<countSubHG; i++)
      for (int j=0; j<masPowerOfSubHG[i]; j++)
      {
        int numberNextVertex;
        do
        {
          numberNextVertex = rand()%countOfVertices;
        }
        while(vertices[numberNextVertex]->get_numberOfHG() != numberOfHG);
        vertices[numberNextVertex]->set_numberOfHG(i+startNumberSubG);
      }
  }

//---------------------------------------------------------------------------
                   //*****************************//
                                                // Последовательное разбиение
  void HGraph::gravitySplitHG (int countSubHG, int startNumberSubG)
  {
    splitResultCount = countSubHG;
                                                // Мощности подграфов
    int *masPowerOfSubHG = new int [countSubHG];
    for (int i=0; i<countSubHG; i++)
      masPowerOfSubHG[i] = countOfVertices/countSubHG;
    for (int i=0; i<countOfVertices%countSubHG; i++)
      masPowerOfSubHG[i]++;

    for (int i=0; i<countSubHG; i++)
      gravityEdge(masPowerOfSubHG[i], i+startNumberSubG);
  }

//---------------------------------------------------------------------------
                   //*****************************//
                                                // Создание подграфа
  HGraph * HGraph::createSubHG (int numberOfSubHG)
  {
    if (numberOfSubHG >= 0)
    {
      HGraph * subHG = new HGraph(vertices, countOfVertices, numberOfSubHG);
      return subHG;
    }
    else
      return NULL;
  }

//---------------------------------------------------------------------------
                   //*****************************//
                                                // Убрать разбиение
  void HGraph::resetSplitHG ()
  {
    for (int i=0; i<countOfVertices; i++)
      vertices[i]->set_numberOfHG(-1);
  }

//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
  countHG = StrToIntDef(Edit13->Text,10);
  if (countHG<1)
    countHG=10;
  hGraph = new HGraph *[countHG];
  for (int i=0; i<countHG; i++)
    hGraph[i] = new HGraph;

  for (int i=0; i<countHG; i++)
    hGraph[i]->HGraphGenerator(StrToIntDef(Edit1->Text,0),
                        StrToIntDef(Edit2->Text,0),
                        StrToIntDef(Edit3->Text,0),
                        StrToIntDef(Edit4->Text,0),
                        StrToIntDef(Edit5->Text,0));
  Button1->Enabled = false;
  Button2->Enabled = true;
  Button3->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
  Chart1->Visible = false;
  Chart2->Visible = false;
  Series1->Clear();
  Series2->Clear();
  for (int j=2; j<=StrToIntDef(Edit6->Text, 0); j++)
  {
    float countOfAllFragments = 0;
    float countOfAllExternalEdges = 0;
    for (int i=0; i<countHG; i++)
    {
      hGraph[i]->resetSplitHG();
      hGraph[i]->randomSplitHG(j,0);
      countOfAllFragments += hGraph[i]->get_countOfFragments();
      countOfAllExternalEdges += hGraph[i]->getCountOfExternalEdges();
    }
    Series2->AddXY(j,100*countOfAllExternalEdges/countOfAllFragments,"",clRed);
  }
  Chart2->Visible = true;
//  Label13->Caption = IntToStr(hGraph->getCountOfExternalEdges());
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
  float exponent;
  if (RadioGroup2->ItemIndex == 0)
    exponent = 2.0;
  else
    exponent = 3.0;
  ProgressBar1->Max = StrToIntDef(Edit6->Text, 0)*countHG;
  ProgressBar1->Min = countHG*2;
  ProgressBar1->Position = countHG*2;
  Chart2->Visible = false;
  Chart1->Visible = false;
  Series2->Clear();
  Series1->Clear();
  float countOfAllFragments = 0;
  for (int i=0; i<countHG; i++)
    countOfAllFragments += hGraph[i]->get_countOfFragments();
  countOfAllFragments /= countHG;
  Series1->AddXY(1,pow((double)hGraph[0]->get_countOfVertices(),2.0) +
                        pow((double)countOfAllFragments,exponent),"",clRed);
  for (int j=2; j<=StrToIntDef(Edit6->Text, 0); j++)
  {
    float countOfAllExternalEdges = 0;
    for (int i=0; i<countHG; i++)
    {
      hGraph[i]->resetSplitHG();
      hGraph[i]->gravitySplitHG(j,0);
      countOfAllExternalEdges += hGraph[i]->getCountOfExternalEdges();
      ProgressBar1->Position = j*countHG+i;
      Form1->Refresh();
    }
    countOfAllExternalEdges /= countHG;
    Series2->AddXY(j,100*countOfAllExternalEdges/countOfAllFragments,"",clRed);
    Series1->AddXY(j,hGraph[0]->get_countOfVertices() +
                        pow((double)hGraph[0]->get_countOfVertices()/j,2.0) +
                        pow((double)countOfAllExternalEdges,exponent) +
                        pow((double)(countOfAllFragments-countOfAllExternalEdges)/j,exponent),"",clRed);
  }
  Chart2->Visible = true;
  Chart1->Visible = true;
//  Label13->Caption = IntToStr(hGraph->getCountOfExternalEdges());
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
  delete [] hGraph;
  delete [] hierarhyHG;
  delete [] increaseOfCountExternalEdges;
}
//---------------------------------------------------------------------------
                   //*****************************//

void TForm1::initHierarhyHG()                   // Инициализация массивов для
{                                               // иерархического разбиения
  minNumberSubHG = 0;
  increaseOfCountExternalEdges = new int * [StrToIntDef(Edit12->Text,0)];
  hierarhyHG = new HGraph *** [StrToIntDef(Edit12->Text,0)];

  for (int i=0; i<StrToIntDef(Edit12->Text,0); i++)
  {
    hierarhyHG[i] = new HGraph ** [(int)pow(2.0,i)];
    for (int j=0; j<(int)pow(2.0,i); j++)
      hierarhyHG[i][j] = new HGraph * [countHG];

    increaseOfCountExternalEdges[i] = new int [(int)pow(2.0,i)];
  }

  ProgressBar2->Max = countHG;
  ProgressBar2->Min = 0;
  ProgressBar2->Position = 0;

  for (int i=0; i<countHG; i++)                 // Создание начальных гиперграфов
  {
    hierarhyHG[0][0][i] = new HGraph;
    hierarhyHG[0][0][i]->HGraphGenerator(StrToIntDef(Edit7->Text,0),
                        StrToIntDef(Edit8->Text,0),
                        StrToIntDef(Edit9->Text,0),
                        StrToIntDef(Edit10->Text,0),
                        StrToIntDef(Edit11->Text,0));
    ProgressBar2->Position = i;
  }
}

//---------------------------------------------------------------------------
                   //*****************************//

  void TForm1::showData (int Complexity)        // Вывод данных на график
                                                // "затраты в шагах" в зависимости
                                                // от сложности алгоритма трассировки
{
  Chart3->Visible = false;
  Series3->Clear();
  double currentCostOfTracing = 0;
  double currentCountOfInternalEdges = 0;
  for (int i=0; i<countHG; i++)
    currentCountOfInternalEdges += hierarhyHG[0][0][i]->get_countOfFragments();
  currentCountOfInternalEdges /= countHG;

  Series3->AddXY(0,pow((double)currentCountOfInternalEdges,Complexity)+
                pow((double)StrToIntDef(Edit7->Text,0),2),"",clRed);

  for (int i=0; i<StrToIntDef(Edit12->Text, 0); i++)
  {
    int maxCountExternalEdges = 0;
    for (int j=0; j<(int)pow(2.0,i); j++)
    {
      if (maxCountExternalEdges < increaseOfCountExternalEdges[i][j])
        maxCountExternalEdges = increaseOfCountExternalEdges[i][j];
      currentCountOfInternalEdges -= increaseOfCountExternalEdges[i][j];
    }
    currentCostOfTracing += pow((double)maxCountExternalEdges,Complexity);
    double nextValue = hierarhyHG[i][0][0]->get_countOfVertices() +
                        pow((double)hierarhyHG[i][0][0]->get_countOfVertices()/2,2.0) +
                        currentCostOfTracing +
                        pow (currentCountOfInternalEdges / pow(2.0,i+1),
                        Complexity);
    Series3->AddXY(i+1,nextValue,"",clRed);
  }
  Chart3->Visible = true;
}

//---------------------------------------------------------------------------
                   //*****************************//

  void TForm1::gatheringData()                  // Проведение иерархического
                                                // разбиения и сбор данных
                                                // об изменении числа внешних связей
{
  ProgressBar2->Max = (int)pow(2.0,StrToIntDef(Edit12->Text, 0))*countHG;
  ProgressBar2->Min = countHG;
  ProgressBar2->Position = countHG;

  int countOldExternalEdges = 0;                // Старое число внешних связей
                                                // для расчета приращения

  for (int i=0; i<StrToIntDef(Edit12->Text, 0); i++)
  {
    Label13->Caption = "Осталось уровней: "+IntToStr(StrToIntDef(Edit12->Text, 0)-i);
    Label13->Width += 50;
    Form1->Refresh();
                                            // Уровни
    for (int j=0; j<(int)pow(2.0,i); j++)       // Подграфы в уровнях
    {
      increaseOfCountExternalEdges[i][j] = 0;
      for (int k=0; k<countHG; k++)             // Множество экспериментов
      {
        hierarhyHG[i][j][k]->gravitySplitHG(2,minNumberSubHG);
                                                // Разбили

        increaseOfCountExternalEdges[i][j] += hierarhyHG[0][0][k]->getCountOfExternalEdges();
                                                // Плюсанул новые внешние связи
        if (i < StrToIntDef(Edit12->Text, 0)-1) // Если не последняя итерация -
                                                // создаю подграфы на основе разбиений
        {
          hierarhyHG[i+1][j*2][k] = hierarhyHG[i][j][k]->createSubHG(minNumberSubHG);
          hierarhyHG[i+1][j*2+1][k] = hierarhyHG[i][j][k]->createSubHG(minNumberSubHG+1);
        }

      }
                                                // Вычисляю среднее значение по серии
      increaseOfCountExternalEdges[i][j] /= countHG;

                                                // Вычитаю уже имевшиеся связи
                                                // для получения прироста
      increaseOfCountExternalEdges[i][j] -= countOldExternalEdges;
      countOldExternalEdges += increaseOfCountExternalEdges[i][j];

                                                // Увеличиваю счетчик собственных
                                                // номеров подграфов
      minNumberSubHG += 2;

      ProgressBar2->Position += countHG;
      Form1->Refresh();
    }
  }
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
  countHG = StrToIntDef(Edit14->Text,10);
  if (countHG<1)
    countHG=10;

  Chart4->Visible = false;
  Chart3->Visible = false;

  Label13->Caption = "Создание ГГ...";
  Label13->Width += 50;
  Label13->Visible = true;
  Form1->Refresh();
  initHierarhyHG();

  gatheringData();

  Series4->Clear();

  Label13->Caption = "Почти все...";
  Form1->Refresh();
                                                // Заполняю график
                                                // "процент внешних связей"
  float countAllFragments = 0;
  for (int i=0; i<countHG; i++)
    countAllFragments += hierarhyHG[0][0][i]->get_countOfFragments();
  countAllFragments /= countHG;
  float nextIncreaseValue = 0;

  for (int i=0; i<StrToIntDef(Edit12->Text, 0); i++)
  {
    for (int j=0; j<(int)pow(2.0,i); j++)
      nextIncreaseValue += increaseOfCountExternalEdges[i][j];
    Series4->AddXY(i+1,100*nextIncreaseValue/countAllFragments,"",clRed);
  }

  Label13->Visible = false;
  Chart4->Visible = true;
  Chart3->Visible = true;
  Button4->Enabled = false;
  RadioGroup1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioGroup1Click(TObject *Sender)
{
  if (RadioGroup1->ItemIndex == 0)
    showData(2);
  else
    showData(3);
}
//---------------------------------------------------------------------------

