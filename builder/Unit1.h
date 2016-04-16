//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class HGVertex;                                 // Определение ниже
                                                // Сделано для нормального
                                                // объявления указателей на
                                                // вершины в гиперребрах
//---------------------------------------------------------------------------

class HGEdge                                    // Ребро гиперграфа
{
private:
  int maxCountOfVertex;                         // Максимальная мощность ребра
  HGVertex **vertices;                          // Указатели на вершины
  bool full;                                    // Состояние заполненности

                   //*****************************//
  int freePlaceForConnect ();                   // Поиск свободного места и изменение
                                                // состояния заполненности, если такого нет

public:
                   //*****************************//
  HGEdge ()
  {
    maxCountOfVertex = 0;
    vertices = NULL;
  }

                   //*****************************//
  HGEdge (int maxV)
  {
    maxCountOfVertex = maxV;
    vertices = new HGVertex * [maxV];
    for (int i=0; i<maxV; i++)
      vertices[i] = NULL;
  }

                   //*****************************//
  ~HGEdge ()
  {
    if (maxCountOfVertex > 0)
      for (int i=0; i<maxCountOfVertex; i++)
        vertices[i] = NULL;
  }

                   //*****************************//
  void disconnectVertex (HGVertex *oldVertex);  // Исключение вершины из ребра

                   //*****************************//
  bool connectVertex (HGVertex *newVertex);     // Попытка включения вершины в ребро

                   //*****************************//
  bool isFull ();                               // Геттер заполненности

                   //*****************************//
  bool set_maxCountOfVertex (int maxCV);        // Сеттер максимальной мощности

                   //*****************************//
  int get_maxCountOfVertex ();                  // Геттер максимальной мощности

                   //*****************************//
  HGVertex* get_vertex (int number);            // Геттер вершины по номеру
                                                // Возвращает NULL, если такой
                                                // не существует

                   //*****************************//
  bool isInEdge (HGVertex *v);                  // Проверка принадлежности
                                                // вершины ребру

                   //*****************************//
                                                  // Геттер числа общих с
                                                  // другим ребром вершин
  int getCountOfCommonVertices (HGEdge *otherEdge);

                     //*****************************//
  int getCountSubHG ();                         // Геттер числа подграфов, в которые
                                                // входит ребро


};

//---------------------------------------------------------------------------

class HGVertex                                  // Вершина гиперграфа
{
private:
  int numberV;                                  // Собственный номер вершины
  int maxDegree;                                // Максимально возможно число
                                                // инцидентных гиперребер
  int numberOfHG;                               // Номер подграфа, -1, если не
                                                // принадлежит ни к одному
  HGEdge **edges;                               // Инцидентные гиперребра
  bool full;                                    // Состояние заполненности

                   //*****************************//
  int freePlaceForConnect ();                   // Поиск свободного места и изменение
                                                // состояния заполненности, если такого нет

public:
                   //*****************************//
  HGVertex ()
  {
    numberV = 0;
    maxDegree = 0;
    full = true;
    edges = NULL;
    numberOfHG = -1;
  }

                   //*****************************//
  HGVertex (int nV, int maxD)
  {
    numberOfHG = -1;
    numberV = nV;
    maxDegree = maxD;
    full = false;
    edges = new HGEdge * [maxD];
    for (int i=0; i<maxD; i++)
      edges[i] = NULL;
  }

                   //*****************************//
  ~HGVertex ()
  {
    for (int i=0; i<maxDegree; i++)
      edges[i] = NULL;
  }

                   //*****************************//
  int get_numberOfHG ();                        // Геттер номера подграфа

                   //*****************************//
  void set_numberOfHG (int numberSubHG);         // Сеттер номера подграфа

                   //*****************************//
  bool connectVertex (HGEdge *newEdge);         // Попытка включения вершины в ребро

                   //*****************************//
  void disconnectVertex (HGEdge *oldEdge);      // Отключение вершины от ребра

                   //*****************************//
  bool isFull ();                               // Геттер заполненности

                   //*****************************//
  int get_numberV ();                           // Геттер собственного номера вершины

                   //*****************************//
  bool set_numberV (int nV);                    // Сеттер собственного номера вершины

                   //*****************************//
  int get_maxDegree ();                         // Геттер максимальной ЛС

                   //*****************************//
  bool set_maxDegree (int mD);                  // Сеттер максимальной ЛС

                   //*****************************//
  bool isInEdge (HGEdge *e);                    // Проверка принадлежности
                                                // вершины ребру

                   //*****************************//
  int getCountOfFreePlaces ();                  // Геттер числа свободных мест
                                                // для подключений

                   //*****************************//
  HGEdge * get_edge (int numberOfEdge);         // Геттер инцидентного ребра по номеру

};

//---------------------------------------------------------------------------

class HGraph                                    // Гиперграф
{
private:
  HGVertex **vertices;                          // Вершины
  HGEdge   **edges;                             // Гиперребра
  int numberOfHG;                               // Номер подграфа (у главного -1)
  int countOfVertices;                          // Число вершин
  int countOfEdges;                             // Число ребер
  int splitResultCount;                         // Число подграфов
  bool mainGraph;                               // Является ли корневым графом

                   //*****************************//
                                                // Алгоритм "затягивания" ребер
                                                // в один подграф
  void gravityEdge (int powerOfSubHG, int numberOfSubHG);

                   //*****************************//
                                                // Создание вершин
  void createVertices (int countVertices, int minDegree, int maxDegree);

                   //*****************************//
                                                // Геттер общего числа возможных
                                                // подключений ребер к вершинам
  int getSummaryDegree ();

                   //*****************************//
                                                // Геттер числа вершин с
                                                // возможностью подключения
  int getCountOfFreeVertices ();


                   //*****************************//
                                                // Геттер максимума возможных
                                                // подключений ребер к вершинам
  int getGlobalMaxDegree ();

                   //*****************************//
                                                // Создание ребер
  void createEdges (int minCountOfVertices, int maxCountOfVertices);

public:
                   //*****************************//
  HGraph ()
  {
    vertices = NULL;
    edges = NULL;
    countOfVertices = 0;
    countOfEdges = 0;
    splitResultCount = 0;
    mainGraph = true;
    numberOfHG = -1;
  }
                   //*****************************//
                                                // Конструктор подграфа
  HGraph (HGVertex **masOfVertex, int cOfVertices, int numberSubG)
  {
    splitResultCount = 0;
    mainGraph = false;
    numberOfHG = numberSubG;

    int countVerticesOfSubG = 0;
    int countEdgesOfSubG = 0;
    for (int i=0; i<cOfVertices; i++)           // Считаю число вершин, входящих
                                                // в подграф, и максимальное
                                                // число инцидентных им ребер
      if (masOfVertex[i]->get_numberOfHG()==numberSubG)
      {
        countVerticesOfSubG++;
        countEdgesOfSubG += masOfVertex[i]->get_maxDegree()
                                - masOfVertex[i]->getCountOfFreePlaces();
      }

    vertices = new HGVertex *[countVerticesOfSubG];
    countOfVertices = countVerticesOfSubG;

                                                // Промежуточный массив под ребра
                                                // для устранения дублей
    HGEdge **newEdges = new HGEdge *[countEdgesOfSubG];
    countVerticesOfSubG = 0;
    countEdgesOfSubG = 0;
    for (int i=0; i<cOfVertices; i++)
      if (masOfVertex[i]->get_numberOfHG()==numberSubG)
      {
                                                // Делаю собственные ссылки на
                                                // вершины у подграфа
        vertices[countVerticesOfSubG] = masOfVertex[i];
        countVerticesOfSubG++;
        for (int j=0; j<masOfVertex[i]->get_maxDegree(); j++)
          if (masOfVertex[i]->get_edge(j)!= NULL)
          {
            bool newEdge = true;
                                                // Если такое ребро уже есть,
                                                // то включать уже не надо
            for (int k=0; k<countEdgesOfSubG; k++)
              if (newEdges[k] == masOfVertex[i]->get_edge(j))
              {
                newEdge = false;
                break;
              }
            if (newEdge)
            {
              newEdges[countEdgesOfSubG] = masOfVertex[i]->get_edge(j);
              countEdgesOfSubG++;
            }
          }
      }
                                                // Окончательное формирование
                                                // массива ссылок на ребра
                                                // подграфа
    edges = new HGEdge * [countEdgesOfSubG-1];
    countOfEdges = countEdgesOfSubG-1;
    for (int i=0; i<(countEdgesOfSubG-1); i++)
    {
      edges[i] = newEdges[i];
      newEdges[i] = NULL;
    }
    delete [] newEdges;
  }


                   //*****************************//
                                                // Создание подграфа
  HGraph * createSubHG (int numberOfSubHG);

                   //*****************************//
                                                // Случайное разбиение
  void randomSplitHG (int countSubHG, int startNumberSubG);

                   //*****************************//
                                                // Убрать разбиение
  void resetSplitHG ();

                   //*****************************//
                                                // Последовательное разбиение
  void gravitySplitHG (int countSubHG, int startNumberSubG);

                   //*****************************//
                                                // Генератор гиперграфа
  void HGraphGenerator (int CountOfVertices, int minDegree, int maxDegree,
                        int minPowerOfEdge, int maxPowerOfEdge);

                   //*****************************//
  int get_numberOfHG ();                        // Геттер номера подграфа

                   //*****************************//
  int getCountOfExternalEdges ();               // Геттер числа внешних связей

                   //*****************************//
                                                // Проверка инцидентности вершины и ребра
  bool isIncidet (HGVertex *vertex, HGEdge *edge);

                   //*****************************//
                                                // Установление инцидентности вершины и ребра
  bool incidenceInstall (HGVertex * vertex, HGEdge *edge);

                   //*****************************//
                                                // Устранение инцидентности вершины и ребра
  void incidenceUninstall (HGVertex * vertex, HGEdge *edge);

                   //*****************************//
  int get_countOfVertices ();                   // Геттер числа вершин

                   //*****************************//
  bool set_countOfVertices (int cV);            // Сеттер числа вершин

                   //*****************************//
  int get_countOfEdges ();                      // Геттер числа гиперребер

                   //*****************************//
  int get_countOfFragments ();                  // Геттер числа ребер

                   //*****************************//
  bool set_countOfEdges (int cE);               // Сеттер числа гиперребер

};
//---------------------------------------------------------------------------

class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TPanel *Panel1;
        TChart *Chart1;
        TBarSeries *Series1;
        TChart *Chart2;
        TBarSeries *Series2;
        TProgressBar *ProgressBar1;
        TPanel *Panel2;
        TLabel *Label1;
        TLabel *Label14;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit3;
        TEdit *Edit4;
        TEdit *Edit5;
        TEdit *Edit6;
        TTabSheet *TabSheet2;
        TPanel *Panel3;
        TPanel *Panel4;
        TLabel *Label15;
        TLabel *Label20;
        TLabel *Label21;
        TLabel *Label22;
        TLabel *Label23;
        TLabel *Label24;
        TLabel *Label25;
        TLabel *Label26;
        TButton *Button4;
        TEdit *Edit7;
        TEdit *Edit8;
        TEdit *Edit9;
        TEdit *Edit10;
        TEdit *Edit11;
        TEdit *Edit12;
        TProgressBar *ProgressBar2;
        TChart *Chart3;
        TChart *Chart4;
        TBarSeries *Series3;
        TBarSeries *Series4;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *Label18;
        TRadioGroup *RadioGroup1;
        TRadioGroup *RadioGroup2;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TEdit *Edit13;
        TLabel *Label11;
        TEdit *Edit14;
        TLabel *Label12;
        TLabel *Label13;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall RadioGroup1Click(TObject *Sender);
private:	// User declarations
  void initHierarhyHG ();                       // Инициализация массивов для
                                                // иерархического разбиения

  void gatheringData ();                        // Проведение иерархического
                                                // разбиения и сбор данных
                                                // об изменении числа внешних связей

  void showData (int Complexity);               // Вывод данных на график
                                                // "затраты в шагах" в зависимости
                                                // от сложности алгоритма трассировки

public:		// User declarations
  HGraph ** hGraph;
  HGraph **** hierarhyHG;
  int ** increaseOfCountExternalEdges;
  int countHG;
  int minNumberSubHG;

        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;

//---------------------------------------------------------------------------
#endif
