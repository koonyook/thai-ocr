//---------------------------------------------------------------------------

#ifndef code5H
#define code5H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ExtDlgs.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Chart.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <Series.hpp>

#include <dos.h>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TImage *Image1;
    TButton *Button1;
    TButton *Button2;
    TOpenPictureDialog *OpenPictureDialog1;
    TImage *Image2;
    TButton *Button3;
    TImage *Image3;
    TEdit *Edit1;
    TChart *Chart1;
    TBarSeries *Series1;
    TEdit *Edit2;
    TChart *Chart2;
    TBarSeries *Series2;
    TImage *Image4;
    TButton *Button4;
    TComboBox *ComboBox1;
    TChart *Chart3;
    TBarSeries *Series3;
    TButton *Button5;
    TImage *Image5;
    TButton *Button6;
    TCheckBox *CheckBox1;
    TCheckBox *CheckBox2;
    TEdit *Edit3;
        TProgressBar *ProgressBar1;
        TLabel *Label1;
        TProgressBar *ProgressBar2;
        TMemo *Memo1;
    TButton *Button7;
    TSaveDialog *SaveDialog1;
    TEdit *Edit4;
    TLabel *Label2;
    TLabel *Label3;
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall Button5Click(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall Button6Click(TObject *Sender);
    void __fastcall Button7Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
