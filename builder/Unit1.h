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

class TForm1 : public TForm
{
__published:	// IDE-managed Components

// tab1

        TPageControl *PageControl1; //widget
        TTabSheet *TabSheet1; //first tab
        TPanel *Panel1; //panel with  chart & bar
        TChart *Chart1; // steps chart
        TBarSeries *Series1;
        TChart *Chart2; //external edges chart
        TBarSeries *Series2;
        TProgressBar *ProgressBar1; //bottom progress bar
        TPanel *Panel2; //panel with buttons
        TLabel *Label1; // Число вершин
        TLabel *Label14; // число подграфов от 2 до
        TLabel *Label2; // число контактов
        TLabel *Label3; //min
        TLabel *Label4; //max
        TLabel *Label5; // мощность ребер
        TLabel *Label6; //min
        TLabel *Label7; //max
        TLabel *Label8; //delimeter
        TLabel *Label9; // параметры исходных ГГ
        TLabel *Label10; //another delimeter
        TLabel *Label11; // количество ГГ


        TButton *Button1; // создать ГГ
        TButton *Button2; // рандом
        TButton *Button3; // последовательно
        TEdit *Edit1; // ввод числа вершин
        TEdit *Edit2; // ввод min контактов
        TEdit *Edit3; // ввод max контактов
        TEdit *Edit4; // ввод min ребер
        TEdit *Edit5; // ввод max ребер
        TEdit *Edit6; // ввод число подграфов от 2 до...
        TEdit *Edit13; // ввод количества ГГ
        TRadioGroup *RadioGroup2; // сложность трассировки

// tab2

        TTabSheet *TabSheet2; //sec tab
        TPanel *Panel3; //panel with  chart & bar
        TPanel *Panel4; //panel with buttons
        TLabel *Label15; // число вершин
        TLabel *Label20; // число уровней
        TLabel *Label21; // число контактов
        TLabel *Label22; // min
        TLabel *Label23; // max
        TLabel *Label24; // мощность ребер
        TLabel *Label25; // min
        TLabel *Label26; // max
        TLabel *Label16; // delimeter
        TLabel *Label17; // параметры  исходного ГГ
        TLabel *Label18; //delimeter        
        TLabel *Label12; // количество ГГ


        TButton *Button4; // старт
        TEdit *Edit7; // ввод числа вершин
        TEdit *Edit8; // ввод min контактов
        TEdit *Edit9;  // ввод max контактов
        TEdit *Edit10;  // ввод min ребер
        TEdit *Edit11;  // ввод max ребер
        TEdit *Edit12; // ввод числа уровней
        TEdit *Edit14; // ввод количества ГГ
        TProgressBar *ProgressBar2; // bottom progress bar
        TChart *Chart3; // steps chart
        TChart *Chart4; // external edges chart
        TBarSeries *Series3;
        TBarSeries *Series4;
        TRadioGroup *RadioGroup1; // сложность трассировки



        TLabel *Label13; // status label
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
