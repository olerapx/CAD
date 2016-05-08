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
        TLabel *Label1; // ����� ������
        TLabel *Label14; // ����� ��������� �� 2 ��
        TLabel *Label2; // ����� ���������
        TLabel *Label3; //min
        TLabel *Label4; //max
        TLabel *Label5; // �������� �����
        TLabel *Label6; //min
        TLabel *Label7; //max
        TLabel *Label8; //delimeter
        TLabel *Label9; // ��������� �������� ��
        TLabel *Label10; //another delimeter
        TLabel *Label11; // ���������� ��


        TButton *Button1; // ������� ��
        TButton *Button2; // ������
        TButton *Button3; // ���������������
        TEdit *Edit1; // ���� ����� ������
        TEdit *Edit2; // ���� min ���������
        TEdit *Edit3; // ���� max ���������
        TEdit *Edit4; // ���� min �����
        TEdit *Edit5; // ���� max �����
        TEdit *Edit6; // ���� ����� ��������� �� 2 ��...
        TEdit *Edit13; // ���� ���������� ��
        TRadioGroup *RadioGroup2; // ��������� �����������

// tab2

        TTabSheet *TabSheet2; //sec tab
        TPanel *Panel3; //panel with  chart & bar
        TPanel *Panel4; //panel with buttons
        TLabel *Label15; // ����� ������
        TLabel *Label20; // ����� �������
        TLabel *Label21; // ����� ���������
        TLabel *Label22; // min
        TLabel *Label23; // max
        TLabel *Label24; // �������� �����
        TLabel *Label25; // min
        TLabel *Label26; // max
        TLabel *Label16; // delimeter
        TLabel *Label17; // ���������  ��������� ��
        TLabel *Label18; //delimeter        
        TLabel *Label12; // ���������� ��


        TButton *Button4; // �����
        TEdit *Edit7; // ���� ����� ������
        TEdit *Edit8; // ���� min ���������
        TEdit *Edit9;  // ���� max ���������
        TEdit *Edit10;  // ���� min �����
        TEdit *Edit11;  // ���� max �����
        TEdit *Edit12; // ���� ����� �������
        TEdit *Edit14; // ���� ���������� ��
        TProgressBar *ProgressBar2; // bottom progress bar
        TChart *Chart3; // steps chart
        TChart *Chart4; // external edges chart
        TBarSeries *Series3;
        TBarSeries *Series4;
        TRadioGroup *RadioGroup1; // ��������� �����������



        TLabel *Label13; // caption: Label13, �������, �� ������������
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall RadioGroup1Click(TObject *Sender);
private:	// User declarations
  void initHierarhyHG ();                       // ������������� �������� ���
                                                // �������������� ���������

  void gatheringData ();                        // ���������� ��������������
                                                // ��������� � ���� ������
                                                // �� ��������� ����� ������� ������

  void showData (int Complexity);               // ����� ������ �� ������
                                                // "������� � �����" � �����������
                                                // �� ��������� ��������� �����������

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
