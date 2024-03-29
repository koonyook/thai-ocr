//---------------------------------------------------------------------------

#include <vcl.h>
#include<stdio.h>
#include<stdlib.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
AnsiString s1="C://model.txt",s2="C://answer.txt";
//---------------------------------------------------------------------------

struct infile
{
    int row;
    int *col;
    char **c;
};
typedef struct infile infile;

__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

infile *GetTextInFile(AnsiString path)   //������� enter ���º�÷Ѵ�ء��÷Ѵ
{
    FILE *f;
    char **c,x;
    int row=0,*col;

    c=(char **)malloc(sizeof(char *));
    col=(int *)malloc(sizeof(int));
    f=fopen(path.c_str(),"r");
    if(f==NULL)
    {
        return NULL;
    }
    while(1)
    {
        x='\0';
        row++;
        c=(char **)realloc(c,sizeof(char *)*row);
        col=(int *)realloc(col,sizeof(int)*row);
        c[row-1]=(char *)malloc(sizeof(char));
        col[row-1]=0;
        while(x!='\n')
        {
            fscanf(f,"%c",&x);
            if(feof(f))
            {
                free(c[row-1]);
                row--;
                c=(char **)realloc(c,sizeof(char *)*row);
                col=(int *)realloc(col,sizeof(int)*row);
                break;
            }
            else if(x!=' ' && x!='#') //������ ����Ŵ ��������дǡ
            {
                col[row-1]++;
                c[row-1]=(char *)realloc(c[row-1],sizeof(char)*col[row-1]);
                c[row-1][col[row-1]-1]=x;
            }
        }
        if(feof(f))
        {
            break;
        }
        else
        {
            c[row-1]=(char *)realloc(c[row-1],sizeof(char)*(col[row-1]+1));
            c[row-1][col[row-1]]='\0';
        }
    }
    fclose(f);
    infile *r;
    r=(infile *)malloc(sizeof(infile));
    r->row=row;
    r->col=col;
    r->c=c;
    return r;
}

int *EditDistance(char *s,int m, char *t,int n)
{
    AnsiString str;
    // d is a table with m+1 rows and n+1 columns
    int **d;
    int min;
    int i,j,cost;
    d=(int **)malloc(sizeof(int *)*m);
    for(i=0;i<m;i++)
    {
        d[i]=(int *)malloc(sizeof(int)*n);
    }
    for(i=0;i<m;i++)
    {
        str="";
        for(j=0;j<n;j++)
        {
            if(s[i]==t[j])
            {
                cost=0;
            }
            else
            {
                cost=1;
            }
            min=-1;
            if(i-1>=0 && j-1>=0)
            {
                if(d[i-1][j-1]+cost<min || min==-1)
                {
                    min=d[i-1][j-1]+cost;    // substitution
                }
                if(d[i  ][j-1]+1   <min || min==-1)
                {
                    min=d[i  ][j-1]+1;       //insertion
                }
                if(d[i-1][j  ]+1   <min || min==-1)
                {
                    min=d[i-1][j  ]+1;        //deletetion
                }
            }
            else if(i-1>=0)
            {
                min=d[i-1][j  ]+1;     //deletetion
            }
            else if(j-1>=0)
            {
                min=d[i  ][j-1]+1;     //insertion
            }
            else
            {
                min=cost; //�繨ش�������
            }
            d[i][j]=min;
            str+=IntToStr(d[i][j])+'\t';
        }
        Form1->Memo1->Lines->Add(str);
    }

    FILE *f;
    f=fopen("C://substitution.txt","a");
    char *mem; //�������������¹��Ѻ������͹��ѧ
    mem=(char *)malloc(sizeof(char));

    int insertion=0,deletetion=0,substitution=0,right=0;
    i=m-1;
    j=n-1;
    while(true)
    {
        if(i-1>=0 && j-1>=0)
        {
            if(d[i-1][j-1]<=d[i-1][j] && d[i-1][j-1]<=d[i][j-1])
            {
                i--;
                j--;
                if(d[i][j]==d[i+1][j+1])
                {
                    right++;
                }
                else
                {
                    substitution++;
                    mem=(char *)realloc(mem,sizeof(char)*substitution*2);
                    mem[substitution*2-2]=s[i+1];
                    mem[substitution*2-1]=t[j+1];
                    //fprintf(f," %c  %c \n",s[i+1],t[j+1]);
                }
            }
            else if(d[i-1][j]<=d[i][j-1])
            {
                i--;
                deletetion++;
            }
            else
            {
                j--;
                insertion++;
            }
        }
        else if(i-1>=0)
        {
            i--;
            deletetion++;
        }
        else if(j-1>=0)
        {
            j--;
            insertion++;
        }
        else
        {
            break;
        }
    }
    if(d[0][0]==0)
    {
        right++;
    }
    else if(insertion+deletetion+substitution<d[m-1][n-1])
    {
        substitution++;
        mem=(char *)realloc(mem,sizeof(char)*substitution*2);
        mem[substitution*2-2]=s[0];
        mem[substitution*2-1]=t[0];
        //fprintf(f," %c  %c \n",s[0],t[0]);
    }
    for(i=substitution-1;i>=0;i--)
    {
        fprintf(f," %c  %c \n",mem[i*2],mem[i*2+1]);
    }
    //fprintf(f,"-----------\n");
    fclose(f);
    free(mem);

    int *r;
    r=(int *)malloc(sizeof(int *)*5);
    r[0]=d[m-1][n-1];     //distance
    r[1]=insertion;
    r[2]=deletetion;
    r[3]=substitution;
    r[4]=right;
    return r;
}
int *CheckAll(infile *a,infile *b)
{
    int *r;
    int *send;
    int i;
    send=(int *)malloc(sizeof(int)*6);
    send[0]=0;
    send[1]=0;
    send[2]=0;
    send[3]=0;
    send[4]=0;
    send[5]=0;
    for(i=0;i<a->row;i++)
    {
        r=EditDistance(a->c[i],a->col[i],b->c[i],b->col[i]);
        send[0]+=r[0];
        send[1]+=r[1];
        send[2]+=r[2];
        send[3]+=r[3];
        send[4]+=r[4]-1;      //enter ����ش���¶١��� ���ź�͡
        send[5]+=a->col[i]-1;
        free(r);
    }
    return send;
}

void __fastcall TForm1::Button1Click(TObject *Sender)
{
    infile *model,*answer;
    int i;
    int *r;
    //model=GetTextInFile("C://test1.txt");
    //answer=GetTextInFile("C://test2.txt");
    model=GetTextInFile(s1);
    answer=GetTextInFile(s2);
    if(model!=NULL && answer!=NULL)
    {
        r=CheckAll(model,answer);
        Edit1->Text=IntToStr(r[0]);
        Edit2->Text=IntToStr(r[1]);
        Edit3->Text=IntToStr(r[2]);
        Edit4->Text=IntToStr(r[3]);
        Edit5->Text=IntToStr(r[4]);
        Edit6->Text=IntToStr(r[5]);

        Edit7->Text=FloatToStr(100*float(r[4])/float(r[5]));
        free(r);
    }
    else
    {
        ShowMessage("Error. Can not open file "+s1+" or "+s2);
    }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button2Click(TObject *Sender)
{
    char *ss1="sbtdufv";     //model
    char *ss2="abcdefg";      //answer
    int *r;
    Memo1->Clear();
    r=EditDistance(ss1,StrLen(ss1),ss2,StrLen(ss2));
    Edit1->Text=IntToStr(r[0]);
    Edit2->Text=IntToStr(r[1]);
    Edit3->Text=IntToStr(r[2]);
    Edit4->Text=IntToStr(r[3]);
    Edit5->Text=IntToStr(r[4]);
    Edit6->Text=IntToStr(StrLen(ss1));

    Edit7->Text=FloatToStr(100*float(r[4])/float(StrLen(ss1)));
    free(r);
}
//---------------------------------------------------------------------------




void __fastcall TForm1::Button3Click(TObject *Sender)
{
    if(OpenDialog1->Execute())
    {
        s1=OpenDialog1->FileName;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
    if(OpenDialog2->Execute())
    {
        s2=OpenDialog2->FileName;
    }
}
//---------------------------------------------------------------------------

