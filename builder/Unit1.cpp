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
    hierarhyHG[i] = new HGraph** [(int)pow(2.0,i)];

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

