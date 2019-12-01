//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "code5.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 *Form1;

Graphics::TBitmap *bitmap1,*bitmap2;
TRect Rect1,Rect2,Rect3,Rect5;
int mytime,minustime; //minustime=เวลาที่ใช้ในการ loadfont
int hhh,www;
const all=269;
struct picar
{
    Byte** a;
    int w,h,type;
    //type1=mono
    //type2=gray
};
struct onerow
{
    picar *z;
    int up,down,tup,tdown;

    //up,down = ตำแหน่งใน array หน้ากระดาษ
    //tup,tdown = ตำแหน่งใน array ที่ตัดมา 1 แถว

};
struct link
{
    int d;
    int **note;
};
typedef struct link link;
struct thindata
{
    picar *z;
    /*
    int loop_num;
    int *loop_juncnum;     //num of junction at each loop
    int *loop_pi,*loop_pj; //position
    int **loop_ji,**loop_jj; //junction at loop
    */
    int end_num;
    int junc_num;
    int node_num;

    link ****map;    //**change**จะเก็บข้อมูลการเชื่อมต่อแบบกราฟมีนำ้หนัก
                     //map[from][to][No.]
    int ***use;      //เอาไว้ดูว่าเส้นทางไหนได้ถูกใช้ไปแล้วบ้าง (ใช้แบบ map)
                     //ไม่มีทาง = -1 มีทางแต่ยังไม่ได้ใช้=0 มีทางและใช้แล้ว =1
    int **have;
    int *node_pi,*node_pj;
    int ***order;    //**change**จะเก็บว่า เมื่อเข้าถึงแต่ละnode แล้วจะมีลำดับวนตามเข็มนาฬิกาอย่างไร
                     //order[from][order][0] เก็บว่าไปถึงไหน (ของเดิม)
                     //order[from][order][1] เก็บว่าไปถึงด้วย No. อะไร
    int *linknode;   //เก็บว่าแต่ละ node มีจำนวนเพื่อนบ้านเท่าไหร่

    int *path;       //เก็บว่าเดินไป node ใดบ้าง
    int *pathno;     //**change** pathno[i] บอกว่า path[i] ไป path[i+1] ใช้เส้นทางใด
    int path_num;
    int path_long;
    int atlast;   //จบอย่างไร 1 จบที่ปลาย 2 จบที่การซ้ำ

    int **branch;
    int **branchno;
    int *branch_num;
    int *branch_long;
    int *branch_last;

    
};
typedef struct thindata thindata;
struct alpha
{
    picar *z;
    int up,down,type;
    //type0=enter
    //type1=low
    //type2=middle
    //type3=high
    //type4=higher     //แก้แล้ว
    char c[3];        ///มาเพิ่มทีหลัง
    int ans;
    int gap;
    thindata *thin;
};
struct bound
{
    int up,down,left,right;
};

struct feature
{
    picar *z;
    int up,down,left;
};
typedef struct feature feature;
struct myfont
{
    alpha *ascii[269];
    int have[269];
    AnsiString fontname;
    bool bold,italic;
    int h,size,gap;
    feature **test;
};

struct walkpath
{
    int d,n;
    int *path,*pathno;
};
typedef struct walkpath walkpath;

typedef struct myfont myfont;

typedef struct picar picar;
typedef struct onerow onerow;
typedef struct alpha alpha;
typedef struct bound bound;
struct paragraph
{
    alpha ***c;
    int *row;
    int *num;
};
typedef struct paragraph paragraph;

void ShowHis(int *a,int s,int e,TBarSeries *series);
picar *NewPicar(int h,int w,int type);
Byte **NewArray(int h,int w);
void FreePicArray(void **a,int h);
void FreePicar(picar *z);
picar *PicToArray01(Graphics::TBitmap *bitmap);
picar *PicToArray256(Graphics::TBitmap *bitmap);
void ShowImage(picar *x,TImage *image,TRect rect);
void SavePicar(picar *x,AnsiString str);
void SaveArray(Byte **a,int h,int w,AnsiString str);
picar *RotateArrayBig(picar *z,float deg);
void GrayToMono(picar *z,Byte the);
void CheckBlackWhite(picar *z);
float FindRotate(picar *z);
void Del1Noise(picar *z);
void erosion(picar *z);
void dilation(picar *z);
int count(picar *z,int i,int j);
void change(picar *z,int i,int j,int old,int color,int direction);
//void change(onerow *z,int i,int j,int old,int color);
void clear(picar *z,int i,int j);
void ClearImage(TImage *z,int i,int j,int bup,int bdown,int bleft,int bright);
void DelNoiseHis(picar *z);
picar **DivideRow(picar *z,int *row);
onerow **DivideRow2(picar *z,int *row);
//void FindBound(onerow *z,int i,int j,bound *b);
void FindBound(picar *z,int i,int j,bound *b,int old,int color);
void FindBoundFont(picar *z,int i,int j,bound *b);
alpha *suck(onerow *z,int ai,int aj,bound *b,int p);
alpha *suck(picar *z,int up,int down,int ai,int aj,bound *b,int p);
alpha *SuckFont(picar *z,int ai,int aj,int up,int down);
alpha **GetAlpha(onerow *z,int *x);
int InRight(picar *z);
int InLeft(picar *z);
int InUp(picar *z);
int InDown(picar *z);
picar *GetInBound(alpha *ascii,int ini,int ini2,int inj,int inj2);
void GetFeature(feature *test,picar *mem,int up,int down);
myfont *LoadFont(AnsiString name,bool bold,bool italic,int high,TImage *image);
void FreeFont(myfont *f);
float compare(alpha *n,myfont *f,int testnum);
float distance(alpha *m,alpha *n);
float distance2(alpha *m,alpha *n);
int CompareSize1(int *point,alpha *n,myfont *f);
int CompareSize2(int *point,alpha *n,myfont *f);
char IntToCharRead(int x);
void ChooseLevel2(int *point,alpha *n,myfont *f);
char **MakeDoc(alpha ***c,int row,int *num,int *gap);
picar *thining(picar *z);
void FindLongestMain2(thindata *t);
void FreeThindata(thindata *t);
picar *DrawPath(thindata *t);
void ViewInFile(picar *z);
float DTW(link *m,link *n,int *k);
link *MakePath1(thindata *t);
link *MakePath2(thindata *t);
link *MakePath3(thindata *t);

void ShowHis(int *a,int s,int e,TBarSeries *series)
{

    series->Clear();
    for(;s<e;s++)
        series->AddXY(s,a[s],s,clRed);
}

picar *NewPicar(int h,int w,int type)
{
    Byte **a;
    int i,j;
    picar *r;
    //malloc
    a=(Byte **)malloc(sizeof(Byte *)*h);
    r=(picar *)malloc(sizeof(picar));
    if(a!=NULL && r!=NULL)
    {
        for(i=0;i<h;i++)
        {
            a[i]=(Byte *)malloc(sizeof(Byte)*w);
            if(a[i]==NULL)
            {
                ShowMessage("cannot malloc");
                break;
            }
        }
        if(i<h)
        {
            for(;i>=0;i--)
            {
                free(a[i]);
            }
            free(a);
            return NULL;
        }
    }
    else
    {
        ShowMessage("cannot malloc");
        return NULL;
    }

    //clear
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
           a[i][j]=0;
        }
    }

    r->a=a;
    r->h=h;
    r->w=w;
    r->type=type;
    return r;
}
Byte **NewArray(int h,int w)
{
    Byte **a;
    int i,j;
    //malloc
    a=(Byte **)malloc(sizeof(Byte *)*h);
    if(a!=NULL)
    {
        for(i=0;i<h;i++)
        {
            a[i]=(Byte *)malloc(sizeof(Byte)*w);
            if(a[i]==NULL)
            {
                ShowMessage("cannot malloc");
                break;
            }
        }
        if(i<h)
        {
            for(;i>=0;i--)
            {
                free(a[i]);
            }
            free(a);
            return NULL;
        }
    }
    else
    {
        ShowMessage("cannot malloc");
        return NULL;
    }

    //clear
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
           a[i][j]=0;
        }
    }
    return a;
}
void FreePicArray(void **a,int h)
{
    int i;
    for(i=0;i<h;i++)
    {
        free(a[i]);
    }
    free(a);
}
void FreePicar(picar *z)
{
    FreePicArray((void **)(z->a),z->h);
    free(z);
}
picar *PicToArray01(Graphics::TBitmap *bitmap)
{
    int w,h,w2,at;
    int i,j,k,c,e[8];
    Byte **a;
    picar *x;

    if(bitmap->PixelFormat==pf1bit)
    {
        h=bitmap->Height;
        w=bitmap->Width;
        x=NewPicar(h,w,1);
        if(x==NULL)
        {
            return NULL;
        }
        a=x->a;

        for(i=0;i<h;i++)
        {
            if(w%8>0)
            {
                w2=w/8+1;
            }
            else
            {
                w2=w/8;
            }
            at=0;
            for(j=0;j<w2;j++)
            {
                c=(int)(((unsigned char *)(bitmap->ScanLine[i]))[j]);
                for(k=7;k>=0;k--)
                {
                    e[k]=c%2;
                    c=c/2;
                }
                for(k=0;k<8&&at<w;k++,at++)
                {
                    a[i][at]=e[k];

                }
            }

        }
        return x;

    }
    else
    {
        ShowMessage("This is not 1bit image");
        return NULL;
    }
}

picar *PicToArray256(Graphics::TBitmap *bitmap)
{
    int w,h,w2,at;
    int i,j,k,c,e[8];
    Byte **a;
    picar *x;

    if(bitmap->PixelFormat==pf8bit)
    {

        h=bitmap->Height;
        w=bitmap->Width;

        x=NewPicar(h,w,2);
        if(x==NULL)
        {
            return NULL;
        }
        a=x->a;
        /*
        a=(Byte **)malloc(sizeof(Byte *)*h);
        if(a!=NULL)
        {
            for(i=0;i<h;i++)
            {
                a[i]=(Byte *)malloc(sizeof(Byte)*w);
                if(a[i]==NULL)
                {
                    ShowMessage("cannot malloc");
                    break;
                }
            }
            if(i<h)
            {
                for(;i>=0;i--)
                {
                    free(a[i]);
                }
                free(a);
                return NULL;
            }
        }
        else
        {
            ShowMessage("cannot malloc");
            return NULL;
        }
        */

        for(i=0;i<h;i++)
        {
            for(j=0;j<w;j++)
            {
                a[i][j]=((Byte *)(bitmap->ScanLine[i]))[j];

            }

        }
        //ViewInFile(x);
        return x;
        /*
        x=(picar *)malloc(sizeof(picar));
        if(x!=NULL)
        {
            x->a=a;
            x->w=w;
            x->h=h;
            x->type=2;
            return x;
        }
        else
        {
            return NULL;
        }
        */
    }
    else if(bitmap->PixelFormat==pf24bit)
    {
        h=bitmap->Height;
        w=bitmap->Width;
        x=NewPicar(h,w,2);
        if(x==NULL)
        {
            return NULL;
        }
        a=x->a;
        /*
        a=(Byte **)malloc(sizeof(Byte *)*h);
        if(a!=NULL)
        {
            for(i=0;i<h;i++)
            {
                a[i]=(Byte *)malloc(sizeof(Byte)*w);
                if(a[i]==NULL)
                {
                    ShowMessage("cannot malloc");
                    break;
                }
            }
            if(i<h)
            {
                for(;i>=0;i--)
                {
                    free(a[i]);
                }
                free(a);
                return NULL;
            }
        }
        else
        {
            ShowMessage("cannot malloc");
            return NULL;
        }
        */
        for(i=0;i<h;i++)
        {
            for(j=0;j<w;j++)
            {
                                                                //luma transform ITU-R 601-2

                c=(
                 ((Byte *)(bitmap->ScanLine[i]))[j*3]   *0.144    //B
                +((Byte *)(bitmap->ScanLine[i]))[j*3+1] *0.587    //G
                +((Byte *)(bitmap->ScanLine[i]))[j*3+2] *0.299    //R
                 );
                if(c>255)
                {
                    a[i][j]=255;
                }
                else
                {
                    a[i][j]=c;
                }
                /*   เฉลี่ย
                a[i][j]=(
                 ((Byte *)(bitmap->ScanLine[i]))[j*3]       //B
                +((Byte *)(bitmap->ScanLine[i]))[j*3+1]     //G
                +((Byte *)(bitmap->ScanLine[i]))[j*3+2]     //R
                 )/3;
                */
            }
        }
        return x;
        /*
        picar *x;
        x=(picar *)malloc(sizeof(picar));
        if(x!=NULL)
        {
            x->a=a;
            x->w=w;
            x->h=h;
            x->type=2;
            return x;
        }
        else
        {
            return NULL;
        }
        */
    }
    else if(bitmap->PixelFormat==pf4bit)
    {
        h=bitmap->Height;
        w=bitmap->Width;
        x=NewPicar(h,w,2);
        if(x==NULL)
        {
            return NULL;
        }
        a=x->a;
        /*
        a=(Byte **)malloc(sizeof(Byte *)*h);
        if(a!=NULL)
        {
            for(i=0;i<h;i++)
            {
                a[i]=(Byte *)malloc(sizeof(Byte)*w);
                if(a[i]==NULL)
                {
                    ShowMessage("cannot malloc");
                    break;
                }
            }
            if(i<h)
            {
                for(;i>=0;i--)
                {
                    free(a[i]);
                }
                free(a);
                return NULL;
            }
        }
        else
        {
            ShowMessage("cannot malloc");
            return NULL;
        }
        */
        for(i=0;i<h;i++)
        {
            if(w%2==1)
            {
                w2=w/2+1;
            }
            else
            {
                w2=w/2;
            }
            at=0;
            for(j=0;j<w2;j++)
            {
                c=(int)(((unsigned char *)(bitmap->ScanLine[i]))[j]);
                for(k=7;k>=0;k--)
                {
                    e[k]=c%2;
                    c=c/2;
                }
                a[i][at]=(e[3]+e[2]*2+e[1]*4+e[0]*8)*17;

                at++;
                if(at<w)
                {
                    a[i][at]=(e[7]+e[6]*2+e[5]*4+e[4]*8)*17;

                    at++;
                }
            }
        }
        return x;
        /*
        x=(picar *)malloc(sizeof(picar));
        if(x!=NULL)
        {
            x->a=a;
            x->w=w;
            x->h=h;
            x->type=2;
            return x;
        }
        else
        {
            return NULL;
        }
        */
    }
    else if(bitmap->PixelFormat==pf1bit)
    {
        return PicToArray01(bitmap);
    }
    else
    {
        ShowMessage("-_- ERROR -_-");
        return NULL;
    }
}

void ShowImage(picar *x,TImage *image,TRect rect)
{
    Graphics::TBitmap *bitmaptem;
    bitmaptem = new Graphics::TBitmap;
    bitmaptem->PixelFormat=pf24bit;
    bitmaptem->Width=x->w;
    bitmaptem->Height=x->h;
    int i,j;
    if(x->type==1)
    {
        for(i=0;i<x->h;i++)
        {
            for(j=0;j<x->w;j++)
            {
                if(x->a[i][j]==1)
                {
                ((Byte *)(bitmaptem->ScanLine[i]))[j*3]
                =((Byte *)(bitmaptem->ScanLine[i]))[j*3+1]
                =((Byte *)(bitmaptem->ScanLine[i]))[j*3+2]
                =0;
                }
            }
        }
    }
    else
    {
        for(i=0;i<x->h;i++)
        {
            for(j=0;j<x->w;j++)
            {
                ((Byte *)(bitmaptem->ScanLine[i]))[j*3]
                =((Byte *)(bitmaptem->ScanLine[i]))[j*3+1]
                =((Byte *)(bitmaptem->ScanLine[i]))[j*3+2]
                =(x->a[i][j]);
            }
        }
    }
    image->Canvas->StretchDraw(rect,bitmaptem);
    delete bitmaptem;
}
void SavePicar(picar *x,AnsiString str)
{
    Graphics::TBitmap *bitmaptem;
    bitmaptem = new Graphics::TBitmap;
    bitmaptem->PixelFormat=pf24bit;
    bitmaptem->Width=x->w;
    bitmaptem->Height=x->h;
    int i,j;
    if(x->type==1)
    {
        for(i=0;i<x->h;i++)
        {
            for(j=0;j<x->w;j++)
            {
                if(x->a[i][j]==1)
                {
                ((Byte *)(bitmaptem->ScanLine[i]))[j*3]
                =((Byte *)(bitmaptem->ScanLine[i]))[j*3+1]
                =((Byte *)(bitmaptem->ScanLine[i]))[j*3+2]
                =0;
                }
            }
        }
    }
    else
    {
        for(i=0;i<x->h;i++)
        {
            for(j=0;j<x->w;j++)
            {
                ((Byte *)(bitmaptem->ScanLine[i]))[j*3]
                =((Byte *)(bitmaptem->ScanLine[i]))[j*3+1]
                =((Byte *)(bitmaptem->ScanLine[i]))[j*3+2]
                =(x->a[i][j]);
            }
        }
    }
    bitmaptem->SaveToFile(str);
    delete bitmaptem;
}

void SaveArray(Byte **a,int h,int w,AnsiString str)
{
    Graphics::TBitmap *bitmaptem;
    bitmaptem = new Graphics::TBitmap;
    bitmaptem->PixelFormat=pf24bit;
    bitmaptem->Width=w;
    bitmaptem->Height=h;
    int i,j;

        for(i=0;i<h;i++)
        {
            for(j=0;j<w;j++)
            {
                if(a[i][j]==1)
                {
                ((Byte *)(bitmaptem->ScanLine[i]))[j*3]
                =((Byte *)(bitmaptem->ScanLine[i]))[j*3+1]
                =((Byte *)(bitmaptem->ScanLine[i]))[j*3+2]
                =0;
                }
            }
        }

    bitmaptem->SaveToFile(str);
    delete bitmaptem;
}
/*
bool ShowImageTrueSize(picar *x,TImage *image,Byte tone)  //tone 0:1bit 1:256red 2:256gray
{
    int i,j,h=x->h,w=x->w;
    Byte **pic=x->a;
    AnsiString str=" ";
    if(pic!=NULL && image!=NULL)
    {

        image->Height=h;
        image->Width=w;

        image->Canvas->Pen->Color=clWhite;
        image->Canvas->Rectangle(0,0,w,h);
        if(tone==0)
        {
            for(i=0;i<h;i++)
            {
                for(j=0;j<w;j++)
                {
                    if(pic[i][j]!=0)
                        //image->Canvas->Pixels[j][i]=clWhite;
                    //else
                        image->Canvas->Pixels[j][i]=clBlack;
                }
            }
        }
        else if(tone==1)
        {
            for(i=0;i<h;i++)
            {
                for(j=0;j<w;j++)
                {
                    image->Canvas->Pixels[j][i]=pic[i][j];
                    /*
                    if(image->Canvas->Pixels[j][i]!=pic[i][j])
                    {
                        str=IntToStr(i)+","+IntToStr(j)+" = "+IntToStr(pic[i][j])+" -> "+IntToStr(image->Canvas->Pixels[j][i]);
                        ShowMessage(str);
                    }
                    */
                    //show in red and black but faster than use tone=2
/*                }
            }
        }
        else if(tone==2)
        {
            Byte n,hex[2];
            for(i=0;i<h;i++)
            {
                for(j=0;j<w;j++)
                {
                    n=pic[i][j];
                    hex[0]=n%16;
                    n=n/16;
                    hex[1]=n%16;
                    image->Canvas->Pixels[j][i]=
                    hex[1]*pow(16,5)+hex[0]*pow(16,4)+
                    hex[1]*pow(16,3)+hex[0]*pow(16,2)+
                    hex[1]*pow(16,1)+hex[0];
                }
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}
*/
/*
Byte **RotateArraySmall(Byte **map,int h,int w,float deg)
/// ยังไม่ได้แก้เป็น picar
{
    Byte **a;
    int i,j;

    a=(Byte **)malloc(sizeof(Byte *)*h);
    if(a!=NULL)
    {
        for(i=0;i<h;i++)
        {
            a[i]=(Byte *)malloc(sizeof(Byte)*w);
            if(a[i]==NULL)
            {
                ShowMessage("cannot malloc");
                break;
            }
        }
        if(i<h)
        {
            for(;i>=0;i--)
            {
                free(a[i]);
            }
            free(a);
            return NULL;
        }
    }
    else
    {
        ShowMessage("cannot malloc");
        return NULL;
    }
    //  (3.1415926535897932384626433832795)
    float seta=deg*M_PI/180;
    float si=sin(seta),co=cos(seta);
    int centery=h/2,centerx=w/2;
    int xnew,ynew;
    //clear
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
           a[i][j]=0;
        }
    }
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
            xnew=co*(j-centerx)-si*(i-centery)+centerx;
            ynew=si*(j-centerx)+co*(i-centery)+centery;
            if(xnew>=0&&xnew<w&&ynew>=0&&ynew<h)
            {

                    if(a[ynew][xnew]!=0)
                    {
                        //ShowMessage(IntToStr(i*w+j)+" on "+IntToStr(a[ynew][xnew]));
                    }
                    else
                    {
                        a[ynew][xnew]=map[i][j];          //// map[i][j]
                    }

            }

        }
    }
    return a;
}
*/
picar *RotateArrayBig(picar *z,float deg)
{
    Byte **a,**map=z->a;
    int i,j,h=z->h,w=z->w;
    //  (3.1415926535897932384626433832795)
    float seta=deg*M_PI/180;
    float si=sin(seta),co=cos(seta);
    int centery=h/2,centerx=w/2;
    int xnew,ynew;
    int wmax,wmin,hmax,hmin,truew,trueh;
    picar *r;
    //find max width
    wmax=co*(0-centerx)-si*(0-centery)+centerx;
    wmin=wmax;
    xnew=co*(w-1-centerx)-si*(0-centery)+centerx;
    if(xnew>wmax)
    {
        wmax=xnew;
    }
    else if(xnew<wmin)
    {
        wmin=xnew;
    }
    xnew=co*(w-1-centerx)-si*(h-1-centery)+centerx;
    if(xnew>wmax)
    {
        wmax=xnew;
    }
    else if(xnew<wmin)
    {
        wmin=xnew;
    }
    xnew=co*(0-centerx)-si*(h-1-centery)+centerx;
    if(xnew>wmax)
    {
        wmax=xnew;
    }
    else if(xnew<wmin)
    {
        wmin=xnew;
    }
    truew=wmax-wmin+1;
    //find max higth
    hmax=si*(0-centerx)+co*(0-centery)+centery;
    hmin=hmax;
    ynew=si*(w-1-centerx)+co*(0-centery)+centery;
    if(ynew>hmax)
    {
        hmax=ynew;
    }
    else if(ynew<hmin)
    {
        hmin=ynew;
    }
    ynew=si*(w-1-centerx)+co*(h-1-centery)+centery;
    if(ynew>hmax)
    {
        hmax=ynew;
    }
    else if(ynew<hmin)
    {
        hmin=ynew;
    }
    ynew=si*(0-centerx)+co*(h-1-centery)+centery;
    if(ynew>hmax)
    {
        hmax=ynew;
    }
    else if(ynew<hmin)
    {
        hmin=ynew;
    }
    trueh=hmax-hmin+1;
    //malloc
    r=NewPicar(trueh,truew,z->type);
    if(r==NULL)
        return NULL;
    a=r->a;
    /*
    a=(Byte **)malloc(sizeof(Byte *)*trueh);
    if(a!=NULL)
    {
        for(i=0;i<trueh;i++)
        {
            a[i]=(Byte *)malloc(sizeof(Byte)*truew);
            if(a[i]==NULL)
            {
                ShowMessage("cannot malloc");
                break;
            }
        }
        if(i<trueh)
        {
            for(;i>=0;i--)
            {
                free(a[i]);
            }
            free(a);
            return NULL;
        }
    }
    else
    {
        ShowMessage("cannot malloc");
        return NULL;
    }

    //clear
    for(i=0;i<trueh;i++)
    {
        for(j=0;j<truew;j++)
        {
           a[i][j]=0;
        }
    }
    */
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
            if(map[i][j]>0)
            {
            xnew=co*(j-centerx)-si*(i-centery)+centerx -wmin;
            ynew=si*(j-centerx)+co*(i-centery)+centery -hmin;
            //if(xnew>=0 && xnew<truew && ynew>=0 && ynew<trueh)
            //{
                    //if(a[ynew][xnew]!=0)
                    {
                        //ShowMessage(IntToStr(i*w+j)+" on "+IntToStr(a[ynew][xnew]));
                    }
                    //else
                    {
                        a[ynew][xnew]=map[i][j];
                    }
            //}
            //else
            //{
            //    ShowMessage("("+IntToStr(xnew)+","+IntToStr(ynew)+") , ("+IntToStr(j)+","+IntToStr(i)+")" );
            //}
            }
        }
    }
    /*
    if(bitmap1->PixelFormat==pf1bit)
        ShowImage(a,Image3,trueh,truew,0);
    else
        ShowImage(a,Image3,trueh,truew,1);
    */
    /*
    picar *r;
    r=(picar *)malloc(sizeof(picar));
    r->h=trueh;
    r->w=truew;
    r->a=a;
    r->type=z->type;
    */
    return r;
}
void GrayToMono(picar *z,Byte the)      //for gray only
{
    Byte **a=z->a;
    int i,j,h=z->h,w=z->w;
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
            if(a[i][j]>the)
            {
                a[i][j]=0;
            }
            else
            {
                a[i][j]=1;
            }
        }
    }
    z->type=1;
}
void adaptive(picar *z,int dh,int dw)
{
    byte **a=z->a;
    int i,j,k,l;
    int h=z->h,w=z->w;
    int up,down,left,right;
    int threshold,sum,mean,max,min;
    for(k=0;k<dh;k++)
    {
        for(l=0;l<dw;l++)
        {
            //จำกัดเขต บน ล่าง ซ้าย ขวา
            up=k*(int(h/dh));
            left=l*(int(w/dw));
            if(k==dh-1) //ถึงล่างสุดแล้ว
            {
                down=h-1;
            }
            else
            {
                down=(k+1)*(int(h/dh)) -1;
            }
            if(l==dw-1)
            {
                right=w-1;
            }
            else
            {
                right=(l+1)*(int(w/dw)) -1;
            }
            //หาค่า threshold
            sum=0;
            max=0;
            min=255;
            for(i=up;i<=down;i++)
            {
                for(j=left;j<=right;j++)
                {
                    sum+=a[i][j];
                    if(a[i][j]<min)
                    {
                        min=a[i][j];
                    }
                    else if(a[i][j]>max)
                    {
                        max=a[i][j];
                    }
                }
            }
            mean=sum/((right-left+1)*(down-up+1));
            threshold=mean-(max-mean);       //เสริมให้ดูดีขึ้น
            //ทำให้เป็นขาวดำ
            for(i=up;i<=down;i++)
            {
                for(j=left;j<=right;j++)
                {
                    if(a[i][j]>threshold)
                    {
                        a[i][j]=0;
                    }
                    else
                    {
                        a[i][j]=1;
                    }
                }
            }
        }
    }
    z->type=1;
}
void CheckBlackWhite(picar *z)
{
    unsigned int one=0;
    Byte **a=z->a;
    int i,j,h=z->h,w=z->w;
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
            if(a[i][j]==1)
            {
                one++;
            }
        }
    }
    if(one>h*w/2)
    {
        for(i=0;i<h;i++)
        {
            for(j=0;j<w;j++)
            {
                if(a[i][j]==1)
                {
                    a[i][j]=0;
                }
                else
                {
                    a[i][j]=1;
                }
            }
        }
    }
}


float FindRotate(picar *z)
{
    int i,j;
    int h,w;
    //int maxgroup=0,minlen=z->h,len,group;
    int *his;
    float truedeg,deg,n,m;
    Byte **a;
    picar *ro;
    int maxup,maxdown,maxslope=0,slope; //มาเพิ่มทีหลังเพื่อแก้ algorithm
    for(deg=-5;deg<=5;deg+=1)        /////////////////////
    {
        ro=RotateArrayBig(z,deg);
        if(ro!=NULL)
        {
            h=ro->h;
            w=ro->w;
            a=ro->a;
            his=(int *)malloc(sizeof(int)*h);
            if(his!=NULL)
            {
                for(i=0;i<h;i++)
                {
                    his[i]=0;
                    for(j=0;j<w;j++)
                    {
                        his[i]+=a[i][j];
                    }
                }
                his[0]=0;
                //len=0;
                //group=0;
                /*
                for(i=0;i<h;i++)
                {
                    his[i]-=3;
                    if(his[i]<0)
                        his[i]=0;
                    else
                        len++;
                    if(his[i]<=0&&his[i+1]>0&&i!=h-1)
                    {
                        group++;
                    }
                }
                group=maxgroup; //*******************
                */

                //หาขาขึ้น มากสุด และ ลงมากสุด
                maxup=0;
                maxdown=0;
                for(i=0;i<h-1;i++)
                {
                    slope=his[i+1]-his[i];
                    if(slope>maxup)
                    {
                        maxup=slope;
                    }
                    else if(slope<maxdown)
                    {
                        maxdown=slope;
                    }
                }
                slope=maxup-maxdown;
                //หาเสร็จแล้ว
                /*
                if(group>maxgroup)
                {
                    maxgroup=group;
                    minlen=len;
                    truedeg=deg;
                }
                else*/ if(/*group==maxgroup &&*/ slope>maxslope)
                {
                    maxslope=slope;
                    truedeg=deg;
                }
                /*
                else if(group==maxgroup && slope==maxslope && len<minlen)
                {
                    minlen=len;
                    truedeg=deg;
                }
                */
                else if(/*group==maxgroup &&*/ slope==maxslope/* && len==minlen */)
                {
                    if(deg<0)
                    {
                        m=deg*(-1);
                    }
                    else
                    {
                        m=deg;
                    }
                    if(truedeg<0)
                    {
                        n=truedeg*(-1);
                    }
                    else
                    {
                        n=truedeg;
                    }
                    if(m<n)
                    {
                        truedeg=deg;
                    }
                }
                free(his);
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            return NULL;
        }
        FreePicar(ro);
    }
    Form1->ProgressBar1->StepBy(2);            //คุงแก้
    Form1->ProgressBar1->Refresh();
    //again in specific drgree
    float bound=truedeg+0.75;
    for(deg=truedeg-0.75;deg<=bound;deg+=0.25)
    {
        ro=RotateArrayBig(z,deg);
        if(ro!=NULL)
        {
            h=ro->h;
            w=ro->w;
            a=ro->a;
            his=(int *)malloc(sizeof(int)*h);
            if(his!=NULL)
            {

                for(i=0;i<h;i++)
                {
                    his[i]=0;
                    for(j=0;j<w;j++)
                    {
                        his[i]+=a[i][j];
                    }
                }
                his[0]=0;
                //หาขาขึ้น มากสุด และ ลงมากสุด
                maxup=0;
                maxdown=0;
                for(i=0;i<h-1;i++)
                {
                    slope=his[i+1]-his[i];
                    if(slope>maxup)
                    {
                        maxup=slope;
                    }
                    else if(slope<maxdown)
                    {
                        maxdown=slope;
                    }
                }
                slope=maxup-maxdown;
                //หาเสร็จแล้ว
                if(slope>maxslope)
                {
                    maxslope=slope;
                    truedeg=deg;
                }

                else if(slope==maxslope)
                {
                    if(deg<0)
                    {
                        m=deg*(-1);
                    }
                    else
                    {
                        m=deg;
                    }
                    if(truedeg<0)
                    {
                        n=truedeg*(-1);
                    }
                    else
                    {
                        n=truedeg;
                    }
                    if(m<n)
                    {
                        truedeg=deg;
                    }
                }
                free(his);
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            return NULL;
        }
        FreePicar(ro);
    }
    Form1->ProgressBar1->StepBy(2);            //คุงแก้
    Form1->ProgressBar1->Refresh();
    return truedeg;
}
void Del1Noise(picar *z)
//use k-filter method
//เวลาใช้ต้องซำ้หลายรอบ ไม่ค่อยดีเท่าไหร่
//มีผลกระทบต่อตัวอักษรเยอะอยู่
{
    int i,j,h=z->h,w=z->w,n,c,r;
    Byte **a=z->a;
    picar *newpic;
    newpic=NewPicar(h,w,z->type);
    for(i=1;i<h-1;i++)
    {
        for(j=1;j<w-1;j++)
        {

                n=a[i-1][j-1]+a[i][j-1]+a[i-1][j]
                +a[i+1][j+1]+a[i][j+1]+a[i+1][j]
                +a[i+1][j-1]+a[i-1][j+1];

                r=a[i-1][j-1]+a[i+1][j-1]+a[i-1][j+1]+a[i+1][j+1];

                c=0;
                if(a[i-1][j-1]!=a[i-1][j]   ) c++  ;
                if(a[i-1][j]  !=a[i-1][j+1] ) c++  ;
                if(a[i-1][j+1]!=a[i]  [j+1] ) c++  ;
                if(a[i]  [j+1]!=a[i+1][j+1] ) c++  ;
                if(a[i+1][j+1]!=a[i+1][j]   ) c++  ;
                if(a[i+1][j]  !=a[i+1][j-1] ) c++  ;
                if(a[i+1][j-1]!=a[i]  [j-1] ) c++  ;
                if(a[i]  [j-1]!=a[i-1][j-1] ) c++  ;
                c=c/2;

                if(c<=1 && r>=2 && n>=5)
                {
                    newpic->a[i][j]=1;
                }
                else if(c<=1 && 4-r>=2 && 8-n>=5)
                {
                    newpic->a[i][j]=2;
                }
        }
    }
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
            if(newpic->a[i][j]==1)
                z->a[i][j]=newpic->a[i][j];
            else if(newpic->a[i][j]==2)
                z->a[i][j]=0;
        }
    }
    FreePicar(newpic);
}
void erosion(picar *z)           //ลบขอบ
{
    int i,j,h=z->h,w=z->w;
    Byte **a=z->a;

    for(i=1;i<h-1;i++)
    {
        for(j=1;j<w-1;j++)
        {

            if(a[i][j]==1)
            {
                if(
                a[i-1][j-1]==0 ||
                a[i][j-1]==0   ||
                a[i-1][j]==0   ||
                a[i+1][j+1]==0 ||
                a[i][j+1]==0   ||
                a[i+1][j]==0   ||
                a[i+1][j-1]==0 ||
                a[i-1][j+1]==0
                )
                a[i][j]=2;
            }
        }
    }
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
            if(a[i][j]==2 || i==0 || j==0 || i==h-1 || j==w-1)
            {
                a[i][j]=0;
            }
        }
    }
}
void dilation(picar *z)         //เพิ่มขอบ
{
    int i,j,h=z->h,w=z->w;
    Byte **a=z->a;

    for(i=1;i<h-1;i++)
    {
        for(j=1;j<w-1;j++)
        {

            if(a[i][j]==1)
            {
                if(a[i-1][j-1]==0)
                    a[i-1][j-1]=2;
                if(a[i][j-1]==0)
                    a[i][j-1]=2;
                if(a[i-1][j]==0)
                    a[i-1][j]=2;
                if(a[i+1][j+1]==0)
                    a[i+1][j+1]=2;
                if(a[i][j+1]==0)
                    a[i][j+1]=2;
                if(a[i+1][j]==0)
                    a[i+1][j]=2;
                if(a[i+1][j-1]==0)
                    a[i+1][j-1]=2;
                if(a[i-1][j+1]==0)
                    a[i-1][j+1]=2;
            }
        }
    }
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
            if(a[i][j]==2)
            {
                a[i][j]=1;
            }
        }
    }
}

Byte **pta_wn(Byte **a,int w,int h,int k)//use for thining function only
{
        int i,j,m;
        int sum;
        Byte **y;

        y = NewArray(h+4,w+4);
        if(k==1)
        {
            for(i=2;i<=w+1;i++)
            {
                for(j=2;j<=h+1;j++)
                {
                     sum=0;
                     for(m=0;m<8;m++)
                     {
                          if(m==0)
                          {
                            sum += a[j][i-1];
                          }
                          else if(m==1)
                          {
                            sum += a[j+1][i-1]*2;
                          }
                          else if(m==2)
                          {
                            sum += a[j+1][i]*4;
                          }
                          else if(m==3)
                          {
                            sum += a[j+1][i+1]*8;
                          }
                          else if(m==4)
                          {
                            sum += a[j][i+1]*16;
                          }
                          else if(m==5)
                          {
                            sum += a[j-1][i+1]*32;
                          }
                          else if(m==6)
                          {
                            sum += a[j-1][i]*64;
                          }
                          else if(m==7)
                          {
                            sum += a[j-1][i-1]*128;
                          }
                          y[j][i]=sum;
                     }
                }
            }
        }

        else if(k==2)
        {
            for(i=2;i<=w+1;i++)
            {
                for(j=2;j<=h+1;j++)
                {
                   sum=0;
                   for(m=0;m<8;m++)
                   {
                        if(m==0)
                        {
                          sum += a[j][i+1];
                        }
                        else if(m==1)
                        {
                          sum += a[j-1][i+1]*2;
                        }
                        else if(m==2)
                        {
                          sum += a[j-1][i]*4;
                        }
                        else if(m==3)
                        {
                          sum += a[j-1][i-1]*8;
                        }
                        else if(m==4)
                        {
                          sum += a[j][i-1]*16;
                        }
                        else if(m==5)
                        {
                          sum += a[j+1][i-1]*32;
                        }
                        else if(m==6)
                        {
                          sum += a[j+1][i]*64;
                        }
                        else if(m==7)
                        {
                          sum += a[j+1][i+1]*128;
                        }
                        y[j][i]=sum;
                   }
                }
            }
        }
        return y;
}
picar *skeleton(picar *z)
{
        int i,j;
        Byte **a,**b,**c,**x,**y,**pre;
        int w=z->h,h=z->w;

        int round=0;
        int check;

        /*
        a = (int **) calloc(h+2, sizeof(int *));
        b = (int **) calloc(h+2, sizeof(int *));
        c = (int **) calloc(h+2, sizeof(int *));
        x = (int **) calloc(h+2, sizeof(int *));
        for(i=0;i<h+2;i++)
        {
                a[i] = (int *)calloc(w+2,sizeof(int));
                b[i] = (int *)calloc(w+2,sizeof(int));
                c[i] = (int *)calloc(w+2,sizeof(int));
                x[i] = (int *)calloc(w+2,sizeof(int));
        }
        */

        a=NewArray(w+4,h+4);
        a =  NewArray(w+4,h+4);
        b =  NewArray(w+4,h+4);
        c = NewArray(w+4,h+4);
        x =  NewArray(w+4,h+4);
        y =  NewArray(w+4,h+4);
        pre = NewArray(w+4,h+4);


        for(i=0;i<w+4;i++)
        {
                for(j=0;j<h+4;j++)
                {
                        if(i>=2 && i<w+2 && j>=2 && j<h+2)
                        {
                             x[i][j]=z->a[i-2][j-2];
                             pre[i][j]=z->a[i-2][j-2];
                        }
                        else
                        {
                             x[i][j]=0;
                             pre[i][j]=0;
                        }
                }
        }
        check=1;

        while(check)
        {
              for(i=2;i<w+2;i++)
              {
                      for(j=2;j<h+2;j++)
                      {
                         a[i][j]=x[i-1][j] && x[i][j+1];
                      }
              }

              for(i=2;i<w+2;i++)
              {
                      for(j=2;j<h+2;j++)
                      {
                              a[i][j] = a[i][j] && a[i+1][j-1] && x[i][j];
                      }
              }

              for(i=2;i<w+2;i++)
              {
                      for(j=2;j<h+2;j++)
                      {
                               b[i][j] = (a[i+1][j] && (!x[i-1][j])) ||
                                              (a[i-1][j] && (!x[i+1][j])) ||
                                              (a[i][j+1] && (!x[i][j-1])) ||
                                              (a[i][j-1] && (!x[i][j+1]));
                      }
              }

              for(i=2;i<w+2;i++)
              {
                      for(j=2;j<h+2;j++)
                      {
                              c[i][j] = (x[i][j] || x[i][j+1])&& (!(x[i][j] && x[i][j+1]));
                      }
              }

              for(i=2;i<w+2;i++)
              {
                      for(j=2;j<h+2;j++)
                      {
                              y[i][j] = c[i][j] && c[i-1][j] && ((x[i][j] || x[i-1][j])&& !(x[i][j] && x[i-1][j]));
                      }
              }

              for(i=2;i<w+2;i++)
              {
                      for(j=2;j<h+2;j++)
                      {
                              c[i][j] = y[i][j] || y[i][j-1];
                      }
              }

              for(i=2;i<=w+1;i++)
              {
                      for(j=2;j<=h+1;j++)
                      {
                              y[i][j] = c[i][j] || c[i+1][j];
                      }
              }

              for(i=2;i<=w+1;i++)
              {
                      for(j=2;j<=h+1;j++)
                      {
                              x[i][j] = x[i][j] && !(b[i][j] && !(y[i][j]));
                      }
              }

              for(i=2;i<=w+1;i++)
              {
                        for(j=2;j<=h+1;j++)
                        {
                                if(pre[i][j]!=x[i][j])
                                {
                                        check=0;
                                }
                        }
              }
              if(check==0)
              {
                check=1;
                for(i=2;i<=w+1;i++)
              {
                        for(j=2;j<=h+1;j++)
                        {
                                pre[i][j]=x[i][j];
                        }
              }
              }
              else
                check=0;

              round++;


        }
        

     //   Label13->Caption = IntToStr(round);



        FreePicArray((void **)a,w+4);
        FreePicArray((void **)b,w+4);
        FreePicArray((void **)c,w+4);
        FreePicArray((void **)y,w+4);
        FreePicArray((void **)pre,w+4);

        picar *r;
        r=NewPicar(w,h,1);
        for(i=2;i<w+2;i++)
        {
            for(j=2;j<h+2;j++)
            {
                r->a[i-2][j-2]=x[i][j];
            }
        }
        return r;
}

picar *thining(picar *z)
{
        int i,j;
        Byte **x,**wnq;
        int check;
        int h=z->h,w=z->w;
        Byte **a=z->a;
        x = NewArray(h+4,w+4);
        //y = NewArray(h+4,w+4);

        for(i=0;i<w+4;i++)
        {
            for(j=0;j<h+4;j++)
            {
                if(i<2 || j<2 || i>=w+2 || j>=h+2)
                {
                    x[j][i]=0;
                    //y[j][i]=0;
                }
                else
                {
                    x[j][i]=a[j-2][i-2];
                    //y[j][i]=a[j-2][i-2];
                }
            }
        }
        /*
        for(i=2;i<w+2;i++)
        {
                for(j=2;j<h+2;j++)
                {
                        if(a[j-2][i-2]==1)
                        {
                            x[j][i]=1;
                            y[j][i]=1;
                        }
                        else
                        {
                            x[j][i]=0;
                            y[j][i]=0;
                        }
                }
        }
        */
        int arr[256]={0,0,0,0,0,0,1,0,0,0, 0,0,1,0,1,0,0,0,0,0,
                      1,0,1,0,1,0,0,0,1,0, 1,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,1,0, 0,0,1,0,1,0,1,0,0,0,
                      1,0,1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
                      1,3,0,4,1,0,1,0,1,3, 0,4,1,0,1,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0, 0,0,1,3,0,4,1,0,1,0,
                      1,3,0,4,1,0,1,0,0,0, 0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,1,1, 0,2,1,0,1,0,1,1,0,2,
                      1,0,1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
                      1,1,0,2,1,0,1,0,1,1, 0,2,1,0,1,0};
        int notend,k,row,col,stable,win[2];

        notend=1;
        while(notend)
        {
                notend=0;

                for(k = 1 ;k<=2;k++)
                {
                      wnq = pta_wn(x,w,h,k);
                      for(row =2;row<w+2;row++)
                      {
                        for(col=2;col<h+2;col++)
                        {
                              stable=0;
                              if(x[col][row]==1)
                              {
                                  win[0]=0;
                                  win[1]=0;

                                  if(k==1)
                                  {
                                    win[0]=wnq[col][row-1];
                                    win[1]=wnq[col-1][row];
                                  }
                                  if(k==2)
                                  {
                                    win[0]=wnq[col][row+1];
                                    win[1]=wnq[col+1][row];
                                  }

                                  if(arr[wnq[col][row]]==1)
                                  {
                                    stable = 1;
                                  }
                                  else if(arr[wnq[col][row]]==2)
                                  {
                                    if(arr[win[0]]==0)
                                      stable =1;
                                  }
                                  else if(arr[wnq[col][row]]==3)
                                  {
                                    if(arr[win[1]]==0)
                                      stable = 1;
                                  }
                                  else if(arr[wnq[col][row]]==4)
                                  {
                                    if((arr[win[0]]==0) && (arr[win[1]]==0))
                                      stable = 1;
                                  }
                              }
                              if(stable)
                              {
                                x[col][row]=0;
                                notend=1;
                              }
                        }
                      }
                      FreePicArray((void **)wnq,h+4);
                }
                /*
                check = 0;
                for(i=2;i<w+2;i++)
                {
                        for(j=2;j<h+2;j++)
                        {
                           if(x[j][i]!=y[j][i])
                           {
                                check=1;
                                y[j][i]=x[j][i];
                           }
                        }
                }
                if(check==0)
                        break;
                */
        }


        //ตัดให้เหลือขนาดปกติ
        picar *r;
        r=NewPicar(h,w,1);
        for(i=2;i<h+2;i++)
        {
            for(j=2;j<w+2;j++)
            {
                r->a[i-2][j-2]=x[i][j];
            }
        }
        FreePicArray((void **)x,h+4);
        //FreePicArray((void **)y,h+4);
        return r;
}
int around(picar *z,int i,int j,int p)
{
    //012
    //7-3
    //654
    if(p==-1)
    {
        if(j-1>=0)
            return z->a[i][j-1];
        else
            return 0;
    }
    else
    {
        p=p%8;
    }
    if(p==0 && i-1>=0 && j-1>=0)
    {
        return z->a[i-1][j-1];
    }
    else if(p==1 && i-1>=0)
    {
        return z->a[i-1][j];
    }
    else if(p==2 && i-1>=0 && j+1<z->w)
    {
        return z->a[i-1][j+1];
    }
    else if(p==3 && j+1<z->w)
    {
        return z->a[i][j+1];
    }
    else if(p==4 && i+1<z->h && j+1<z->w)
    {
        return z->a[i+1][j+1];
    }
    else if(p==5 && i+1<z->h)
    {
        return z->a[i+1][j];
    }
    else if(p==6 && i+1<z->h && j-1>=0)
    {
        return z->a[i+1][j-1];
    }
    else if(p==7 && j-1>=0)
    {
        return z->a[i][j-1];
    }
    else
    {
        return 0;
    }
}
int neighbour(picar *z,int i,int j)     //0<x<20
{
    int k,l,count,ne=0;
    count=0;
    for(k=0;k<8;k++)
    {
        if(around(z,i,j,k-1)==0 && around(z,i,j,k)!=0)
        {
            break;
        }
    }
    if(k==9)
        return 0;      //ถ้ามีจะเป็น noise หรือ error
    for(l=k;l<k+8;l++)
    {
        if(around(z,i,j,l-1)==0 && around(z,i,j,l)!=0) //เริ่มขึ้นอันใหม่
        {
            count=1;
        }
        else if(around(z,i,j,l-1)!=0 && around(z,i,j,l)!=0) //กำลังนับอยู่
        {
            count++;
        }
        else if(around(z,i,j,l-1)!=0 && around(z,i,j,l)==0) //จบอันปัจจุบัน
        {
            if(count<=2)
            {
                ne++;
                count=0;
            }
            else if(count==3 && l%2==1)   //เป็นสีดำสามจุดที่เรียงต่อกันแบบไม่หักมุม
            {
                ne++;
                count=0;
            }
        }
    }
    return ne;
}
int PosToi(int ans)
{
    ans=ans%8;
    if     (ans==0 || ans==1 || ans==2)
    {
        return -1;
    }
    else if(ans==7 ||           ans==3)
    {
        return 0;
    }
    else if(ans==6 || ans==5 || ans==4)
    {
        return 1;
    }
}
int PosToj(int ans)
{
    ans=ans%8;
    if     (ans==0 || ans==7 || ans==6)
    {
        return -1;
    }
    else if(ans==1 ||           ans==5)
    {
        return 0;
    }
    else if(ans==2 || ans==3 || ans==4)
    {
        return 1;
    }
}
void FindAllPath(picar *z,int i,int j,int *si,int *sj)
{
    int k,l,count,ne=0,ans;
    count=0;
    for(k=0;k<8;k++)
    {
        if(around(z,i,j,k-1)==0 && around(z,i,j,k)!=0)
        {
            break;
        }
    }

    for(l=k;l<k+8;l++)
    {
        if(around(z,i,j,l-1)==0 && around(z,i,j,l)!=0) //เริ่มขึ้นอันใหม่
        {
            count=1;
        }
        else if(around(z,i,j,l-1)!=0 && around(z,i,j,l)!=0) //กำลังนับอยู่
        {
            count++;
        }
        else if(around(z,i,j,l-1)!=0 && around(z,i,j,l)==0) //จบอันปัจจุบัน
        {
            if(count<=2)
            {
                if(l%2==1)
                {
                    ans=l-count;
                }
                else
                {
                    ans=l-1;
                }

                si[ne]=i+PosToi(ans);
                sj[ne]=j+PosToj(ans);
                ne++;
                count=0;
            }
            else if(count==3 && l%2==1)   //เป็นสีดำสามจุดที่เรียงต่อกันแบบไม่หักมุม
            {

                si[ne]=i+PosToi(l-2);
                sj[ne]=j+PosToj(l-2);
                ne++;
                count=0;
            }
        }
    }
}
void NextStep(picar *z,int i,int j,int fi,int fj,int *ni,int *nj)
{
    int k,l;
    *ni=-1;
    for(k=i-1;k<=i+1;k++)
    {
        for(l=j-1;l<=j+1;l++)
        {
            if(k>=0 && k<z->h && l>=0 && l<z->w)  //ไม่ออกนอกขอบเขต
            {
                if(z->a[k][l]>0 && !(k==fi && l==fj) && !(k==i && l==j)) //ไม่เป็นจุดปัจจุบัน และจุดเก่าที่ผ่านมา
                {
                    if(!((k!=fi && l==fj && fi==i) || (l!=fj && k==fi && fj==j)))  //ขาเข้า
                    {
                        if(k==i || l==j)    //ขาออก อยู่ในแฉกขึ้น ลง ซ้าย ขวา
                        {
                            *ni=k;
                            *nj=l;
                            break;
                        }
                        else if(z->a[i][l]==0 && z->a[k][j]==0)  //ขาออกอยู่ในแฉก กากบาท
                        {
                            *ni=k;
                            *nj=l;
                            break;
                        }
                    }
                }
            }
        }
        if(*ni!=-1)
            break;
    }
}
/*
void PreChain1(thindata *t)      //จะรวบ loop ให้เป็นจุดจุดเดียว แล้วจำทางแยกไว้
//input thining ที่มีกลุ่มเดียว เข้ามา
//จะไม่มี "ะ ฐ ญ" ที่ไม่ติดกันสองชิ้นเข้ามาพร้อมกัน
{
    picar *z=t->z;
    int i,j,k,l,m,n;
    int numloop,*posiloop,*posjloop,**juncloopi,**juncloopj,*numjuncloop;
    //pos_loop มีไว้จำตำแหน่งจุดกึ่งกลาง loop
    //juncloop มีไว้จำตำแหน่งเชื่อมออกทุกจุดของแต่ละ loop
    //numjuncloop เก็บจำนวนจุดเชื่อมออก ของแต่ละ loop
    bool goout;
    //ทาสีส่วนที่เปิด ด้วย 200 (วนไปรอบ ๆ เพื่อทา)
    int **nodepos; //เอาไว้จำว่าตำแหน่งไหนเป็นของ node ที่เท่าไหร่ เพื่อใช้สร้าง map
    int node=0;
    nodepos=(int **)malloc(sizeof(int *)*z->h);
    for(i=0;i<z->h;i++)
    {
        nodepos[i]=(int *)malloc(sizeof(int)*z->w);
        for(j=0;j<z->w;j++)
        {
            nodepos[i][j]=-1;
        }
    }


    i=0;
    for(j=0;j<z->w;j++)
    {
        if(z->a[i][j]==0)
        {
            change(z,i,j,0,200,4);
        }
    }

    i=z->h-1;
    for(j=0;j<z->w;j++)
    {
        if(z->a[i][j]==0)
        {
            change(z,i,j,0,200,4);
        }
    }

    j=0;
    for(i=1;i<z->h-1;i++)
    {
        if(z->a[i][j]==0)
        {
            change(z,i,j,0,200,4);
        }
    }

    j=z->w-1;
    for(i=1;i<z->h-1;i++)
    {
        if(z->a[i][j]==0)
        {
            change(z,i,j,0,200,4);
        }
    }



    //หา 1 ที่ติดกับ 0 (ที่จะหลงเหลืออยู่ถ้ามี loop) แล้วเปลี่ยนเป็น 20
    //ส่วนเส้นที่ไม่ใช่ loop เปลี่ยนเป็น 255
    for(i=0;i<z->h;i++)
    {
        for(j=0;j<z->w;j++)
        {
            if(z->a[i][j]==1)
            {
                for(k=i-1;k<=i+1;k++)
                {
                    for(l=j-1;l<=j+1;l++)
                    {
                        if(k>=0&&k<z->h&&l>=0&&l<z->w)
                        {
                            if(z->a[k][l]==0)
                            {
                                z->a[i][j]=20;
                                break;
                            }
                        }
                    }
                    if(z->a[i][j]==20)
                        break;
                }
                if(z->a[i][j]==1)
                {
                    z->a[i][j]=255;
                }
            }
        }
    }

    //นับจำนวน loop ทั้งหมด พร้อมทั้งระบุตำแหน่งจุดศูนย์กลาง
    bound *b;
    b=(bound *)malloc(sizeof(bound));
    numloop=0;
    posiloop=(int *)malloc(sizeof(int));
    posjloop=(int *)malloc(sizeof(int));

    numjuncloop=(int *)malloc(sizeof(int));
    juncloopi=(int **)malloc(sizeof(int *));
    juncloopj=(int **)malloc(sizeof(int *));

    for(i=0;i<z->h;i++)
    {
        for(j=0;j<z->w;j++)
        {
            if(z->a[i][j]==20)
            {
                numloop++;
                posiloop=(int *)realloc(posiloop,sizeof(int)*numloop);
                posjloop=(int *)realloc(posjloop,sizeof(int)*numloop);

                numjuncloop=(int *)realloc(numjuncloop,sizeof(int)*numloop);
                numjuncloop[numloop-1]=0;

                juncloopi=(int **)realloc(juncloopi,sizeof(int *)*numloop);
                juncloopj=(int **)realloc(juncloopj,sizeof(int *)*numloop);

                juncloopi[numloop-1]=(int *)malloc(sizeof(int));
                juncloopj[numloop-1]=(int *)malloc(sizeof(int));

                b->up=i;
                b->down=i;
                b->left=j;
                b->right=j;
                FindBound(z,i,j,b,20,21);
                posiloop[numloop-1]=(b->up+b->down)/2;
                posjloop[numloop-1]=(b->right+b->left)/2;
                for(k=b->up;k<=b->down;k++)
                {
                    for(l=b->left;l<=b->right;l++)
                    {
                        if(z->a[k][l]==21)
                        {
                            for(m=k-1;m<=k+1;m++)
                            {
                                for(n=l-1;n<=l+1;n++)
                                {
                                    //goout=false;
                                    if(m>=0&&m<z->h&&n>=0&&n<z->w)
                                    {
                                        if(z->a[m][n]==255)
                                        {
                                            if((m==k || n==l)||(m!=k && n!=l && z->a[k][n]!=21 && z->a[m][l]!=21))
                                            {
                                                numjuncloop[numloop-1]++;
                                                juncloopi[numloop-1]=(int *)realloc(juncloopi[numloop-1],sizeof(int)*numjuncloop[numloop-1]);
                                                juncloopj[numloop-1]=(int *)realloc(juncloopj[numloop-1],sizeof(int)*numjuncloop[numloop-1]);

                                                juncloopi[numloop-1][numjuncloop[numloop-1]-1]=m;    //แก้ตรงนี้
                                                juncloopj[numloop-1][numjuncloop[numloop-1]-1]=n;    //แก้ตรงนี้
                                                //goout=true;
                                                //break;
                                            }
                                        }
                                    }
                                }
                                //if(goout)
                                    //break;
                            }
                        }
                    }
                }
                for(k=b->up;k<=b->down;k++)
                {
                    for(l=b->left;l<=b->right;l++)
                    {
                        if(z->a[k][l]==21)
                        {
                           z->a[k][l]=10+numjuncloop[numloop-1];
                           nodepos[k][l]=node;
                        }
                    }
                }
                node++;
            }
        }
    }

    //return (assign value)
    //z->type=2;
    //ShowImage(z,Form1->Image2,Rect2);
    t->loop_num=numloop;
    t->loop_juncnum=numjuncloop;
    t->loop_pi=posiloop;
    t->loop_pj=posjloop;
    t->loop_ji=juncloopi;
    t->loop_jj=juncloopj;

*/
    /*
    //free memory
    free(posiloop);
    free(posjloop);
    free(numjuncloop);
    for(i=0;i<numloop;i++)
    {
        free(juncloopi[i]);
        free(juncloopj[i]);
    }
    free(juncloopi);
    free(juncloopj);
    */
/*    ///จบเรื่อง loop ทั้งหมด

    //เปลี่ยนที่ว่างจาก 200 ให้เป็น 0 เหมือนเดิม
    for(i=0;i<z->h;i++)
    {
        for(j=0;j<z->w;j++)
        {
            if(z->a[i][j]==200)
            {
                z->a[i][j]=0;
            }
        }
    }

    //เริ่มการหา junction และจุดปลาย โดยจะไม่ยุ่งกับ loop เลย
    t->end_num=0;
    t->junc_num=0;
    for(i=0;i<z->h;i++)
    {
        for(j=0;j<z->w;j++)
        {
            if(z->a[i][j]==255)
            {
                z->a[i][j]=neighbour(z,i,j);
                if(z->a[i][j]==1)
                {
                    t->end_num++;
                    nodepos[i][j]=node;
                    node++;
                }
                else if(z->a[i][j]>2 && z->a[i][j]<10)
                {
                    t->junc_num++;
                    nodepos[i][j]=node;
                    node++;
                }
            }
        }
    }
    //z->type=2;
    //SavePicar(z,"C://step7.bmp");
    //เริ่มการ สร้าง map

    link *temp,*pt;
    int lasti,lastj;
    int o,p;
    int *si,*sj;
    bool **pass,*already;

    pass=(bool **)malloc(sizeof(bool *)*z->h);
    for(i=0;i<z->h;i++)
    {
        pass[i]=(bool *)malloc(sizeof(bool)*z->w);
        for(j=0;j<z->w;j++)
        {
            pass[i][j]=false;
        }
    }

    node=t->loop_num+t->junc_num+t->end_num;

    t->map=(link ***)malloc(sizeof(link **)*node);
    t->have=(bool **)malloc(sizeof(bool *)*node);
    for(i=0;i<node;i++)
    {
        t->map[i]=(link **)malloc(sizeof(link *)*node);
        t->have[i]=(bool *)malloc(sizeof(bool)*node);
        for(j=0;j<node;j++)
        {
            t->have[i][j]=false;
        }
    }
    t->node_pi=(int *)malloc(sizeof(int)*node);
    t->node_pj=(int *)malloc(sizeof(int)*node);

    already=(bool *)malloc(sizeof(bool)*t->loop_num);
    for(i=0;i<t->loop_num;i++)
    {
        already[i]=false;
    }
    for(i=0;i<z->h;i++)
    {
        for(j=0;j<z->w;j++)
        {
            if(nodepos[i][j]>=0)
            {
                if(nodepos[i][j]<t->loop_num) //เจอ loop
                {
                    if(already[nodepos[i][j]]==false)
                    {
                        already[nodepos[i][j]]=true;
                        //เก็บจุดกลาง
                        t->node_pi[nodepos[i][j]]=t->loop_pi[nodepos[i][j]];
                        t->node_pj[nodepos[i][j]]=t->loop_pj[nodepos[i][j]];

                        for(k=0;k<t->loop_juncnum[nodepos[i][j]];k++)
                        {
                            //สร้าง link 1 อัน
                            m=t->loop_ji[nodepos[i][j]][k];
                            n=t->loop_jj[nodepos[i][j]][k];


                            if(pass[m][n]==false)//ตรวจดูว่าจุดนี้เคยถูกมาถึงแล้วหรือไม่
                            {
                                pass[m][n]=true;
                                temp=(link *)malloc(sizeof(link));
                                temp->d=0;
                                temp->note=(int **)malloc(sizeof(int *));
                                //เดินไปจนเจอ !=2

                                //หา pixel ของ loop เพื่อใช้เป็น last
                                lasti=-1;
                                lastj=-1;
                                for(o=m-1;o<=m+1;o++)
                                {
                                    for(p=n-1;p<=n+1;p++)
                                    {
                                        if(o>=0 && o<z->h && p>=0 && p<z->w && /*z->a[o][p]>10 &&*/ /* nodepos[o][p]==nodepos[i][j]) */
/*                                        {
                                            if(lasti!=m && lastj!=n)
                                            {
                                                lasti=o;
                                                lastj=p;
                                            }
                                        }
                                    }
                                }
                                while(1)
                                {
                                    if(z->a[m][n]==2)
                                    {
                                        temp->d++;
                                        temp->note=(int **)realloc(temp->note,sizeof(int)*temp->d);
                                        temp->note[temp->d-1]=(int *)malloc(sizeof(int)*2);
                                        temp->note[temp->d-1][0]=m;
                                        temp->note[temp->d-1][1]=n;
                                        NextStep(z,m,n,lasti,lastj,&m,&n);
                                        lasti=temp->note[temp->d-1][0];
                                        lastj=temp->note[temp->d-1][1];
                                    }
                                    else
                                        break;
                                }
                                //จบสายของสองแล้ว ไปเจอตัวอื่น
                                t->have[nodepos[i][j]][nodepos[m][n]]=true;
                                t->have[nodepos[m][n]][nodepos[i][j]]=true;
                                pass[lasti][lastj]=true;
                                //สลับ pointer ส่งเข้า map
                                //pt=t->map[nodepos[i][j]][nodepos[m][n]];
                                t->map[nodepos[i][j]][nodepos[m][n]]=temp;

                                t->map[nodepos[m][n]][nodepos[i][j]]=(link *)malloc(sizeof(link));
                                temp=t->map[nodepos[m][n]][nodepos[i][j]]; //สลับกัน เป็นขากลับ

                                temp->d=t->map[nodepos[i][j]][nodepos[m][n]]->d;
                                temp->note=(int **)malloc(sizeof(int *)*temp->d);
                                o=0;
                                for(p=temp->d-1;p>=0;p--,o++)
                                {
                                    temp->note[o]=(int *)malloc(sizeof(int)*2);
                                    temp->note[o][0]=t->map[nodepos[i][j]][nodepos[m][n]]->note[p][0];
                                    temp->note[o][1]=t->map[nodepos[i][j]][nodepos[m][n]]->note[p][1];
                                }
                                //temp=pt;
                            }
                        }
                    }

                }
                else //เจอจุดปลายหรือทางแยก
                {
                    //เก็บจุดกลาง
                    t->node_pi[nodepos[i][j]]=i;
                    t->node_pj[nodepos[i][j]]=j;

                    si=(int *)malloc(sizeof(int)*z->a[i][j]);
                    sj=(int *)malloc(sizeof(int)*z->a[i][j]);
                    FindAllPath(z,i,j,si,sj);

                    for(k=0;k<z->a[i][j];k++)
                    {

                        m=si[k];
                        n=sj[k];

                        if(pass[m][n]==false)//ตรวจดูว่าจุดนี้เคยถูกมาถึงแล้วหรือไม่
                        {
                            pass[m][n]=true;
                            temp=(link *)malloc(sizeof(link));
                            temp->d=0;
                            temp->note=(int **)malloc(sizeof(int *));
                            //เดินไปจนเจอ !=2

                            //หา pixel ของ loop เพื่อใช้เป็น last
                            lasti=i;
                            lastj=j;

                            while(1)
                            {
                                if(z->a[m][n]==2)
                                {
                                    temp->d++;
                                    temp->note=(int **)realloc(temp->note,sizeof(int)*temp->d);
                                    temp->note[temp->d-1]=(int *)malloc(sizeof(int)*2);
                                    temp->note[temp->d-1][0]=m;
                                    temp->note[temp->d-1][1]=n;
                                    NextStep(z,m,n,lasti,lastj,&m,&n);
                                    lasti=temp->note[temp->d-1][0];
                                    lastj=temp->note[temp->d-1][1];
                                }
                                else
                                    break;
                            }
                            //จบสายของสองแล้ว ไปเจอตัวอื่น
                            t->have[nodepos[i][j]][nodepos[m][n]]=true;
                            t->have[nodepos[m][n]][nodepos[i][j]]=true;
                            pass[lasti][lastj]=true;
                            //สลับ pointer ส่งเข้า map
                            //pt=t->map[nodepos[i][j]][nodepos[m][n]];
                            t->map[nodepos[i][j]][nodepos[m][n]]=temp;

                            t->map[nodepos[m][n]][nodepos[i][j]]=(link *)malloc(sizeof(link));
                            temp=t->map[nodepos[m][n]][nodepos[i][j]]; //สลับกัน เป็นขากลับ

                            temp->d=t->map[nodepos[i][j]][nodepos[m][n]]->d;
                            temp->note=(int **)malloc(sizeof(int *)*temp->d);
                            o=0;
                            for(p=temp->d-1;p>=0;p--,o++)
                            {
                                temp->note[o]=(int *)malloc(sizeof(int)*2);
                                temp->note[o][0]=t->map[nodepos[i][j]][nodepos[m][n]]->note[p][0];
                                temp->note[o][1]=t->map[nodepos[i][j]][nodepos[m][n]]->note[p][1];
                            }
                            //temp=pt;
                        }
                    }
                    free(si);
                    free(sj);
                }
            }
        }
    }
    //สร้าง map เสร็จแล้ว
    t->node_num=node;

    //free memory pass อย่างเดียว
    for(i=0;i<z->h;i++)
    {
        free(pass[i]);
    }
    free(pass);
    free(already);
}
*/
int GetPos(int i,int j,int m,int n)
{
    if(m==i-1 && n==j-1)
        return 0;
    else if(m==i-1 && n==j)
        return 1;
    else if(m==i-1 && n==j+1)
        return 2;
    else if(m==i && n==j+1)
        return 3;
    else if(m==i+1 && n==j+1)
        return 4;
    else if(m==i+1 && n==j)
        return 5;
    else if(m==i+1 && n==j-1)
        return 6;
    else if(m==i && n==j-1)
        return 7;
    else
        return -1;
}
void PreChain2(thindata *t)      //แค่สรุปทางแยกเอาไว้ ไม่รวบ loop
//input thining ที่มีกลุ่มเดียว เข้ามา
//จะไม่มี "ะ ฐ ญ" ที่ไม่ติดกันสองชิ้นเข้ามาพร้อมกัน
{
    picar *z=t->z;
    int i,j,k,l,m,n,x;
    int **nodepos; //เอาไว้จำว่าตำแหน่งไหนเป็นของ node ที่เท่าไหร่ เพื่อใช้สร้าง map
    int node=0;

    nodepos=(int **)malloc(sizeof(int *)*z->h);
    for(i=0;i<z->h;i++)
    {
        nodepos[i]=(int *)malloc(sizeof(int)*z->w);
        for(j=0;j<z->w;j++)
        {
            nodepos[i][j]=-1;
        }
    }
    //หา junction และจุดปลายใส่ nodepos
    t->end_num=0;
    t->junc_num=0;
    for(j=0;j<z->w;j++) //---------|
    {                            //|->สลับกันเพื่อให้จุดเริ่มต้นอยู่ด้านซ้ายเสมอโดยไม่ต้องสลับ
        for(i=0;i<z->h;i++)//------|
        {
            if(z->a[i][j]==1)
            {
                z->a[i][j]=neighbour(z,i,j);
                if(z->a[i][j]==1)
                {
                    t->end_num++;
                    nodepos[i][j]=node;
                    node++;
                }
                else if(z->a[i][j]>2)
                {
                    t->junc_num++;
                    nodepos[i][j]=node;
                    node++;
                }
            }
        }
    }
    //z->type=2;
    //SavePicar(z,"C://step7.bmp");
    //เริ่มการ สร้าง map

    link *temp,*pt;
    int lasti,lastj;
    int o,p;
    int *si,*sj;
    bool ***pass,*already;
    int countorder;
    pass=(bool ***)malloc(sizeof(bool **)*z->h);
    for(i=0;i<z->h;i++)
    {
        pass[i]=(bool **)malloc(sizeof(bool *)*z->w);
        for(j=0;j<z->w;j++)
        {
            pass[i][j]=(bool *)malloc(sizeof(bool)*8);
            for(k=0;k<8;k++)
            {
                pass[i][j][k]=false;
            }
        }
    }

    node=t->junc_num+t->end_num;

    t->linknode=(int *)malloc(sizeof(int)*node);

    t->map=(link ****)malloc(sizeof(link ***)*node);
    t->have=(int **)malloc(sizeof(int *)*node);
    t->use=(int ***)malloc(sizeof(int **)*node);

    for(i=0;i<node;i++)
    {
        t->map[i]=(link ***)malloc(sizeof(link **)*node);
        t->have[i]=(int *)malloc(sizeof(int)*node);
        t->use[i]=(int **)malloc(sizeof(int *)*node);
        for(j=0;j<node;j++)
        {
            t->have[i][j]=0;
        }
    }

    t->node_pi=(int *)malloc(sizeof(int)*node);
    t->node_pj=(int *)malloc(sizeof(int)*node);

    t->order=(int ***)malloc(sizeof(int **)*node);
    if(node<6)
    {
        for(i=0;i<node;i++)
        {
            t->order[i]=(int **)malloc(sizeof(int *)*6);
            for(j=0;j<6;j++)
            {
                t->order[i][j]=(int *)malloc(sizeof(int)*2);
            }
        }
    }
    else
    {
        for(i=0;i<node;i++)
        {
            t->order[i]=(int **)malloc(sizeof(int *)*node);
            for(j=0;j<node;j++)
            {
                t->order[i][j]=(int *)malloc(sizeof(int)*2);
            }
        }
    }
    for(j=0;j<z->w;j++)         //สลับกันเหมือนเดิมแต่ไม่รู้ว่ามีผลหรือไม่
    {
        for(i=0;i<z->h;i++)
        {
            if(nodepos[i][j]>=0)
            {
                    //เก็บจุดกลาง
                    t->node_pi[nodepos[i][j]]=i;
                    t->node_pj[nodepos[i][j]]=j;

                    si=(int *)malloc(sizeof(int)*z->a[i][j]);
                    sj=(int *)malloc(sizeof(int)*z->a[i][j]);
                    FindAllPath(z,i,j,si,sj);
                    countorder=0;
                    for(k=0;k<z->a[i][j];k++)
                    {

                        m=si[k];
                        n=sj[k];

                        if(pass[i][j][GetPos(i,j,m,n)]==false )//ตรวจดูว่าจุดนี้เคยถูกมาถึงแล้วหรือไม่
                        {
                            pass[i][j][GetPos(i,j,m,n)]=true;
                            temp=(link *)malloc(sizeof(link));
                            temp->d=0;
                            temp->note=(int **)malloc(sizeof(int *));
                            //เดินไปจนเจอ !=2
                            lasti=i;
                            lastj=j;

                            while(1)
                            {
                                if(z->a[m][n]==2)
                                {
                                    temp->d++;
                                    temp->note=(int **)realloc(temp->note,sizeof(int)*temp->d);
                                    temp->note[temp->d-1]=(int *)malloc(sizeof(int)*2);
                                    temp->note[temp->d-1][0]=m;
                                    temp->note[temp->d-1][1]=n;
                                    NextStep(z,m,n,lasti,lastj,&m,&n);
                                    lasti=temp->note[temp->d-1][0];
                                    lastj=temp->note[temp->d-1][1];
                                }
                                else
                                    break;
                            }
                            //จบสายของสองแล้ว ไปเจอตัวอื่น
                            t->have[nodepos[i][j]][nodepos[m][n]]++;
                            if(nodepos[i][j]!=nodepos[m][n])
                            {
                                t->have[nodepos[m][n]][nodepos[i][j]]++;
                            }
                            pass[m][n][GetPos(m,n,lasti,lastj)]=true;
                            //สลับ pointer ส่งเข้า map ในกรณีที่ไม่กลับมาเจอตัวเอง
                            //ขอ memory ให้ map
                            if(t->have[nodepos[i][j]][nodepos[m][n]]==1) //เพิ่งเกิด ยังไม่ได้ malloc
                            {
                                t->map[nodepos[i][j]] [nodepos[m][n]]=(link **)malloc(sizeof(link*));
                                t->use[nodepos[i][j]] [nodepos[m][n]]=(int *)malloc(sizeof(int));
                                t->use[nodepos[i][j]] [nodepos[m][n]] [0]=0;
                            }
                            else if(t->have[nodepos[i][j]][nodepos[m][n]]>1)
                            {
                                t->map[nodepos[i][j]] [nodepos[m][n]]=(link **)realloc(t->map[nodepos[i][j]][nodepos[m][n]],sizeof(link *)*t->have[nodepos[i][j]][nodepos[m][n]]);
                                t->use[nodepos[i][j]] [nodepos[m][n]]=(int *)realloc(t->use[nodepos[i][j]] [nodepos[m][n]],sizeof(int)*t->have[nodepos[i][j]][nodepos[m][n]]);
                                t->use[nodepos[i][j]] [nodepos[m][n]] [t->have[nodepos[i][j]][nodepos[m][n]]-1] =0;
                            }
                            //ใส่ของที่ได้มา
                            t->map [nodepos[i][j]] [nodepos[m][n]] [t->have[nodepos[i][j]][nodepos[m][n]]-1]=temp;
                            //ทำย้อนกลับ
                            if(nodepos[i][j]!=nodepos[m][n])
                            {
                                //ขอ memory ให้ map
                                if(t->have[nodepos[m][n]][nodepos[i][j]]==1) //เพิ่งเกิด ยังไม่ได้ malloc
                                {
                                    t->map[nodepos[m][n]] [nodepos[i][j]]=(link **)malloc(sizeof(link*));
                                    t->use[nodepos[m][n]] [nodepos[i][j]]=(int *)malloc(sizeof(int));
                                    t->use[nodepos[m][n]] [nodepos[i][j]] [0]=0;
                                }
                                else if(t->have[nodepos[i][j]][nodepos[m][n]]>1)
                                {
                                    t->map[nodepos[m][n]] [nodepos[i][j]]=(link **)realloc(t->map[nodepos[m][n]] [nodepos[i][j]],sizeof(link *)*t->have[nodepos[m][n]] [nodepos[i][j]]);
                                    t->use[nodepos[m][n]] [nodepos[i][j]]=(int *)realloc(t->use[nodepos[m][n]] [nodepos[i][j]],sizeof(int)*t->have[nodepos[m][n]] [nodepos[i][j]]);
                                    t->use[nodepos[m][n]] [nodepos[i][j]] [t->have[nodepos[m][n]] [nodepos[i][j]]-1] =0;
                                }
                                t->map[nodepos[m][n]] [nodepos[i][j]] [t->have[nodepos[m][n]][nodepos[i][j]]-1]=(link *)malloc(sizeof(link));
                                temp=t->map[nodepos[m][n]] [nodepos[i][j]] [t->have[nodepos[m][n]][nodepos[i][j]]-1]; //สลับกัน เป็นขากลับ

                                temp->d=t->map[nodepos[i][j]] [nodepos[m][n]] [t->have[nodepos[i][j]][nodepos[m][n]]-1] ->d;
                                temp->note=(int **)malloc(sizeof(int *)*temp->d);
                                o=0;
                                for(p=temp->d-1;p>=0;p--,o++)
                                {
                                    temp->note[o]=(int *)malloc(sizeof(int)*2);
                                    temp->note[o][0]=t->map[nodepos[i][j]][nodepos[m][n]] [t->have[nodepos[i][j]][nodepos[m][n]]-1]->note[p][0];
                                    temp->note[o][1]=t->map[nodepos[i][j]][nodepos[m][n]] [t->have[nodepos[i][j]][nodepos[m][n]]-1]->note[p][1];
                                }
                            }
                            t->order[nodepos[i][j]][countorder][0]=nodepos[m][n];
                            t->order[nodepos[i][j]][countorder][1]=t->have[nodepos[i][j]][nodepos[m][n]]-1;
                            countorder++;

                        }
                        else
                        {
                            for(l=0;l<node;l++)
                            {
                                for(x=0;x<t->have[l][nodepos[i][j]];x++)
                                {
                                    if(t->map[l][nodepos[i][j]] [x]->d>0)
                                    {   //       |---------------------|      |-----------------------------------|
                                        //                                           |=====================|
                                        if(t->map[l] [nodepos[i][j]] [x]->note[t->map[l] [nodepos[i][j]] [x]->d-1][0]==m
                                        && t->map[l] [nodepos[i][j]] [x]->note[t->map[l] [nodepos[i][j]] [x]->d-1][1]==n
                                        )
                                        {
                                            t->order[nodepos[i][j]][countorder][0]=l;
                                            t->order[nodepos[i][j]][countorder][1]=x;
                                            countorder++;
                                            break;
                                        }
                                    }
                                    else if(t->map[l][nodepos[i][j]] [x]->d==0)
                                    {
                                        if(nodepos[m][n]==l)

                                        {
                                            t->order[nodepos[i][j]][countorder][0]=l;
                                            t->order[nodepos[i][j]][countorder][1]=x;
                                            countorder++;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    free(si);
                    free(sj);

            }
        }
    }
    //สร้าง map เสร็จแล้ว
    t->node_num=node;
    for(i=0;i<node;i++)
    {
        t->linknode[i]=z->a[t->node_pi[i]][t->node_pj[i]];
    }
    //free memory pass อย่างเดียว
    for(i=0;i<z->h;i++)
    {
        for(j=0;j<z->w;j++)
        {
            free(pass[i][j]);
        }
        free(pass[i]);
    }
    free(pass);
}
int count(picar *z,int i,int j)
{
    if(i==-1 || j==-1 || i==z->h || j==z->w || z->a[i][j]!=1)
    {
        return 0;
    }
    else
    {
        z->a[i][j]=2;
        return 1+count(z,i,j+1)+count(z,i,j-1)+count(z,i+1,j)+count(z,i-1,j)+
        count(z,i+1,j+1)+count(z,i+1,j-1)+count(z,i-1,j+1)+count(z,i-1,j-1);
    }
}
void change(picar *z,int i,int j,int old,int color,int direction)
{
    if(direction==4)
    {
        if(!(i==-1 || j==-1 || i==z->h || j==z->w || z->a[i][j]!=old))
        {
            z->a[i][j]=color;
            change(z,i,j+1,old,color,4);
            change(z,i,j-1,old,color,4);
            change(z,i+1,j,old,color,4);
            change(z,i-1,j,old,color,4);
        }
    }
    else if(direction==8)
    {
        if(!(i==-1 || j==-1 || i==z->h || j==z->w || z->a[i][j]!=old))
        {
            z->a[i][j]=color;
            change(z,i,j+1,old,color,8);
            change(z,i,j-1,old,color,8);
            change(z,i+1,j,old,color,8);
            change(z,i-1,j,old,color,8);
            change(z,i+1,j+1,old,color,8);
            change(z,i+1,j-1,old,color,8);
            change(z,i-1,j+1,old,color,8);
            change(z,i-1,j-1,old,color,8);
        }
    }
}
/*
void change(onerow *z,int i,int j,int old,int color)
{
    if(!(i==-1 || j==-1 || i==z->h || j==z->w || z->a[i][j]!=old))
    {
        z->a[i][j]=color;
        change(z,i,j+1,old,color);
        change(z,i,j-1,old,color);
        change(z,i+1,j,old,color);
        change(z,i-1,j,old,color);
        change(z,i+1,j+1,old,color);
        change(z,i+1,j-1,old,color);
        change(z,i-1,j+1,old,color);
        change(z,i-1,j-1,old,color);
    }
}
*/
void clear(picar *z,int i,int j)
{
    if(!(i==-1 || j==-1 || i==z->h || j==z->w || z->a[i][j]==0))
    {
        z->a[i][j]=0;
        clear(z,i,j+1);
        clear(z,i,j-1);
        clear(z,i+1,j);
        clear(z,i-1,j);

        clear(z,i+1,j+1);
        clear(z,i+1,j-1);
        clear(z,i-1,j+1);
        clear(z,i-1,j-1);
    }
}
int CountGroup(picar *z)
{
    int i,j,n=0;
    for(i=0;i<z->h;i++)
    {
        for(j=0;j<z->w;j++)
        {
            if(z->a[i][j]==1)
            {
                change(z,i,j,1,2,8);
                n++;
            }
        }
    }
    for(i=0;i<z->h;i++)
    {
        for(j=0;j<z->w;j++)
        {
            if(z->a[i][j]==2)
            {
                change(z,i,j,2,1,8);
            }
        }
    }
    return n;
}
void ClearImage(TImage *z,int i,int j,int bup,int bdown,int bleft,int bright)
{
    if(!(i<bup || j<bleft || i>bdown || j>bright || z->Canvas->Pixels[j][i]==clWhite))
    {
        z->Canvas->Pixels[j][i]=clWhite;
        ClearImage(z,i,j+1,bup,bdown,bleft,bright);
        ClearImage(z,i,j-1,bup,bdown,bleft,bright);
        ClearImage(z,i+1,j,bup,bdown,bleft,bright);
        ClearImage(z,i-1,j,bup,bdown,bleft,bright);

        ClearImage(z,i+1,j+1,bup,bdown,bleft,bright);
        ClearImage(z,i+1,j-1,bup,bdown,bleft,bright);
        ClearImage(z,i-1,j+1,bup,bdown,bleft,bright);
        ClearImage(z,i-1,j-1,bup,bdown,bleft,bright);
    }
}
void DelNoiseHis(picar *z)
//Delete Noise Use Histogram
//of size of group of black pixel
{
    int max=0,sum=0,num=0,mean;
    unsigned int *his;
    int h=z->h,w=z->w;
    int i,j,c;
    Byte **a=z->a;
    his=(unsigned int *)malloc(sizeof(unsigned int));
    //make histogram
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
            if(a[i][j]==1)
            {
                c=count(z,i,j);
                if(c>max && c<2000)
                {
                    his=(unsigned int *)realloc(his,sizeof(unsigned int)*(c+1));
                    for(max=max+1;max<c+1;max++)
                    {
                        his[max]=0;
                    }
                    max--;
                }
                if(c>=2000)
                {
                    clear(z,i,j);
                }
                else
                {
                    his[c]++;
                }
            }
        }
    }
    
    //clear 2 to 1
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
            if(a[i][j]==2)
            {
                a[i][j]=1;
            }
        }
    }
    //process histrogram
    for(i=4;i<=max;i++)
    {
        num+=his[i];
        sum+=his[i]*i;
    }
    mean=sum/num;
    free(his);
    //delete noise

    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
            if(a[i][j]==1)
            {
                c=count(z,i,j);
                if(c<2 || c>mean*4)  /// ปรับได้ที่นี่ น่าจะตรงกับค่าประมาณ mean + 3SD
                {
                    clear(z,i,j);
                }
            }
        }
    }
    //clear 2 to 1
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
            if(a[i][j]==2)
            {
                a[i][j]=1;
            }
        }
    }
}

picar **DivideRow(picar *z,int *row)
{
    int h=z->h,w=z->w;
    int *his,*his2,num,sum,n,mean,i,j,begin,end; //0
    Byte **a=z->a;
    picar **r;
    r=(picar **)malloc(sizeof(picar *));
    //make histogram
    his=(int *)malloc(sizeof(int)*h);
    his2=(int *)malloc(sizeof(int)*h);   //1
    for(i=0;i<h;i++)
    {
        his[i]=0;
        his2[i]=0;            //2
        for(j=0;j<w;j++)
        {
            his[i]+=a[i][j];
        }
        his2[i]=his[i];     //3
        his[i]-=4;      //แก้ไขตรงนี้ได้
        if(his[i]<0)
            his[i]=0;
    }
    //process histogram
    num=0;
    sum=0;
    n=0;
    i=0;
    while(his[i]>0)
    {
        i++;
    }
    while(his[i]==0)
    {
        i++;
    }
    for(;i<h-1;i++)
    {
        if(his[i]>0&&his[i+1]==0)
        {
            num++;
            sum+=n;
            n=0;
        }
        if(his[i]==0)
        {
            n++;
        }
    }
    mean=sum/num;
    //divide row
    num=0;
    i=0;
    while(his[i]>0 && i<h)
    {
        i++;
    }
    while(his[i]==0 && i<h)
    {
        i++;
    }
    ///////////เก็บบรรทัด////////////
    n=0;
    num=0;
    while(i<h)
    {
        if(his[i-1]==0&&his[i]>0)
        {
            if(n==0)
            {
                begin=i;
                while(his2[begin]>0 && begin-1>=0)      //4
                {
                    begin--;
                }
            }
            while(his[i]>0 && i<h)
            {
                i++;
            }
        }
        else if(his[i-1]>0&&his[i]==0)
        {
            n=0;
            while(his[i]==0 && i<h)
            {
                n++;
                i++;
            }
            if(n>mean/3 && i<h)          ///////////////แก้ไขได้ตรงนี้
            {
                end=i-n;
                while(his2[end]>0 && end+1<h)      //5
                {
                    end++;
                }
                num++;
                r=(picar **)realloc(r,sizeof(picar *)*num);
                r[num-1]=(picar *)malloc(sizeof(picar));
                r[num-1]->a=&(z->a[begin]);
                r[num-1]->w=z->w;
                r[num-1]->h=end-begin;
                r[num-1]->type=1;
                n=0;
            }
        }
    }
    free(his);
    ///////////////////////
    *row=num;
    return r;
}

onerow **DivideRow2(picar *z,int *row)
{

    int *his,*his2,h=z->h,w=z->w;  //his is histogram of group that have in each row
                                    //his2 is slope of his
    int i,j,s,sum,m;
    Byte **a=z->a;

    his=(int *)malloc(sizeof(int)*h);
    his2=(int *)malloc(sizeof(int)*h);

    for(i=0;i<h;i++)                //loop นี้นะจะทำให้ช้าอยู่พอสมควร
                                    //เพราะมีการ change เยอะมาก
    {
        his[i]=0;
        for(j=0;j<w;j++)
        {
            if(a[i][j]==1)
            {
               his[i]++;
               change(z,i,j,1,2,8);
            }
        }
        for(j=0;j<w;j++)
        {
            if(a[i][j]==2)
            {
               change(z,i,j,2,1,8);
            }
        }
    }
    ShowHis(his,0,z->h,Form1->Series1);
    //เริ่มทำ his2
    for(i=1;i<h;i++)
    {
        his2[i-1]=his[i]-his[i-1];
    }
    for(i=h-2;i>=0;i--)       ///////แก้ไขภายหลัง เลื่อนไปข้างหน้า 1 ช่องทั้งหมด
    {
           his2[i+1]=his2[i];
           his2[i]=0;
    }
    his2[0]=his[0];
    ShowHis(his2,0,z->h,Form1->Series2);

    //สำหรับแท่งในทางเดียวกันที่ติดกัน ให้เอาไปรวมไว้ที่แท่งที่สูงที่สุด
    for(i=0;i<h;i++)
    {
        if(his2[i]!=0 && i<h)
        {
            sum=his2[i];
            m=his2[i];  //ยาวที่สุด
            s=i;        //s คือจุดเริ่มต้น
            his2[i]=0;      
            j=i;
            i++;
            while(his2[i]*m>0)   //เป็นบวกหรือลบเหมือนกัน
            {
                if(m>0)
                {
                    if(abs(his2[i])>abs(m))
                    {
                        m=his2[i];
                        j=i;
                    }
                }
                else if(m<0)
                {
                    if(abs(his2[i])>=abs(m))
                    {
                        m=his2[i];
                        j=i;
                    }
                }
                sum+=his2[i];
                his2[i]=0;
                i++;
            }
            i--;    //เป็นจุดสิ้นสุด
            his2[j]=sum;
        }
    }
    ShowHis(his2,0,z->h,Form1->Series3);


    //count group
    int group=0;

    for(i=1;i<h-1;i++)
    {
       if(his[i]>0 && his[i+1]==0)
       {
            group++;
       }
    }
    if(his[h-1]>0)
    {
        group++;
    }

    //malloc data
    int *max,*min,*begin,*end,*up,*down;

    max=(int *)malloc(sizeof(int)*group);
    min=(int *)malloc(sizeof(int)*group);
    begin=(int *)malloc(sizeof(int)*group);   //ต้นก้อน  สัมพัทธ์หน้ากระดาษ
    end=(int *)malloc(sizeof(int)*group);     //ท้ายก้อน สัมพัทธ์หน้ากระดาษ
    up=(int *)malloc(sizeof(int)*group);      //สูงสุดของ his2 แต่ละก้อน
    down=(int *)malloc(sizeof(int)*group);    //ต่ำสุดของ his2 แต่ละก้อน

    //get data
    int c=0;
    //begin and end
    if(his[0]>0)
    {
        begin[0]=0;
    }
    for(i=0;i<h-1;i++)
    {
        if(his[i]>0 && his[i+1]==0)
        {
            end[c]=i;
            c++;
        }
        else if(his[i]==0 && his[i+1]>0)
        {
            begin[c]=i+1;
        }
    }
    if(his[h-1]>0)
    {
        end[c]=h-1;
    }

    // min max up down
    for(i=0;i<group;i++)
    {
        min[i]=0;
        max[i]=0;
        for(j=begin[i];j<=end[i];j++)
        {
            if(his2[j]>max[i])
            {
                max[i]=his2[j];
                up[i]=j;
            }
            else if(his2[j]<=min[i])
            {
                min[i]=his2[j];
                down[i]=j;
            }
        }
        if(his2[j]<=min[i])         //แก้ bug แล้ว
        {
            min[i]=his2[j];
            down[i]=j;
        }
        down[i]--;
    }
    //fix data
    for(i=0;i<group;i++)
    {
        if(begin[i]==end[i])
        {
            min[i]=0-max[i];
            down[i]=up[i];
        }
    }
    // finish get data
    c=0;     //c=count group that เป็นสระ
    for(i=0;i<group;i++)
    {
        if(begin[i]!=-1)
        {
            //ลองปรับเงื่อนไขในการเป็นสระบนและล่างให้ดี

            if(up[i]-begin[i]<2 && i>0 && begin[i-1]!=-1)     //รวบสระข้างบน แค่ 1 ระดับ
            {
                if(
                    (end[i-1]-begin[i-1] < (down[i]-up[i])*0.95) &&
                    (max[i-1]<=max[i]) &&
                    (min[i-1]>=min[i])
                  )
                {
                    if
                    (
                    (i>1 && begin[i-2]!=-1 && begin[i-1]-end[i-2]<=begin[i]-end[i-1]) ||
                    (i<=1) || begin[i-2]==-1 || end[i-2]-down[i-2]>2
                    )
                    {
                        begin[i]=begin[i-1];
                        begin[i-1]=-1;
                        c++;
                    }
                }
            }
            if(end[i]-down[i]<2 && i<group-1 && begin[i+1]!=-1)     //รวบสระข้างล่าง
            {
                if(
                    (end[i+1]-begin[i+1] < (down[i]-up[i])*0.75) &&
                    (max[i+1]<=max[i]) && ( min[i+1]>=min[i] )
                  )
                {
                    if
                    (
                    (i<group-2 && begin[i+2]!=-1 &&
                    begin[i+2]-end[i+1]>=begin[i+1]-end[i]) ||
                    (i>=group-2)
                    )
                    {
                        end[i]=end[i+1];
                        begin[i+1]=-1;
                        c++;
                    }
                }
            }
        }
    }

    //part to complete
    //find mean (down-up)
    int n=0,mean;
    for(i=0;i<group;i++)
    {
        if(begin[i]!=-1 && down[i]-up[i]>5)
        //down[i]-up[i]>5 น่าจะใช้สถิติ
        {
            n++;
            sum+=down[i]-up[i];
        }
    }
    mean=sum/n;           //mean มันปัดลง อาจได้ค่าไม่น่าเชื่อถือ
    //part with mean

    for(i=0;i<group;i++)
    {
        if(begin[i]!=-1)
        {
            if(end[i]-begin[i]<mean)
            {
               j=i+1;
               while(begin[j]==-1 && j<group)
                    j++;
               if(i<group-1 && j<group &&
                  max[i]<=max[j] && min[i]>=min[j] &&
                  end[i]-begin[i]<down[i]-up[i]*0.95)
                  //ปรับเงื่อนไขตรงนี้ด้วย
               {
                    begin[j]=begin[i];  //รวบสระบน (อักษรระดับ 4) กับบรรทัดอักษรที่อยู่ล่างของมัน
                    begin[i]=-1;
                    c++;
               }
            }
        }
    }
    //sending

    onerow **r;
    r=(onerow **)malloc(sizeof(onerow *)*(group-c));
    for(i=0;i<group-c;i++)
    {
        r[i]=(onerow *)malloc(sizeof(onerow));
        r[i]->z=(picar *)malloc(sizeof(picar));
    }
    n=0;
    for(i=0;i<group;i++)
    {
        if(begin[i]!=-1)
        {
           r[n]->z->a=&(z->a[begin[i]]);
           r[n]->z->w=z->w;
           r[n]->z->h=end[i]-begin[i]+1;
           r[n]->up=up[i]-begin[i];
           r[n]->down=down[i]-begin[i];

           r[n]->tup=0-begin[i];
           r[n]->tdown=z->h-1-begin[i];

           n++;
        }
    }
    free(begin);
    free(end);
    free(up);
    free(down);
    free(max);
    free(min);
    free(his);
    free(his2);
    *row=n;
    return r;
}
/*
void FindBound(onerow *z,int i,int j,bound *b)
{
    if(i<z->tdown && i>=z->tup && j>=0 && j<z->w)
    {
      if(z->a[i][j]==1)
      {
        z->a[i][j]=2;           /////////////สำคัญมาก
        if(i>b->down)
            b->down=i;
        else if(i<b->up)
            b->up=i;
        if(j>b->right)
            b->right=j;
        else if(j<b->left)
            b->left=j;

        FindBound(z,i  ,j-1,b);
        FindBound(z,i  ,j+1,b);

        FindBound(z,i+1,j-1,b);
        FindBound(z,i+1,j  ,b);
        FindBound(z,i+1,j+1,b);

        FindBound(z,i-1,j-1,b);
        FindBound(z,i-1,j  ,b);
        FindBound(z,i-1,j+1,b);
      }
    }
}
*/
void FindBound(picar *z,int i,int j,bound *b,int old,int color)
{
    if(i<z->h && i>=0 && j>=0 && j<z->w)
    {
      if(z->a[i][j]==old)
      {
        z->a[i][j]=color;           /////////////สำคัญมาก
        if(i>b->down)
            b->down=i;
        else if(i<b->up)
            b->up=i;
        if(j>b->right)
            b->right=j;
        else if(j<b->left)
            b->left=j;

        FindBound(z,i  ,j-1,b,old,color);
        FindBound(z,i  ,j+1,b,old,color);

        FindBound(z,i+1,j-1,b,old,color);
        FindBound(z,i+1,j  ,b,old,color);
        FindBound(z,i+1,j+1,b,old,color);

        FindBound(z,i-1,j-1,b,old,color);
        FindBound(z,i-1,j  ,b,old,color);
        FindBound(z,i-1,j+1,b,old,color);
      }
    }
}
void FindBoundFont(picar *z,int i,int j,bound *b)
{
    if(i>=0 && i<z->h && j>=0 && j<z->w)
    {
      if(z->a[i][j]==1)
      {
        z->a[i][j]=2;        /////////////สำคัญมาก
        if(i>b->down)
            b->down=i;
        else if(i<b->up)
            b->up=i;
        if(j>b->right)
            b->right=j;
        else if(j<b->left)
            b->left=j;

        FindBoundFont(z,i  ,j-1,b);
        FindBoundFont(z,i  ,j+1,b);

        FindBoundFont(z,i+1,j-1,b);
        FindBoundFont(z,i+1,j  ,b);
        FindBoundFont(z,i+1,j+1,b);

        FindBoundFont(z,i-1,j-1,b);
        FindBoundFont(z,i-1,j  ,b);
        FindBoundFont(z,i-1,j+1,b);
      }
    }
}

alpha *suck(onerow *z,int ai,int aj,bound *b,int p) //p=2=mid p=1=สระ
{
    alpha *r;
    int i,j;

    //set bound first value
    b->up=b->down=ai;
    b->left=b->right=aj;
    //find bound
    FindBound(z->z,ai,aj,b,1,2);
  if((b->down-b->up < (z->down-z->up)*0.95)||p==2)
  {
    //put data in r for return
    r=(alpha *)malloc(sizeof(alpha));
    //r->z->h=b->down-b->up+1;
    //r->z->w=b->right-b->left+1;
    r->up=z->up-b->up;
    r->down=z->down-b->up;

    r->z=NewPicar(b->down-b->up+1,b->right-b->left+1,1);
    /*
    r->z->a=(Byte **)malloc(sizeof(Byte *)*r->h);
    for(i=0;i<r->h;i++)
    {
        r->a[i]=(Byte *)malloc(sizeof(Byte *)*r->w);
    }
    */
    for(i=b->up;i<=b->down;i++)
    {
        for(j=b->left;j<=b->right;j++)
        {
            if(z->z->a[i][j]==2)
            {
                z->z->a[i][j]=3;
                r->z->a[i-b->up][j-b->left]=1;
            }
            else
            {
                r->z->a[i-b->up][j-b->left]=0;
            }
        }
    }
    return r;
  }
  else
  {
    //เปลี่ยนกลับคืน
    for(i=b->up;i<=b->down;i++)
    {
        for(j=b->left;j<=b->right;j++)
        {
            if(z->z->a[i][j]==2)
            {
                z->z->a[i][j]=1;

            }
        }
    }
    return NULL;
  }
}

alpha *suck(picar *z,int up,int down,int ai,int aj,bound *b,int p) //p=2=mid p=1=สระ
{
    alpha *r;
    int i,j;

    //set bound first value
    b->up=b->down=ai;
    b->left=b->right=aj;
    //find bound
    FindBound(z,ai,aj,b,1,2);
    if((b->down-b->up < (down-up)*0.95)||p==2) //ถ้าเป็นสระต้องเป็นสระที่ไม่ใหญ่จนเกินไป
    {
        //put data in r for return
        r=(alpha *)malloc(sizeof(alpha));
        //r->z->h=b->down-b->up+1;
        //r->z->w=b->right-b->left+1;
        r->up=up-b->up;
        r->down=down-b->up;

        r->z=NewPicar(b->down-b->up+1,b->right-b->left+1,1);
        /*
        r->z->a=(Byte **)malloc(sizeof(Byte *)*r->h);
        for(i=0;i<r->h;i++)
        {
            r->a[i]=(Byte *)malloc(sizeof(Byte *)*r->w);
        }
        */
        for(i=b->up;i<=b->down;i++)
        {
            for(j=b->left;j<=b->right;j++)
            {
                if(z->a[i][j]==2)
                {
                    z->a[i][j]=3;
                    r->z->a[i-b->up][j-b->left]=1;
                }
                else
                {
                    r->z->a[i-b->up][j-b->left]=0;
                }
            }
        }
        return r;
    }
    else
    {
        //เปลี่ยนกลับคืน
        for(i=b->up;i<=b->down;i++)
        {
            for(j=b->left;j<=b->right;j++)
            {
                if(z->a[i][j]==2)
                {
                    z->a[i][j]=1;
                }
            }
        }
        return NULL;
    }
}
alpha *SuckFont(picar *z,int ai,int aj,int up,int down)
{
    alpha *r;
    int i,j;
    bound *b;
    b=(bound *)malloc(sizeof(bound));
    //set bound first value
    b->up=b->down=ai;
    b->left=b->right=aj;
    //find bound
    FindBoundFont(z,ai,aj,b);

    //put data in r for return
    r=(alpha *)malloc(sizeof(alpha));
    /*
    r->h=b->down-b->up+1;
    r->w=b->right-b->left+1;
    */
    r->up= up - b->up;
    r->down= down - b->up;
    /*
    r->a=(Byte **)malloc(sizeof(Byte *)*r->h);
    for(i=0;i<r->h;i++)
    {
        r->a[i]=(Byte *)malloc(sizeof(Byte *)*r->w);
    }
    */
    r->z=NewPicar(b->down-b->up+1,b->right-b->left+1,1);
    for(i=b->up;i<=b->down;i++)
    {
        for(j=b->left;j<=b->right;j++)
        {
            if(z->a[i][j]==2)
            {
                z->a[i][j]=3;
                r->z->a[i-b->up][j-b->left]=1;
            }
            else
            {
                r->z->a[i-b->up][j-b->left]=0;
            }
        }
    }
    free(b);
    return r;
}

alpha **GetAlpha(onerow *z,int *x)
{
    int *his,*his2,max,min,enter=0;  //his is simply histogram
                                    //his2 is slope of his
    int i,j,k,l,count=0,n,sla,h=z->z->h,w=z->z->w;
    int farest=0; //ใช้ในการหา gap ของ alpha
    int lastup;
    Byte **a=z->z->a;
    alpha **r;
    bound *b,*b2;
    b=(bound *)malloc(sizeof(bound));
    b2=(bound *)malloc(sizeof(bound));
    r=(alpha **)malloc(sizeof(alpha *));
    //จะดึงตัวอักษรออกมา
    while(1)
    {
        n=0;
        for(j=0;j<w;j++)
        {
            for(i=z->up;i<z->down+1;i++)
            {
                if(a[i][j]==1)
                {
                    count++;
                    r=(alpha **)realloc(r,sizeof(alpha *)*count);

                    r[count-1]=suck(z,i,j,b,2);   //suck ช่วย NewPicar แล้ว
                    r[count-1]->type=2; //middle
                    n++;
                    if(count-1==0)
                    {
                        r[count-1]->gap=0;
                    }
                    else
                    {
                        r[count-1]->gap=j-farest;
                    }
                    if(j+r[count-1]->z->w-1>farest)
                    {
                        farest=j+r[count-1]->z->w-1;
                    }
                    else
                    //SaveArray(z->a,z->h,z->w,"C:\\new\new.bmp");
                    //ลงไปสำรวจข้างล่าง
                    sla=0;

                    for(k=z->down+2;k<z->tdown && k<z->down+((z->down-z->up)/2);k++)
                    //
                    {
                        for(l=b->left;l<=b->right;l++)
                        {
                            if(a[k][l]==1)
                            {
                                count++;
                                r=(alpha **)realloc(r,sizeof(alpha *)*count);
                                r[count-1]=suck(z,k,l,b2,1);
                                if(r[count-1]==NULL)
                                {
                                    count--;
                                    free(&r[count]);
                                    r=(alpha **)realloc(r,sizeof(alpha *)*count);
                                }
                                else
                                {
                                    r[count-1]->type=1; //low    //แก้แล้ว
                                    n++;
                                    sla=1;
                                    r[count-1]->gap=j-farest;
                                    break;
                                }
                            }
                        }
                        if(sla==1)
                        {
                            break;
                        }
                    }

                    //สำรวจด้านบน
                    sla=0;

                    for(k=z->up-2;k>z->tup && k>z->up-((z->down-z->up)*1);k--)

                    {
                        for(l=b->left;l<=b->right;l++)   //ตรงนี้ควรแคบเข้าอีกหน่อย แต่ก็กลัวขาดไม้เอก
                        {
                            if(a[k][l]==1)
                            {
                                count++;
                                r=(alpha **)realloc(r,sizeof(alpha *)*count);
                                r[count-1]=suck(z,k,l,b2,1);
                                if(r[count-1]==NULL || b2->down-b2->up > z->down-z->up) //อันหลังเป็นกรณีที่ ไ ใ โ ยื่นเข้ามาข้างบน
                                {
                                    count--;
                                    free(&r[count]);
                                    r=(alpha **)realloc(r,sizeof(alpha *)*count);
                                    change(z->z,k,l,3,1,8);
                                }
                                else if(sla==1 && b2->down>lastup && r[count-2]->z->w>(b->right-b->left)/2)  //กรณีของ ื ที่แยกออกไม่หมด
                                {
                                    count--;
                                    free(&r[count]);
                                    r=(alpha **)realloc(r,sizeof(alpha *)*count);
                                    r[count-1]->c[1]='ื';
                                    r[count-1]->ans=215;
                                }
                                else
                                {
                                    r[count-1]->type=sla+3; //high   //แก้แล้ว
                                    n++;
                                    sla++;
                                    r[count-1]->gap=j-farest;

                                    if(sla==1)
                                    {
                                        //k-=r[count-1]->h
                                        lastup=b2->up;
                                    }
                                    else if(sla==2)
                                        break;
                                }
                            }
                        }
                        if(sla==2)
                            break;
                    }

                }
            }
        }
        //เพิ่ม enter ที่ท้ายบรรทัด
        count++;
        r=(alpha **)realloc(r,sizeof(alpha *)*count);
        r[count-1]=(alpha *)malloc(sizeof(alpha));
        r[count-1]->z=(picar *)malloc(sizeof(picar));
        r[count-1]->type=0;
        r[count-1]->z->w=n;   //จะนำไปใช้เรียงในภายหลัง
        r[count-1]->down=z->down;
        enter++;
        farest=0;
        r[count-1]->gap=0;
        //ทำงานเสร็จหนึ่งบรรทัดในนั้น แต่อาจเหลือบรรทัดอื่นอีก

        his=(int *)malloc(sizeof(int)*h);
        his2=(int *)malloc(sizeof(int)*h);
        for(i=0;i<h;i++)
        {
            his[i]=0;
            for(j=0;j<w;j++)
            {
                if(a[i][j]==1)
                {
                    his[i]++;
                    change(z->z,i,j,1,2,8);
                }
            }
            for(j=0;j<w;j++)
            {
                if(a[i][j]==2)
                {
                    change(z->z,i,j,2,1,8);
                }
            }
        }
        for(i=1;i<h;i++)
        {
            his2[i-1]=his[i]-his[i-1];
        }
        for(i=0;i<h;i++)
        {
            if(his2[i]>0)
            {
                j=i+1;
                while(his2[j]>0 && j<h)
                {
                    his2[i]+=his2[j];
                    his2[j]=0;
                    j++;
                }
                i=j;
            }
        }
        for(i=h-2;i>=0;i--)       ///////แก้ไขภายหลัง
        {
            if(his2[i]>0)
            {
                his2[i+1]=his2[i];
                his2[i]=0;
            }
        }
        for(i=h-1;i>=0;i--)
        {
            if(his2[i]<0)
            {
                j=i-1;
                while(his2[j]<0 && j>=0)
                {
                    his2[i]+=his2[j];
                    his2[j]=0;
                    j--;
                }
                i=j;
            }
        }
        his2[0]=his[0];
        // end make histogram

        max=min=0;
        for(i=0;i<h;i++)
        {
            if(his2[i]>=his2[max])
            {
                max=i;
            }
            else if(his2[i]<his2[min])
            {
                min=i;
            }
        }
        if(min-5>max && his2[max]>=0 && his2[min]<=0) //5 ไม่มีที่มี หาสถิติมาช่วย

        {
            z->up=max;
            z->down=min;
        }
        else
        {
            free(his);
            free(his2);
            break;
        }
        free(his);
        free(his2);
    }

    *x=count;
    free(b);
    free(b2);

    //ดึงตัวอักษรเสร็จแล้ว

    //จัดเรียงใหม่( สลับบรรทัด โดยดูจาก r[i]->type ที่==0 )

    alpha **r2;
    int at;
    r2=(alpha **)malloc(sizeof(alpha *)*count);

    max=0;
    int p=0;
    for(k=0;k<enter;k++)
    {
        i=count-1;
        min=4000000;    ////////
        while(i>=0)
        {
            if(r[i]->down > max && r[i]->down < min)
            {
                min=r[i]->down;
                at=i;
            }
            i=i-r[i]->z->w-1;
        }
        for(i=p,j=at-r[at]->z->w;i<=p+r[at]->z->w;i++,j++)
        {
            r2[i]=r[j];
        }
        p=i;
        max=min;
    }
    for(i=0;i<count;i++)
    {
        r[i]=r2[i];
    }
    free(r2);
    return r;

}

paragraph *GetAlpha2(picar *z)
{
    int *his,*his2,max,min,enter=0;  //his is simply histogram
                                    //his2 is slope of his
    int i,j,k,l,count,n,sla,h=z->h,w=z->w;
    int farest=0; //ใช้ในการหา gap ของ alpha
    int lastup;
    Byte **a=z->a;
    alpha ***r;
    bound *b,*b2;
    b=(bound *)malloc(sizeof(bound));
    b2=(bound *)malloc(sizeof(bound));
    r=(alpha ***)malloc(sizeof(alpha **));

    int sum,m,s;
    int up,down; //แทน z->up และ z->down//
    int *row,*num;
    int *sort;
    int newhis[2]={0,h}; //เอาไว้เก็บว่า his ช่วงไหนบ้างถูกรบกวนและต้อง update
    sort=(int *)malloc(sizeof(int));
    row=(int *)malloc(sizeof(int));
    num=(int *)malloc(sizeof(int));
    //จะดึงตัวอักษรออกมา
    his=(int *)malloc(sizeof(int)*h);
    his2=(int *)malloc(sizeof(int)*h);

    while(1)
    {
        //หาบรรทัดที่เด่นที่สุดก่อน

        for(i=newhis[0];i<newhis[1];i++)
        {
            his[i]=0;
            for(j=0;j<w;j++)
            {
                if(a[i][j]==1)
                {
                    his[i]++;
                    change(z,i,j,1,2,8);
                }
            }
            for(j=0;j<w;j++)
            {
                if(a[i][j]==2)
                {
                    change(z,i,j,2,1,8);
                }
            }
        }

        for(i=1;i<h;i++)
        {
            his2[i-1]=his[i]-his[i-1];
        }
        //copy
        for(i=h-2;i>=0;i--)
        {
           his2[i+1]=his2[i];
           his2[i]=0;
        }
        his2[0]=his[0];
        //รวบความชันให้เป็นแท่งเดียว
        for(i=0;i<h;i++)
        {
            if(his2[i]!=0 && i<h)
            {
                sum=his2[i];
                m=his2[i];  //ยาวที่สุด
                s=i;        //s คือจุดเริ่มต้น
                his2[i]=0;
                j=i;
                i++;
                while(his2[i]*m>0)   //เป็นบวกหรือลบเหมือนกัน
                {
                    if(m>0)
                    {
                        if(abs(his2[i])>abs(m))
                        {
                            m=his2[i];
                            j=i;
                        }
                    }
                    else if(m<0)
                    {
                        if(abs(his2[i])>=abs(m))
                        {
                            m=his2[i];
                            j=i;
                        }
                    }
                    sum+=his2[i];
                    his2[i]=0;
                    i++;
                }
                i--;    //เป็นจุดสิ้นสุด
                his2[j]=sum;
            }
        }
        //end of copy
        /*
        for(i=0;i<h;i++)
        {
            if(his2[i]>0)
            {
                j=i+1;
                while(his2[j]>0 && j<h)
                {
                    his2[i]+=his2[j];
                    his2[j]=0;
                    j++;
                }
                i=j;
            }
        }
        for(i=h-2;i>=0;i--)       ///////แก้ไขภายหลัง
        {
            if(his2[i]>0)
            {
                his2[i+1]=his2[i];
                his2[i]=0;
            }
        }
        for(i=h-1;i>=0;i--)
        {
            if(his2[i]<0)
            {
                j=i-1;
                while(his2[j]<0 && j>=0)
                {
                    his2[i]+=his2[j];
                    his2[j]=0;
                    j--;
                }
                i=j;
            }
        }
        his2[0]=his[0];
        */

        // end make histogram
        
        max=min=0;
        for(i=0;i<h;i++)
        {
            if(his2[i]>his2[max])
            {
                max=i;
            }
            /*
            else if(his2[i]<his2[min])
            {
                min=i;
            }
            */
        }

        //ตามล่าหา min ที่อยู่ในภูเขา his เดียวกับ max
        j=0;
        for(i=max;i<h;i++)
        {
            if(his[i]==0)
            {
                j++;
                if(j==2)         //ถ่วงเวลาหลังหมดภูเขา
                    break;
            }
            if(his2[i]<=his2[min])
            {
                min=i;
            }
        }
        if(min-5>max && his2[max]>0 && his2[min]<0) //5 ไม่มีที่มี หาสถิติมาช่วย
        {
            up=max;
            down=min-1;
            newhis[0]=up;
            newhis[1]=down;
        }
        else
        {
            //free(his);    แก้ Bug 26/7/50
            //free(his2);
            break;
        }


        //จบการหา up กับ down เพื่อใช้ระบุเจาะจงบรรทัด
        n=0;
        count=0;

        r=(alpha ***)realloc(r,sizeof(alpha **)*enter+1);
        r[enter]=(alpha **)malloc(sizeof(alpha *));
        for(j=0;j<w;j++)
        {
            for(i=up;i<down+1;i++)
            {
                if(a[i][j]==1)
                {
                    count++;
                    r[enter]=(alpha **)realloc(r[enter],sizeof(alpha *)*count);

                    r[enter][count-1]=suck(z,up,down,i,j,b,2);   //suck ช่วย NewPicar แล้ว
                    if(b->up<newhis[0])
                    {
                        newhis[0]=b->up;
                    }
                    if(b->down>newhis[1])
                    {
                        newhis[1]=b->down;
                    }
                    r[enter][count-1]->type=2; //middle
                    n++;
                    if(count-1==0)
                    {
                        r[enter][count-1]->gap=0;
                    }
                    else
                    {
                        r[enter][count-1]->gap=j-farest;
                    }
                    if(j+r[enter][count-1]->z->w-1>farest)
                    {
                        farest=j+r[enter][count-1]->z->w-1;
                    }
                    else
                    //SaveArray(z->a,z->h,z->w,"C:\\new\new.bmp");
                    //ลงไปสำรวจข้างล่าง
                    sla=0;

                    for(k=down+2;k<z->h && k<down+((down-up)/2);k++)
                    //
                    {
                        for(l=b->left;l<=b->right;l++)
                        {
                            if(a[k][l]==1)
                            {
                                count++;
                                r[enter]=(alpha **)realloc(r[enter],sizeof(alpha *)*count);
                                r[enter][count-1]=suck(z,up,down,k,l,b2,1);
                                if(r[enter][count-1]==NULL)
                                {
                                    count--;
                                    free(&r[enter][count]);
                                    r[enter]=(alpha **)realloc(r[enter],sizeof(alpha *)*count);
                                }
                                else
                                {
                                    if(b2->up<newhis[0])
                                    {
                                        newhis[0]=b2->up;
                                    }
                                    if(b2->down>newhis[1])
                                    {
                                        newhis[1]=b2->down;
                                    }
                                    r[enter][count-1]->type=1; //low    //แก้แล้ว
                                    n++;
                                    sla=1;
                                    r[enter][count-1]->gap=j-farest;
                                    break;
                                }
                            }
                        }
                        if(sla==1)
                        {
                            break;
                        }
                    }

                    //สำรวจด้านบน
                    sla=0;

                    for(k=up-2;k>0 && k>up-((down-up)*1);k--)

                    {
                        for(l=b->left;l<=b->right;l++)   //ตรงนี้ควรแคบเข้าอีกหน่อย แต่ก็กลัวขาดไม้เอก
                        {
                            if(a[k][l]==1)
                            {
                                count++;
                                r[enter]=(alpha **)realloc(r[enter],sizeof(alpha *)*count);
                                r[enter][count-1]=suck(z,up,down,k,l,b2,1);
                                if(r[enter][count-1]==NULL || b2->down-b2->up > down-up) //อันหลังเป็นกรณีที่ ไ ใ โ ยื่นเข้ามาข้างบน
                                {
                                    count--;
                                    free(&r[enter][count]);
                                    r[enter]=(alpha **)realloc(r[enter],sizeof(alpha *)*count);
                                    change(z,k,l,3,1,8);
                                }
                                else if(sla==1 && b2->down>lastup && r[enter][count-2]->z->w>(b->right-b->left)/2)  //กรณีของ ื ที่แยกออกไม่หมด
                                {
                                    count--;
                                    free(&r[enter][count]);
                                    r[enter]=(alpha **)realloc(r[enter],sizeof(alpha *)*count);
                                    r[enter][count-1]->c[1]='ื';
                                    r[enter][count-1]->ans=215;
                                }
                                else
                                {
                                    if(b2->up<newhis[0])
                                    {
                                        newhis[0]=b2->up;
                                    }
                                    if(b2->down>newhis[1])
                                    {
                                        newhis[1]=b2->down;
                                    }
                                    r[enter][count-1]->type=sla+3; //high   //แก้แล้ว
                                    n++;
                                    sla++;
                                    r[enter][count-1]->gap=j-farest;

                                    if(sla==1)
                                    {
                                        //k-=r[count-1]->h
                                        lastup=b2->up;
                                    }
                                    else if(sla==2)
                                        break;
                                }
                            }
                        }
                        if(sla==2)
                            break;
                    }

                }
            }
        }
        //เพิ่ม enter ที่ท้ายบรรทัด
        count++;
        r[enter]=(alpha **)realloc(r[enter],sizeof(alpha *)*count);
        r[enter][count-1]=(alpha *)malloc(sizeof(alpha));
        r[enter][count-1]->z=(picar *)malloc(sizeof(picar));
        r[enter][count-1]->type=0;
        r[enter][count-1]->gap=0;
        enter++;
        farest=0;

        num=(int *)realloc(num,sizeof(int)*enter);
        num[enter-1]=count;
        sort=(int *)realloc(sort,sizeof(int)*enter);
        sort[enter-1]=down;
        //ทำงานเสร็จหนึ่งบรรทัด
        newhis[1]++;
    }

    *row=enter;
    free(b);
    free(b2);
    free(his);
    free(his2);
    //ดึงตัวอักษรเสร็จแล้ว

    //จัดเรียงใหม่
    alpha **mem;
    int x,y;
    for(i=0;i<*row-1;i++)
    {
        min=sort[i];
        x=i;
        for(j=i+1;j<*row;j++)
        {
            if(sort[j]<min)
            {
                min=sort[j];
                x=j;
            }
        }
        if(x!=i)
        {
            mem=r[i];
            r[i]=r[x];
            r[x]=mem;

            y=sort[i];
            sort[i]=sort[x];
            sort[x]=y;

            y=num[i];
            num[i]=num[x];
            num[x]=y;
        }
    }
    free(sort);
    paragraph *para;
    para=(paragraph *)malloc(sizeof(paragraph));
    para->c=r;
    para->row=row;
    para->num=num;
    return para;
}

int InRight(picar *z)
{
    int sum=0,j=z->w-1,i;
    while(sum==0)
    {
        for(i=0;i<z->h;i++)
        {
            if(z->a[i][j]==1)
            {
                sum++;
                break;
            }
        }
        j--;
    }
    j+=2;
    return j;
}
int InLeft(picar *z)
{
    int sum=0,j=0,i;
    while(sum==0)
    {
        for(i=0;i<z->h;i++)
        {
            if(z->a[i][j]==1)
            {
                sum++;
                break;
            }
        }
        j++;
    }
    j--;
    return j;
}
int InUp(picar *z)
{
    int sum=0,j,i=0;
    while(sum==0)
    {
        for(j=0;j<z->w;j++)
        {
            if(z->a[i][j]==1)
            {
                sum++;
                break;
            }
        }
        i++;
    }
    i--;
    return i;
}
int InDown(picar *z)
{
    int sum=0,j,i=z->h-1;
    while(sum==0)
    {
        for(j=0;j<z->w;j++)
        {
            if(z->a[i][j]==1)
            {
                sum++;
                break;
            }
        }
        i--;
    }
    i+=2;
    return i;
}
picar *GetInBound(alpha *ascii,int ini,int ini2,int inj,int inj2)
{
    int i,j;
    picar *mem;
    mem=NewPicar(ini2-ini,inj2-inj,1);

    for(i=0;i<mem->h;i++)
    {
        for(j=0;j<mem->w;j++)
        {
            if(i+ini >=0 && i+ini < ascii->z->h && j+inj>=0 && j+inj < ascii->z->w)
            {
                mem->a[i][j]=ascii->z->a[i+ini][j+inj];
            }

            else
            {
                mem->a[i][j]=0;
            }
        }
    }
    return mem;
}

void GetFeature(feature *test,picar *mem,int up,int down)     //up และ down ของ picar
{
    int i,j;
    int ini=InUp(mem);
    int inj=InLeft(mem);
    int ini2=InDown(mem);
    int inj2=InRight(mem);

    test->z=NewPicar(ini2-ini,inj2-inj,1);
    /*
    test->z->w=inj2-inj;
    test->z->h=ini2-ini;
    test->z->a=NewArray(test->z->h,test->z->w);
    */
    for(i=0;i<test->z->h;i++)
    {
        for(j=0;j<test->z->w;j++)
        {
             test->z->a[i][j]=mem->a[i+ini][j+inj];
        }
    }
    test->left=inj;
    test->up=up-ini;
    test->down=down-ini;
}
void LoadFeature(myfont *f)
{
    int i,j,x,s,ini,inj,ini2,inj2,sum,count,last;
    picar *mem;
    f->test=(feature **)malloc(sizeof(feature *)*10);
    for(i=0;i<10;i++)
    {
        f->test[i]=(feature *)malloc(sizeof(feature));
    }
    //feture0 หัว 'ก'

    x=161;
    s=0;
    mem=GetInBound(f->ascii[x],f->ascii[x]->up,f->ascii[x]->up+(f->h/2),0,f->ascii[x]->z->w/2);
    //SavePicar(mem,"C:\\feature00.bmp");
    GetFeature(f->test[s],mem,0,f->h);
    FreePicar(mem);
    //SaveArray(f->test[0]->z->a,f->test[0]->z->h,f->test[0]->z->w,"C:\\feture0.bmp");

    //feture1 หัว 'ถ'
    x=182;
    s=1;
    inj2=f->ascii[x]->z->w/2;
    sum=2;

    while(sum>1)
    {
        sum=0;
        last=0;
        for(i=0;i<f->ascii[x]->z->h;i++)
        {
            if(f->ascii[x]->z->a[i][inj2]==1 && last==0)
            {
                sum++;
            }
            last=f->ascii[x]->z->a[i][inj2];
        }
        inj2++;
    }
    inj2--;

    mem=GetInBound(f->ascii[x],(f->ascii[x]->down+f->ascii[x]->up)/2,f->ascii[x]->down+1,0,inj2);
    //SavePicar(mem,"C:\\feature00.bmp");
    GetFeature(f->test[s],mem,f->ascii[x]->up-(f->ascii[x]->down+f->ascii[x]->up)/2,f->ascii[x]->down-(f->ascii[x]->down+f->ascii[x]->up)/2);
    FreePicar(mem);

    //SaveArray(f->test[1]->z->a,f->test[1]->z->h,f->test[1]->z->w,"C:\\feture1.bmp");


    //feture2 หัว 'ภ' //เหมือนหัว 'ถ' เลย
    x=192;
    s=2;
    inj2=f->ascii[x]->z->w/2;
    sum=2;
    while(sum>1)
    {
        sum=0;
        last=0;
        for(i=0;i<f->ascii[x]->z->h;i++)
        {
            if(f->ascii[x]->z->a[i][inj2]==1 && last==0)
            {
                sum++;
            }
            last=f->ascii[x]->z->a[i][inj2];
        }
        inj2++;
    }
    inj2--;

    mem=GetInBound(f->ascii[x],(f->ascii[x]->down+f->ascii[x]->up)/2,f->ascii[x]->down+1,0,inj2);
    //SavePicar(mem,"C:\\feature00.bmp");
    GetFeature(f->test[s],mem,f->ascii[x]->up-(f->ascii[x]->down+f->ascii[x]->up)/2,f->ascii[x]->down-(f->ascii[x]->down+f->ascii[x]->up)/2);
    FreePicar(mem);

    //SaveArray(f->test[2]->z->a,f->test[2]->z->h,f->test[2]->z->w,"C:\\feture2.bmp");

    //feture3 หัว 'บ'
    x=186;
    s=3;
    mem=GetInBound(f->ascii[x],f->ascii[x]->up,f->ascii[x]->up+(f->h/2),0,f->ascii[x]->z->w/2);
    //SavePicar(mem,"C:\\feature00.bmp");
    GetFeature(f->test[s],mem,0,f->h);
    FreePicar(mem);
    //SaveArray(f->test[3]->z->a,f->test[3]->z->h,f->test[3]->z->w,"C:\\feture3.bmp");

    //feture4 หัว 'ข' ต้องทำ histogram และทำ  ด้วย
    int *hisi,*hisj;
    
    x=162;
    s=4;
    //hisi=(int *)malloc(sizeof(int)*f->ascii[x]->z->h);
    hisj=(int *)malloc(sizeof(int)*f->ascii[x]->z->w);
    //make his
    /*
    for(i=0;i<f->ascii[x]->z->h;i++)
    {
        hisi[i]=0;
    }
    */
    for(j=0;j<f->ascii[x]->z->w;j++)
    {
        hisj[j]=0;
    }
    for(i=0;i<f->ascii[x]->z->h;i++)
    {
        for(j=0;j<f->ascii[x]->z->w;j++)
        {
            hisj[j]+=f->ascii[x]->z->a[i][j];
            //hisi[i]+=f->ascii[x]->z->a[i][j];
        }
    }
    //หา inj2
    /*
    last=0;
    count=0;
    for(i=f->ascii[x]->z->h-1;i>=0;i--)
    {
        if(count==0)
        {
            if(hisi[i]<last)
                count++;
        }
        else if(count==1)
        {
            if(hisi[i]>last)
                count++;
        }
        else if(count==2)
        {
            break;
        }
        last=hisi[i];
    }
    ini2=i+1;
    */
    count=0;
    last=0;
    for(j=f->ascii[x]->z->w-1;j>=0;j--)
    {
        if(count==0)
        {
            if(hisj[j]<last)
                count++;
        }
        else if(count==1)
        {
            if(hisj[j]>last)
                count++;
        }
        else if(count==2)
        {
            break;
        }
        last=hisj[j];
    }
    inj2=j+1;
    free(hisj);
    //หา ini2 โดยไล่จากข้างล่างหาเส้นที่ผ่านมากกว่าสองกลุ่ม
    i=f->ascii[x]->z->h-1;
    sum=0;
    while(sum<=2 && i>=0)
    {
        last=0;
        sum=0;
        for(j=0;j<f->ascii[x]->z->w;j++)
        {
            if(f->ascii[x]->z->a[i][j]==1 && last==0)
            {
                sum++;
            }
            last=f->ascii[x]->z->a[i][j];
        }
        i--;
    }
    if(i==-1)
    {
        ini2=f->ascii[x]->z->h/2;
    }
    else
    {
        ini2=i+2;
    }

    //หาเสร็จแล้ว
    mem=GetInBound(f->ascii[x],f->ascii[x]->up,ini2,0,inj2);
    //SavePicar(mem,"C:\\feature00.bmp");
    GetFeature(f->test[s],mem,0,f->h);
    FreePicar(mem);
    //SaveArray(f->test[s]->z->a,f->test[s]->z->h,f->test[s]->z->w,"C:\\feture4.bmp");

    //feture5 หัว 'ฃ' ต้องทำ histogram และทำ  ด้วย เหมือน 'ข' ทุกอย่าง
    x=163;
    s=5;

    hisj=(int *)malloc(sizeof(int)*f->ascii[x]->z->w);

    for(j=0;j<f->ascii[x]->z->w;j++)
    {
        hisj[j]=0;
    }
    for(i=0;i<f->ascii[x]->z->h;i++)
    {
        for(j=0;j<f->ascii[x]->z->w;j++)
        {
            hisj[j]+=f->ascii[x]->z->a[i][j];
        }
    }
    count=0;
    last=0;
    for(j=f->ascii[x]->z->w-1;j>=0;j--)
    {
        if(count==0)
        {
            if(hisj[j]<last)
                count++;
        }
        else if(count==1)
        {
            if(hisj[j]>last)
                count++;
        }
        else if(count==2)
        {
            break;
        }
        last=hisj[j];
    }
    inj2=j+1;
    free(hisj);
    //หา ini2 โดยไล่จากข้างล่างหาเส้นที่ผ่านมากกว่าสองกลุ่ม
    i=f->ascii[x]->z->h-1;
    sum=0;
    while(sum<=2 && i>=0)
    {
        last=0;
        sum=0;
        for(j=0;j<f->ascii[x]->z->w;j++)
        {
            if(f->ascii[x]->z->a[i][j]==1 && last==0)
            {
                sum++;
            }
            last=f->ascii[x]->z->a[i][j];
        }
        i--;
    }
    if(i==-1)
    {
        ini2=f->ascii[x]->z->h/2;
    }
    else
    {
        ini2=i+2;
    }
    //หาเสร็จแล้ว
    //ShowHis(hisi,0,f->ascii[x]->z->h,Form1->Series1);
    //ShowHis(hisj,0,f->ascii[x]->w,Form1->Series2);
    mem=GetInBound(f->ascii[x],f->ascii[x]->up,ini2,0,inj2);
    //SavePicar(mem,"C:\\feature00.bmp");
    GetFeature(f->test[s],mem,0,f->h);
    f->test[s]->left=-1;
    FreePicar(mem);
    //SaveArray(f->test[s]->z->a,f->test[s]->z->h,f->test[s]->z->w,"C:\\feture5.bmp");

    //Feature6 หัว 'ด'
    x=180;
    s=6;
    inj2=f->ascii[x]->z->w/2;
    sum=2;
    while(sum>1)
    {
        sum=0;
        last=0;
        for(i=0;i<f->ascii[x]->z->h;i++)
        {
            if(f->ascii[x]->z->a[i][inj2]==1 && last==0)
            {
                sum++;
            }
            last=f->ascii[x]->z->a[i][inj2];
        }
        inj2++;
    }
    inj2--;

    i=0;
    sum=0;
    while(sum<=2 && i<f->ascii[x]->z->h)
    {
        last=0;
        sum=0;
        for(j=0;j<f->ascii[x]->z->w;j++)
        {
            if(f->ascii[x]->z->a[i][j]==1 && last==0)
            {
                sum++;
            }
            last=f->ascii[x]->z->a[i][j];
        }
        i++;
    }
    if(i==f->ascii[x]->z->h)
    {
        ini=f->ascii[x]->z->h/2;
    }
    else
    {
        ini=i-1;
    }
    mem=GetInBound(f->ascii[x],ini,f->ascii[x]->down+1,0,inj2);
    //SavePicar(mem,"C:\\feature00.bmp");
    GetFeature(f->test[s],mem,f->ascii[x]->up-ini,f->ascii[x]->down-ini);
    FreePicar(mem);
    //SaveArray(f->test[s]->z->a,f->test[s]->z->h,f->test[s]->z->w,"C:\\feture6.bmp");

    //Feature7 หัว 'ค'  เหมือน ด ทั้งหมด
    x=164;
    s=7;
    inj2=f->ascii[x]->z->w/2;
    sum=2;
    while(sum>1)
    {
        sum=0;
        last=0;
        for(i=0;i<f->ascii[x]->z->h;i++)
        {
            if(f->ascii[x]->z->a[i][inj2]==1 && last==0)
            {
                sum++;
            }
            last=f->ascii[x]->z->a[i][inj2];
        }
        inj2++;
    }
    inj2--;

    i=0;
    sum=0;
    while(sum<=2 && i<f->ascii[x]->z->h)
    {
        last=0;
        sum=0;
        for(j=0;j<f->ascii[x]->z->w;j++)
        {
            if(f->ascii[x]->z->a[i][j]==1 && last==0)
            {
                sum++;
            }
            last=f->ascii[x]->z->a[i][j];
        }
        i++;
    }
    if(i==f->ascii[x]->z->h)
    {
        ini=f->ascii[x]->z->h/2;
    }
    else
    {
        ini=i-1;
    }
    mem=GetInBound(f->ascii[x],ini,f->ascii[x]->down+1,0,inj2);
    //SavePicar(mem,"C:\\feature00.bmp");
    GetFeature(f->test[s],mem,f->ascii[x]->up-ini,f->ascii[x]->down-ini);
    FreePicar(mem);
    //SaveArray(f->test[s]->z->a,f->test[s]->z->h,f->test[s]->z->w,"C:\\feture7.bmp");

    //Feature8 ครึ่งล่างของ 'ล'
    x=197;
    s=8;
    mem=GetInBound(f->ascii[x],(f->ascii[x]->up+f->ascii[x]->down)/2,f->ascii[x]->down+1,0,f->ascii[x]->z->w);
    //SavePicar(mem,"C:\\feature00.bmp");
    GetFeature(f->test[s],mem,f->ascii[x]->up-(f->ascii[x]->up+f->ascii[x]->down)/2,f->ascii[x]->down-(f->ascii[x]->up+f->ascii[x]->down)/2);
    FreePicar(mem);
    //SaveArray(f->test[s]->z->a,f->test[s]->z->h,f->test[s]->z->w,"C:\\feture8.bmp");

    //Feature9 ฝั่งซ้ายของ 'ผ'
    x=188;
    s=9;
    mem=GetInBound(f->ascii[x],f->ascii[x]->up,f->ascii[x]->down+1,0,f->ascii[x]->z->w/2);
    //SavePicar(mem,"C:\\feature00.bmp");
    GetFeature(f->test[s],mem,0,f->h);
    FreePicar(mem);
    //SaveArray(f->test[s]->z->a,f->test[s]->z->h,f->test[s]->z->w,"C:\\feture9.bmp");
    //จบ การดึง feature

    //save รูปที่ load ออกมาดู
    for(i=0;i<all;i++)
    {
        if(f->have[i]==1)
        {
            SavePicar(f->ascii[i]->z,"C:\\new\\"+IntToStr(i)+".bmp");
        }
    }
}

myfont *LoadFont(AnsiString nameinput,bool bold,bool italic,int high,TImage *image)
{
    //group of level of data
    int l1[1][2]={{216,217}};
    int l2[4][2]={{161,208},{210,210},{46,46},{224,230}}; //240-249=เลขไทย//46=dot
    int l3[3][2]={{209,209},{212,215},{231,237}};
    int l4[1][2]={{232,236}};
    int l[5]={0,1,4,3,1};
    int s,count;
    int i,j,k,up,down,sum,x,ini,inj,m,memh;  //memh เอาไว้จำ Height
                                           //m อัตราเร็วในการหาขนาด font
    picar *z,*mem;
    //////////////////////////FindSize & find up down


    up=5;
    down=5;
    memh=high;

    while(down-up+1!=high)
    {
        image->Picture=NULL;

        //ต้องใส่ข้อมูลใหม่ให้มันเพราะมันลืมหลัง NULL รูป

        image->Canvas->Font->Name=nameinput;

        m=abs((down-up+1)-high);
        /*
        if(m<=5)                  ////////////////แก้ตรงนี้ได้
        {
            m=1;
        }
        */

        if(down-up+1>high)
        {
            memh-=m;
            image->Canvas->Font->Height=memh;
        }
        else if(down-up+1<high)
        {
            memh+=m;
            image->Canvas->Font->Height=memh;
        }

        if(bold&&italic==true)
            image->Canvas->Font->Style=TFontStyles()<< fsBold << fsItalic ;
        else if(bold)
            image->Canvas->Font->Style=TFontStyles()<< fsBold ;
        else if(italic)
            image->Canvas->Font->Style=TFontStyles()<< fsItalic ;
        else if(bold||italic==false)
            image->Canvas->Font->Style=TFontStyles();

        //เขียนตัวอักษรที่ดูด้านบน
        image->Canvas->TextOutA(0,5,"ด");
        sum=0;
        i=0;
        while(sum==0)
        {
            sum=0;
            for(j=0;j<image->Width;j++)
            {
                if(image->Canvas->Pixels[j][i]!=clWhite)
                {
                    sum++;
                    break;
                }
            }
            i++;
        }
        up=i-1;

        image->Picture=NULL;

        //ต้องใส่ข้อมูลใหม่ให้มันเพราะมันลืมหลัง NULL รูป
        image->Canvas->Font->Name=nameinput;

        image->Canvas->Font->Height=memh;

        if(bold&&italic==true)
            image->Canvas->Font->Style=TFontStyles()<< fsBold << fsItalic ;
        else if(bold)
            image->Canvas->Font->Style=TFontStyles()<< fsBold ;
        else if(italic)
            image->Canvas->Font->Style=TFontStyles()<< fsItalic ;
        else if(bold||italic==false)
            image->Canvas->Font->Style=TFontStyles();

        //เขียนตัวอักษรที่ดูด้านล่าง
        image->Canvas->TextOutA(0,5,"ธ");

        sum=0;
        i=image->Height-1;
        while(sum==0)
        {
            sum=0;
            for(j=0;j<image->Width;j++)
            {
                if(image->Canvas->Pixels[j][i]!=clWhite)
                {
                    sum++;
                    break;
                }
            }
            i--;
        }
        down=i+1;
    }
    // ให้ size เพิ่มขึ้นไปให้สุด (เพราะความสูงหนึ่งเป็นไปได้หลายขนาด)

    while(down-up+1==high)
    {

        image->Picture=NULL;

        //ต้องใส่ข้อมูลใหม่ให้มันเพราะมันลืมหลัง NULL รูป

        image->Canvas->Font->Name=nameinput;


        memh++;
        image->Canvas->Font->Height=memh;


        if(bold&&italic==true)
            image->Canvas->Font->Style=TFontStyles()<< fsBold << fsItalic ;
        else if(bold)
            image->Canvas->Font->Style=TFontStyles()<< fsBold ;
        else if(italic)
            image->Canvas->Font->Style=TFontStyles()<< fsItalic ;
        else if(bold||italic==false)
            image->Canvas->Font->Style=TFontStyles();

        //เขียนตัวอักษรที่ดูด้านบน
        image->Canvas->TextOutA(0,5,"ด");
        sum=0;
        i=0;
        while(sum==0)
        {
            sum=0;
            for(j=0;j<image->Width;j++)
            {
                if(image->Canvas->Pixels[j][i]!=clWhite)
                {
                    sum++;
                    break;
                }
            }
            i++;
        }
        up=i-1;

        image->Picture=NULL;

        //ต้องใส่ข้อมูลใหม่ให้มันเพราะมันลืมหลัง NULL รูป
        image->Canvas->Font->Name=nameinput;

        image->Canvas->Font->Height=memh;

        if(bold&&italic==true)
            image->Canvas->Font->Style=TFontStyles()<< fsBold << fsItalic ;
        else if(bold)
            image->Canvas->Font->Style=TFontStyles()<< fsBold ;
        else if(italic)
            image->Canvas->Font->Style=TFontStyles()<< fsItalic ;
        else if(bold||italic==false)
            image->Canvas->Font->Style=TFontStyles();

        //เขียนตัวอักษรที่ดูด้านล่าง
        image->Canvas->TextOutA(0,5,"ธ");

        sum=0;
        i=image->Height-1;
        while(sum==0)
        {
            sum=0;
            for(j=0;j<image->Width;j++)
            {
                if(image->Canvas->Pixels[j][i]!=clWhite)
                {
                    sum++;
                    break;
                }
            }
            i--;
        }
        down=i+1;
    }
    memh--;
    ////end find size & find up down  //ได้ memh เป็น size มาใช้
    Form1->ProgressBar2->StepBy(10);     //คุง
    Form1->ProgressBar2->Refresh();
    ///allocate memory
    myfont *f;
    f=(myfont *)malloc(sizeof(myfont));
    f->bold=bold;
    f->italic=italic;
    //f->fontname=nameinput;  // มีปัญหาเรื่อง memory ไม่ใช้ก็ได้
    f->size=image->Canvas->Font->Height;
    f->h=down-up+1;

    for(i=0;i<all;i++)
    {
        f->have[i]=0;
    }

    char c[4];

    //L1 load ***********************************************************
    c[0]='ธ';
    c[2]='\0';
    for(s=0;s<l[1];s++)
    {
        for(x=l1[s][0];x<=l1[s][1];x++)
        {
            f->have[x]=1;
            f->ascii[x]=(alpha *)malloc(sizeof(alpha));

            image->Picture=NULL;
            //set ค่า font
            image->Canvas->Font->Name=nameinput;

            image->Canvas->Font->Height=memh;

            if(bold&&italic==true)
                image->Canvas->Font->Style=TFontStyles()<< fsBold << fsItalic ;
            else if(bold)
                image->Canvas->Font->Style=TFontStyles()<< fsBold ;
            else if(italic)
                image->Canvas->Font->Style=TFontStyles()<< fsItalic ;
            else if(bold||italic==false)
                image->Canvas->Font->Style=TFontStyles();
            //เขียนลงไป
            c[1]=x;
            image->Canvas->TextOutA(0,5,c);
            i=image->Height-1;
            sum=0;
            while(sum==0)
            {
               for(j=0;j<image->Width-1;j++)
               {
                    if(image->Canvas->Pixels[j][i]!=clWhite)
                    {
                        sum++;
                        break;
                    }
               }
               i--;
            }
            ini=i+2;
            sum=0;
            j=image->Width-1;
            while(sum==0)
            {
               for(i=0;i<image->Height-1;i++)
               {
                    if(image->Canvas->Pixels[j][i]!=clWhite)
                    {
                        sum++;
                        break;
                    }
               }
               j--;
            }
            inj=j+2;

            z=NewPicar(ini,inj,1);

            for(i=0;i<ini;i++)
            {
                for(j=0;j<inj;j++)
                {
                    if(image->Canvas->Pixels[j][i]!=clWhite) //เป็น clBlack ไปเลยไม่ได้
                        z->a[i][j]=1;
                    else
                        z->a[i][j]=0;
                }
            }
            for(i=down+1;i<ini;i++)
            {
                for(j=0;j<inj;j++)
                {
                    if(z->a[i][j]==1)
                    {
                        f->ascii[x]=SuckFont(z,i,j,up,down); //SuckFontได้ขอพื้นที่ให้ picar แล้ว
                        f->ascii[x]->type=1;
                        f->ascii[x]->c[0]='อ';
                        f->ascii[x]->c[1]=char(x);
                        f->ascii[x]->c[2]='\0';
                        i=ini;      //เพื่อให้หลุดจาก loop
                        j=inj;
                    }
                }
            }
            FreePicar(z);
            //SaveArray(f->ascii[x]->a,f->ascii[x]->h,f->ascii[x]->w,"C:\\"+IntToStr(x)+".bmp");
        }
    }
    //finish L1 load
    Form1->ProgressBar2->StepBy(2);     //คุง
    Form1->ProgressBar2->Refresh();
    //L2 load ***********************************************************
    int ini2,inj2;
    c[0]='อ';
    c[2]='\0';
    for(s=0;s<l[2];s++)
    {
        for(x=l2[s][0];x<=l2[s][1];x++)
        {
            f->have[x]=1;
            f->ascii[x]=(alpha *)malloc(sizeof(alpha));

            image->Picture=NULL;
            //set ค่า font
            image->Canvas->Font->Name=nameinput;

            image->Canvas->Font->Height=memh;

            if(bold&&italic==true)
                image->Canvas->Font->Style=TFontStyles()<< fsBold << fsItalic ;
            else if(bold)
                image->Canvas->Font->Style=TFontStyles()<< fsBold ;
            else if(italic)
                image->Canvas->Font->Style=TFontStyles()<< fsItalic ;
            else if(bold||italic==false)
                image->Canvas->Font->Style=TFontStyles();
            //เขียนลงไป
            c[1]=x;
            image->Canvas->TextOutA(0,5,char(x));

            //หา ini และ inj
            i=image->Height-1;
            sum=0;
            while(sum==0)
            {
               for(j=0;j<image->Width-1;j++)
               {
                    if(image->Canvas->Pixels[j][i]!=clWhite)
                    {
                        sum++;
                        break;
                    }
               }
               i--;
            }
            ini=i+2;
            sum=0;
            j=image->Width-1;
            while(sum==0)
            {
               for(i=0;i<image->Height-1;i++)
               {
                    if(image->Canvas->Pixels[j][i]!=clWhite)
                    {
                        sum++;
                        break;
                    }
               }
               j--;
            }
            inj=j+2;
            //หา ini2 และ inj2
            sum=0;
            i=0;
            while(sum==0)
            {
                for(j=0;j<inj;j++)
                {
                    if(image->Canvas->Pixels[j][i]!=clWhite)
                    {
                        sum++;
                        break;
                    }
                }
                i++;
            }
            ini2=i-1;
            sum=0;
            j=0;
            while(sum==0)
            {
                for(i=0;i<ini;i++)
                {
                    if(image->Canvas->Pixels[j][i]!=clWhite)
                    {
                        sum++;
                        break;
                    }
                }
                j++;
            }
            inj2=j-1;
            //หา ini2 และ inj2 เสร็จแล้ว
            /*
            f->ascii[x]->z->w=inj-inj2;
            f->ascii[x]->z->h=ini-ini2;
            f->ascii[x]->z->a=NewArray(ini-ini2,inj-inj2);
            */
            f->ascii[x]->z=NewPicar(ini-ini2,inj-inj2,1);
            f->ascii[x]->up=up-ini2;
            f->ascii[x]->down=down-ini2;

            
            for(i=ini2;i<ini;i++)
            {
                for(j=inj2;j<inj;j++)
                {
                    if(image->Canvas->Pixels[j][i]!=clWhite)
                    {
                        f->ascii[x]->z->a[i-ini2][j-inj2]=1;
                    }
                    else
                    {
                        f->ascii[x]->z->a[i-ini2][j-inj2]=0;
                    }
                }
            }

            f->ascii[x]->type=2;
            f->ascii[x]->c[0]='อ';
            f->ascii[x]->c[1]=char(x);
            f->ascii[x]->c[2]='\0';

            //SaveArray(f->ascii[x]->a,f->ascii[x]->h,f->ascii[x]->w,"C:\\"+IntToStr(x)+".bmp");
            Form1->ProgressBar2->StepBy(1);     //คุง
            Form1->ProgressBar2->Refresh();
        }
    }
    //finish L2 load

    //L3 load ***********************************************************
    c[0]='ด';
    c[2]='\0';
    for(s=0;s<l[3];s++)
    {
        for(x=l3[s][0];x<=l3[s][1];x++)
        {
            f->have[x]=1;
            f->ascii[x]=(alpha *)malloc(sizeof(alpha));

            image->Picture=NULL;
            //set ค่า font
            image->Canvas->Font->Name=nameinput;

            image->Canvas->Font->Height=memh;

            if(bold&&italic==true)
                image->Canvas->Font->Style=TFontStyles()<< fsBold << fsItalic ;
            else if(bold)
                image->Canvas->Font->Style=TFontStyles()<< fsBold ;
            else if(italic)
                image->Canvas->Font->Style=TFontStyles()<< fsItalic ;
            else if(bold||italic==false)
                image->Canvas->Font->Style=TFontStyles();
            //เขียนลงไป
            c[1]=x;
            image->Canvas->TextOutA(0,5,c);

            //เอาตัวที่อยู่ข้างล่างออกไป
            i=(up+down)/2;
            for(j=0;j<image->Width;j++)
            {
                if(image->Canvas->Pixels[j][i]!=clWhite)
                {
                    ClearImage(image,i,j,up,down,0,image->Width-1);
                }
            }

            //หา ini และ inj
            i=image->Height-1;
            sum=0;
            while(sum==0)
            {
               for(j=0;j<image->Width-1;j++)
               {
                    if(image->Canvas->Pixels[j][i]!=clWhite)
                    {
                        sum++;
                        break;
                    }
               }
               i--;
            }
            ini=i+2;
            sum=0;
            j=image->Width-1;
            while(sum==0)
            {
               for(i=0;i<image->Height-1;i++)
               {
                    if(image->Canvas->Pixels[j][i]!=clWhite)
                    {
                        sum++;
                        break;
                    }
               }
               j--;
            }
            inj=j+2;
            //หา ini2 และ inj2
            sum=0;
            i=0;
            while(sum==0)
            {
                for(j=0;j<inj;j++)
                {
                    if(image->Canvas->Pixels[j][i]!=clWhite)
                    {
                        sum++;
                        break;
                    }
                }
                i++;
            }
            ini2=i-1;
            sum=0;
            j=0;
            while(sum==0)
            {
                for(i=0;i<ini;i++)
                {
                    if(image->Canvas->Pixels[j][i]!=clWhite)
                    {
                        sum++;
                        break;
                    }
                }
                j++;
            }
            inj2=j-1;
            //หา ini2 และ inj2 เสร็จแล้ว
            /*
            f->ascii[x]->z->w=inj-inj2;
            f->ascii[x]->z->h=ini-ini2;
            f->ascii[x]->z->a=NewArray(ini-ini2,inj-inj2);
            */
            f->ascii[x]->z=NewPicar(ini-ini2,inj-inj2,1);
            f->ascii[x]->up=up-ini2;
            f->ascii[x]->down=down-ini2;

            
            for(i=ini2;i<ini;i++)
            {
                for(j=inj2;j<inj;j++)
                {
                    if(image->Canvas->Pixels[j][i]!=clWhite)
                    {
                        f->ascii[x]->z->a[i-ini2][j-inj2]=1;
                    }
                    else
                    {
                        f->ascii[x]->z->a[i-ini2][j-inj2]=0;
                    }
                }
            }

            f->ascii[x]->type=2;
            f->ascii[x]->c[0]='อ';
            f->ascii[x]->c[1]=char(x);
            f->ascii[x]->c[2]='\0';
            Form1->ProgressBar2->StepBy(1);     //คุง
            Form1->ProgressBar2->Refresh();
            //SaveArray(f->ascii[x]->a,f->ascii[x]->h,f->ascii[x]->w,"C:\\"+IntToStr(x)+".bmp");
        }
    }
    //finish L3 load

    //L4 load **** ต้องไปอยู่นอก 255 เพราะอยู่สูง ****************
    c[0]='ด';
    c[1]='ํ';
    c[3]='\0';
    //หาระดับการตัดก่อน
    c[2]='\0';
    image->Canvas->TextOutA(0,5,c);

    sum=0;
    i=0;
    while(sum==0)
    {
        for(j=0;j<inj;j++)
        {
            if(image->Canvas->Pixels[j][i]!=clWhite)
            {
                sum++;
                break;
            }
        }
        i++;
    }
    int upper=i-1;

    for(s=0;s<l[4];s++)
    {
        for(x=l4[s][0];x<=l4[s][1];x++)
        {
            f->have[x+24]=1;
            f->ascii[x+24]=(alpha *)malloc(sizeof(alpha));

            image->Picture=NULL;
            //set ค่า font
            image->Canvas->Font->Name=nameinput;

            image->Canvas->Font->Height=memh;

            if(bold&&italic==true)
                image->Canvas->Font->Style=TFontStyles()<< fsBold << fsItalic ;
            else if(bold)
                image->Canvas->Font->Style=TFontStyles()<< fsBold ;
            else if(italic)
                image->Canvas->Font->Style=TFontStyles()<< fsItalic ;
            else if(bold||italic==false)
                image->Canvas->Font->Style=TFontStyles();


            //เขียนของจริงลงไป
            c[2]=x;
            image->Canvas->TextOutA(0,5,c);

            //เอาตัวที่อยู่ข้างล่างออกไป สองระดับ
            i=(up+down)/2;
            for(j=0;j<image->Width;j++)
            {
                if(image->Canvas->Pixels[j][i]!=clWhite)
                {
                    ClearImage(image,i,j,up,down,0,image->Width-1);
                }
            }
            for(i=up;i>upper;i--)
            {
                for(j=0;j<image->Width;j++)
                {
                    if(image->Canvas->Pixels[j][i]!=clWhite)
                    {
                        ClearImage(image,i,j,upper,up,0,image->Width-1);
                    }
                }
            }
            //หา ini และ inj
            i=image->Height-1;
            sum=0;
            while(sum==0)
            {
               for(j=0;j<image->Width-1;j++)
               {
                    if(image->Canvas->Pixels[j][i]!=clWhite)
                    {
                        sum++;
                        break;
                    }
               }
               i--;
            }
            ini=i+2;
            sum=0;
            j=image->Width-1;
            while(sum==0)
            {
               for(i=0;i<image->Height-1;i++)
               {
                    if(image->Canvas->Pixels[j][i]!=clWhite)
                    {
                        sum++;
                        break;
                    }
               }
               j--;
            }
            inj=j+2;
            //หา ini2 และ inj2
            sum=0;
            i=0;
            while(sum==0)
            {
                for(j=0;j<inj;j++)
                {
                    if(image->Canvas->Pixels[j][i]!=clWhite)
                    {
                        sum++;
                        break;
                    }
                }
                i++;
            }
            ini2=i-1;
            sum=0;
            j=0;
            while(sum==0)
            {
                for(i=0;i<ini;i++)
                {
                    if(image->Canvas->Pixels[j][i]!=clWhite)
                    {
                        sum++;
                        break;
                    }
                }
                j++;
            }
            inj2=j-1;
            //หา ini2 และ inj2 เสร็จแล้ว
            /*
            f->ascii[x+24]->z->w=inj-inj2;
            f->ascii[x+24]->z->h=ini-ini2;
            f->ascii[x+24]->z->a=NewArray(ini-ini2,inj-inj2);
            */
            f->ascii[x+24]->z=NewPicar(ini-ini2,inj-inj2,1);
            f->ascii[x+24]->up=up-ini2;
            f->ascii[x+24]->down=down-ini2;

            
            for(i=ini2;i<ini;i++)
            {
                for(j=inj2;j<inj;j++)
                {
                    if(image->Canvas->Pixels[j][i]!=clWhite)
                    {
                        f->ascii[x+24]->z->a[i-ini2][j-inj2]=1;
                    }
                    else
                    {
                        f->ascii[x+24]->z->a[i-ini2][j-inj2]=0;
                    }
                }
            }

            f->ascii[x+24]->type=2;
            f->ascii[x+24]->c[0]='อ';
            f->ascii[x+24]->c[1]=char(x);
            f->ascii[x+24]->c[2]='\0';

            Form1->ProgressBar2->StepBy(1);     //คุง
            Form1->ProgressBar2->Refresh();
            //SaveArray(f->ascii[x+24]->z->a,f->ascii[x+24]->z->h,f->ascii[x+24]->w,"C:\\"+IntToStr(x+24)+".bmp");
        }
    }
    //finish L4 load

    //หา gap (เว้นวรรค) โดยใช้ dot (46)

    image->Picture=NULL;
    //set ค่า font
    image->Canvas->Font->Name=nameinput;

    image->Canvas->Font->Height=memh;

    if(bold&&italic==true)
        image->Canvas->Font->Style=TFontStyles()<< fsBold << fsItalic ;
    else if(bold)
        image->Canvas->Font->Style=TFontStyles()<< fsBold ;
    else if(italic)
        image->Canvas->Font->Style=TFontStyles()<< fsItalic ;
    else if(bold||italic==false)
        image->Canvas->Font->Style=TFontStyles();

    i=up-f->ascii[46]->up;
    //พิมพ์ ".." แล้วหาระยะทาง
    image->Canvas->TextOutA(0,5,"..");

    count=0;
    for(j=0;j<image->Width-1;j++)
    {
        if(count==0
        && image->Canvas->Pixels[j][i]!=clWhite
        && image->Canvas->Pixels[j+1][i]==clWhite)
        {
            count=1;
            s=j;  //start
        }
        else if(count==1
        && image->Canvas->Pixels[j][i]==clWhite
        && image->Canvas->Pixels[j+1][i]!=clWhite)
        {
            count=0;
            f->gap=j-s;    //ยังไม่ใช่ของจริง นี่คือ gap ระหว่างจุด
            break;
        }
    }
    //พิมพ์  ". ." หาระยะทาง
    image->Canvas->TextOutA(0,5,". .");
    count=0;
    for(j=0;j<image->Width-1;j++)
    {
        if(count==0
        && image->Canvas->Pixels[j][i]!=clWhite
        && image->Canvas->Pixels[j+1][i]==clWhite)
        {
            count=1;
            s=j;  //start
        }
        else if(count==1
        && image->Canvas->Pixels[j][i]==clWhite
        && image->Canvas->Pixels[j+1][i]!=clWhite)
        {
            f->gap=(j-s)-f->gap;    //คำตอบจริง
            break;
        }
    }
    //finish find gap
    Form1->ProgressBar2->StepBy(1);     //คุง
    Form1->ProgressBar2->Refresh();
    //load ตัวพิเศษ 8 ตัว "ญ ฐ ะ แ"="173,176,208,225" ดึงจากที่เก็บไว้ได้เลย
    for(k=0;k<4;k++)
    {
        count=0;
        if(k==0)     //ญ
        {
            x=173;
            s=261;
        }
        else if(k==1)  //ฐ
        {
            x=176;
            s=263;
        }
        else if(k==2)  //ะ
        {
            x=208;
            s=265;
        }
        else if(k==3) //แ
        {
            x=225;
            s=267;
        }

        z=NewPicar(f->ascii[x]->z->h,f->ascii[x]->z->w,1);
        for(i=0;i<f->ascii[x]->z->h;i++)
        {
            for(j=0;j<f->ascii[x]->z->w;j++)
            {
                if(f->ascii[x]->z->a[i][j]==1)
                {
                    z->a[i][j]=1;
                }
                else
                {
                    z->a[i][j]=0;
                }
            }
        }
        for(i=0;i<f->ascii[x]->z->h;i++)
        {
            for(j=0;j<f->ascii[x]->z->w;j++)
            {
                if(z->a[i][j]==1)
                {
                    if(count==0)
                    {
                        f->ascii[s]=SuckFont(z,i,j,f->ascii[x]->up,f->ascii[x]->down);
                        count=1;
                    }
                    else if(count==1)
                    {
                        f->ascii[s+1]=SuckFont(z,i,j,f->ascii[x]->up,f->ascii[x]->down);
                        count=2;
                    }
                }
            }
        }
        if(count==1)
        {
            FreePicar(f->ascii[s]->z);
            //FreePicArray((void **)(f->ascii[s]->z->a),f->ascii[s]->z->h);
        }
        else if(count==2)
        {
            f->have[s]=1;
            f->have[s+1]=1;
            f->ascii[s]->c[0]='อ';
            f->ascii[s]->c[1]=x;
            f->ascii[s]->c[2]='\0';
            f->ascii[s+1]->c[0]='อ';
            f->ascii[s+1]->c[1]=x;
            f->ascii[s+1]->c[2]='\0';
        }
    }
    Form1->ProgressBar2->StepBy(8);     //คุง
    Form1->ProgressBar2->Refresh();
    /*
    for(x=261;x<=266;x++)
    {
        SaveArray(f->ascii[x]->z->a,f->ascii[x]->z->h,f->ascii[x]->w,"C:\\"+IntToStr(x)+".bmp");
    }
    */
    //ดึงตัวพิเศษ เสร็จแล้ว



    ///ดึงลักษณะเด่นออกจาก font ที่ load มา
//    LoadFeature(f); //////////ฝฝ/////////ใช้ใน HD เท่านั้น

    //ทำ skeleton+thining ทุกตัวเก็บไว้
    //แต่จะทำกับตัวที่มีเพียงกลุ่มเดียวเท่านั้น
    picar *sk;
    for(i=0;i<all;i++)
    {

        if(f->have[i]==1)
        {
            f->have[i]=CountGroup(f->ascii[i]->z);
            if(f->have[i]==1)
            {
                f->ascii[i]->thin=(thindata *)malloc(sizeof(thindata));

                sk=skeleton(f->ascii[i]->z);
                f->ascii[i]->thin->z=thining(sk);
                FreePicar(sk);

                //f->ascii[i]->thin->z=thining(f->ascii[i]->z);

                PreChain2(f->ascii[i]->thin);
                if(f->ascii[i]->thin->node_num>0)
                {
                    FindLongestMain2(f->ascii[i]->thin);
                }
            }
        }
    }
    Form1->ProgressBar2->StepBy(5);     //คุง
    Form1->ProgressBar2->Refresh();
    return f;
}
void FreeFont(myfont *f)
{
    int i;
    for(i=0;i<261;i++)
    {
        if(f->have[i]>=1)
        {
            FreePicar(f->ascii[i]->z);
            if(f->have[i]==1)
            {
                FreeThindata(f->ascii[i]->thin);
            }
            free(f->ascii[i]);
        }
    }
    /*  //จะใช้กับ HD เท่านั้น
    for(i=0;i<10;i++)
    {
        FreePicar(f->test[i]->z);
        free(f->test[i]);
    }
    free(f->test);
    */
    free(f);
}
float compare(alpha *n,myfont *f,int testnum)
{
    int i,j=0,k,l,sum=0,ai,aj,d,max,point=0;
    bool end;
    if(testnum==0||testnum==3||testnum==4||testnum==5) //ก บ ข ฃ
    {
        while(sum==0 && j<n->z->w)
        {
            for(i=n->up;i<(n->up+n->down)/2;i++)
            {
                if(i>=0 && i<n->z->h && n->z->a[i][j]==1)
                {
                    sum++;
                    break;
                }
            }
            j++;
        }
        aj=j-1;
        ai=n->up-f->test[testnum]->up;
    }
    else if(testnum==1||testnum==2) // ถ ภ
    {
        while(sum==0 && j<n->z->w)
        {
            for(i=(n->down+n->up)/2+1;i<=n->down;i++)
            {
                if(i>=0 && i<n->z->h && n->z->a[i][j]==1)
                {
                    sum++;
                    break;
                }
            }
            j++;
        }
        aj=j-1;
        ai=n->up-f->test[testnum]->up;
    }
    else if(testnum==6||testnum==7||testnum==8||testnum==9) // ด ค ล ผ
    {
        aj=f->test[testnum]->left;
        ai=n->up-f->test[testnum]->up;
    }
    sum=0;
    max=0;
    for(i=0;i<f->test[testnum]->z->h;i++)
    {
        for(j=0;j<f->test[testnum]->z->w;j++)
        {
            if(f->test[testnum]->z->a[i][j]==1)
            {
                point++;
                d=0;
                end=false;
                while(i+ai-d>=0 || i+ai+d<n->z->h || j+aj-d>=0 || j+aj+d<n->z->w)
                {
                    for(k=0-d;k<=d;k++)
                    {
                        for(l=0-d;l<=d;l++)
                        {
                            if(k==0-d || k==d || l==0-d || l==d)
                            {
                                if(i+ai+k >=0 && i+ai+k <n->z->h && j+aj+l >=0 && j+aj+l <n->z->w)
                                {
                                    if(n->z->a[i+ai+k][j+aj+l]==1)
                                    {
                                        sum+=d;
                                        if(d>max)
                                        {
                                            max=d;
                                        }
                                        end=true;
                                        break;
                                    }
                                }
                            }
                        }
                        if(end)
                            break;
                    }
                    if(end)
                        break;
                    d++;
                }
            }
        }
    }
    if(point==0)
    {
        ShowMessage("Error in detect feature from font (testnum="+IntToStr(testnum)+")");
        point=1;
        sum=-1;
    }
    //n->f[testnum]= float(sum)/float(point);   //ไม่รู้จะ return อะไร ระหว่าง d เฉลี่ย กับ max
    //n->f[testnum]= max;
    return float(sum)/float(point);
}
float distance(alpha *m,alpha *n)      //m ทาบ n
{
    int ai,aj,i,j,k,l,d,sum,max,point,end;

    ai=n->up-m->up;
    aj=0;

    point=0;
    sum=0;
    max=0;
    for(i=0;i<m->z->h;i++)
    {
        for(j=0;j<m->z->w;j++)
        {
            if(m->z->a[i][j]==1)
            {
                point++;
                d=0;
                end=false;
                while(i+ai-d>=0 || i+ai+d<n->z->h || j+aj-d>=0 || j+aj+d<n->z->w)
                {
                    for(k=0-d;k<=d;k++)
                    {
                        for(l=0-d;l<=d;l++)
                        {
                            if(k==0-d || k==d || l==0-d || l==d)
                            {
                                if(i+ai+k >=0 && i+ai+k <n->z->h && j+aj+l >=0 && j+aj+l <n->z->w)
                                {
                                    if(n->z->a[i+ai+k][j+aj+l]==1)
                                    {
                                        sum+=d;
                                        if(d>max)
                                        {
                                            max=d;
                                        }
                                        end=true;
                                        break;
                                    }
                                }
                            }
                        }
                        if(end)
                            break;
                    }
                    if(end)
                        break;
                    d++;
                }
            }
        }
    }
    return float(sum)/float(point);
}
float distance2(alpha *m,alpha *n)      //ทำทั้งสองอันแล้วค่อยเฉลี่ย
{
    int ai,aj,i,j,k,l,d,sum,max,point,end;

    ai=n->up-m->up;
    aj=0;
    //debug path
    int bug1[100][100],bug2[100][100];
    for(i=0;i<=m->z->h;i++)
    {
        for(j=0;j<=m->z->w;j++)
        {
            if(i==m->z->h || j==m->z->w)
            {
                bug1[i][j]=-1;
            }
            else if(m->z->a[i][j]==1)
            {
                bug1[i][j]=8;
            }
            else
                bug1[i][j]=m->z->a[i][j];
        }
    }
    for(i=0;i<=n->z->h;i++)
    {
        for(j=0;j<=n->z->w;j++)
        {
            if(i==n->z->h || j==n->z->w)
            {
                bug2[i][j]=-1;
            }
            else if(n->z->a[i][j]==1)
            {
                bug2[i][j]=8;
            }
            else
                bug2[i][j]=n->z->a[i][j];
        }
    }
    if(ai>=0 && aj>=0)
    {
        bug2[ai][aj]=2;
    }
    //end debug
    point=0;
    sum=0;
    max=0;
    for(i=0;i<m->z->h;i++)
    {
        for(j=0;j<m->z->w;j++)
        {
            if(m->z->a[i][j]==1)
            {
                point++;
                d=0;
                end=false;
                while(i+ai-d>=0 || i+ai+d<n->z->h || j+aj-d>=0 || j+aj+d<n->z->w)
                {
                    for(k=0-d;k<=d;k++)
                    {
                        for(l=0-d;l<=d;l++)
                        {
                            if(k==0-d || k==d || l==0-d || l==d)
                            {
                                if(i+ai+k >=0 && i+ai+k <n->z->h && j+aj+l >=0 && j+aj+l <n->z->w)
                                {
                                    if(n->z->a[i+ai+k][j+aj+l]==1)
                                    {
                                        sum+=d;
                                        if(d>max)
                                        {
                                            max=d;
                                        }
                                        end=true;
                                        break;
                                    }
                                }
                            }
                        }
                        if(end)
                            break;
                    }
                    if(end)
                        break;
                    d++;
                }
            }
        }
    }

    ai=m->up-n->up;
    //debug
    if(ai>=0 && aj>=0)
    {
        bug1[ai][aj]=2;
    }
    //end debug
    for(i=0;i<n->z->h;i++)
    {
        for(j=0;j<n->z->w;j++)
        {
            if(n->z->a[i][j]==1)
            {
                point++;
                d=0;
                end=false;
                while(i+ai-d>=0 || i+ai+d<m->z->h || j+aj-d>=0 || j+aj+d<m->z->w)
                {
                    for(k=0-d;k<=d;k++)
                    {
                        for(l=0-d;l<=d;l++)
                        {
                            if(k==0-d || k==d || l==0-d || l==d)
                            {
                                if(i+ai+k >=0 && i+ai+k <m->z->h && j+aj+l >=0 && j+aj+l <m->z->w)
                                {
                                    if(m->z->a[i+ai+k][j+aj+l]==1)
                                    {
                                        sum+=d;
                                        if(d>max)
                                        {
                                            max=d;
                                        }
                                        end=true;
                                        break;
                                    }
                                }
                            }
                        }
                        if(end)
                            break;
                    }
                    if(end)
                        break;
                    d++;
                }
            }
        }
    }
    return float(sum)/float(point);
}
int CompareSize1(int *point,alpha *n,myfont *f)
{
    //เปรียบเทียบทั้ง ความกว้าง, ความยาว, ตำแหน่ง,
    int i,t=f->h*15/100,count=0;
    if(t<1)
    {
        t=1;
    }
    for(i=0;i<all;i++)
    {
        if(point[i]==1 &&f->have[i]>0)
        {
            if  (
                  abs(f->ascii[i]->up - n->up)<=t         //ปรับค่าตรงนี้ให้เหมาะสมด้วย
                &&abs(f->ascii[i]->z->h  - n->z->h )<=t
                &&abs(f->ascii[i]->z->w  - n->z->w )<=t
                )
            {
                   count++;
            }
            else
            {
                point[i]=0;
            }
        }
        else
        {
            point[i]=0;  //ห้ามลบ มันถูกอยู่แล้ว
        }
    }
    return count;
}
int CompareSize2(int *point,alpha *n,myfont *f)
{
    //เปรียบเทียบทั้ง
    //จำนวน group, loop(ได้ผลไม่ดี), สายโซ่ที่ยาวที่สุด ,จุดต้นสาย,จุดปลายสาย
    int i,t=f->h*15/100+1,count=0,mem[9];
    int ai1,aj1,ai2,aj2,bi1,bj1,bi2,bj2;    //a ต้น b ปลาย //1 input 2 ต้นแบบ
    for(i=0;i<all;i++)
    {
        if(point[i]==1)
        {
            if(f->have[i]==1)
            {
                if(f->ascii[i]->thin->node_num>0)
                {
                    ai1=n          ->thin->node_pi[n          ->thin->path[0]] - n          ->up;
                    ai2=f->ascii[i]->thin->node_pi[f->ascii[i]->thin->path[0]] - f->ascii[i]->up;
                    aj1=n          ->thin->node_pj[n          ->thin->path[0]];
                    aj2=f->ascii[i]->thin->node_pj[f->ascii[i]->thin->path[0]];

                    bi1=n          ->thin->node_pi[n          ->thin->path[n          ->thin->path_num-1]] - n          ->up;
                    bi2=f->ascii[i]->thin->node_pi[f->ascii[i]->thin->path[f->ascii[i]->thin->path_num-1]] - f->ascii[i]->up;
                    bj1=n          ->thin->node_pj[n          ->thin->path[n          ->thin->path_num-1]];
                    bj2=f->ascii[i]->thin->node_pj[f->ascii[i]->thin->path[f->ascii[i]->thin->path_num-1]];

                    if(
                        //n->thin->loop_num ==f->ascii[i]->thin->loop_num ไม่ควรใช้ ได้ผลไม่ดี
                        abs(n->thin->path_long - f->ascii[i]->thin->path_long)<=(f->ascii[i]->thin->path_long*40/100+1) // +- กี่ % ของสายต้นแบบดี
                        && sqrt( (ai1-ai2)*(ai1-ai2) + (aj1-aj2)*(aj1-aj2) )  <= f->h*25/100+1             //จุดแก้ไขที่สำคัญมาก  คุงแก้เป็น 15 จากเดิม 25
                        && sqrt( (bi1-bi2)*(bi1-bi2) + (bj1-bj2)*(bj1-bj2) )  <= f->h*25/100+1             //แต่เราคิดว่ามากๆไว้ดีที่สุด
                      //&&abs((n->thin->node_pi[n->thin->path[0]]-n->up) - (f->ascii[i]->thin->node_pi[f->ascii[i]->thin->path[0]]-f->ascii[i]->up)) <= t+f->h*15/100
                      //&&abs(n->thin->node_pj[n->thin->path[0]] - f->ascii[i]->thin->node_pj[f->ascii[i]->thin->path[0]]) <= t+f->h*15/100
                      //&&abs((n->thin->node_pi[n->thin->path[n->thin->path_num-1]]-n->up) - (f->ascii[i]->thin->node_pi[f->ascii[i]->thin->path[f->ascii[i]->thin->path_num-1]]-f->ascii[i]->up)) <= t+f->h*15/100
                      //&&abs (n->thin->node_pj[n->thin->path[n->thin->path_num-1]]        -  f->ascii[i]->thin->node_pj[f->ascii[i]->thin->path[f->ascii[i]->thin->path_num-1]]                 ) <= t+f->h*15/100
                      )
                    {
                       count++;
                    }
                    else
                    {
                       point[i]=0;
                    }
                }
                else
                {
                    point[i]=0;
                }
            }
            else
            {
                point[i]=0;
            }
        }
    }
    return count;
}
/* Debug
            if(i==232 && n->type==3)
            {
                i=i;
                SavePicar(n->z,"C://น่าจะเอก.bmp");
            }
*/
/*
            if  (f->have[i]==1)
            {
                point[i]=0;
                mem[0]=abs(f->ascii[i]->up - n->up);
                mem[1]=abs(f->ascii[i]->z->h  - n->z->h );
                mem[2]=abs(f->ascii[i]->z->w  - n->z->w );
                mem[3]=abs( n->thin->loop_num - f->ascii[i]->thin->loop_num);
                mem[4]=abs( n->thin->path_long - f->ascii[i]->thin->path_long);
                mem[5]=abs((n->thin->node_pi[n->thin->path[0]]-n->up) - (f->ascii[i]->thin->node_pi[f->ascii[i]->thin->path[0]]-f->ascii[i]->up));
                mem[6]=abs( n->thin->node_pj[n->thin->path[0]] - f->ascii[i]->thin->node_pj[f->ascii[i]->thin->path[0]]);
                mem[7]=abs((n->thin->node_pi[n->thin->path[n->thin->path_num-1]]-n->up) - (f->ascii[i]->thin->node_pi[f->ascii[i]->thin->path[f->ascii[i]->thin->path_num-1]]-f->ascii[i]->up));
                mem[8]=abs( n->thin->node_pj[n->thin->path[n->thin->path_num-1]]        -  f->ascii[i]->thin->node_pj[f->ascii[i]->thin->path[f->ascii[i]->thin->path_num-1]]                 );
                if(mem[0]<=t && mem[1]<=t && mem[2]<=t
                   &&mem[3]<=1
                   &&mem[4]<=f->ascii[i]->thin->path_long*20/100
                   &&sqrt((mem[5]*mem[5])+(mem[6]*mem[6]))<1.4142*
                  )
                {
                    count++;
                }

            }
            else
            {
                point[i]=0;
            }
*/
char IntToCharRead(int x)
{
    char dum;
    if(x<=12)
        dum=161+x;
    else if(x==13)
        dum=161+12;
    else if(x>=14 && x<=16)
        dum=161+x-1;
    else if(x==17)
        dum=161+17-2;
    else if(x>=18 && x<=47)
        dum=161+x-2;
    else if(x==48)
        dum='ฯ';
    else if(x==49)
        dum='ะ';
    else if(x==50)
        dum='ะ';
    else if(x==51)
        dum='า';
    else if(x==52)
        dum='เ';
        return dum;
}
void ChooseLevel2(int *point,alpha *n,myfont *f)
{
    int num[24]={0,3,4,5,6,7,9,
    167,168,169,
    263,
    184,
    194,195,
    197,
    199,
    202,
    205,206,207,208,
    210,
    224,225};

    bool check[24];
    float dis[24],min,g;
    int i,j,k,count=0,answer;
    for(i=0;i<24;i++)
    {
        check[i]=false;
        dis[i]=-1;
    }
    //{161,172,173,174,175,179,182,192,196,198, 261}
    //0
    if(
    point[161]==1 ||
    point[172]==1 ||
    point[173]==1 ||
    point[174]==1 ||
    point[175]==1 ||
    point[179]==1 ||
    point[182]==1 ||
    point[192]==1 ||
    point[196]==1 ||
    point[198]==1 ||

    point[261]==1)
    {
        check[0]=true;
    }
    //{183,185,186,187,190,191,193,201,203,204}
    //1
    if(
    point[183]==1 ||
    point[185]==1 ||
    point[186]==1 ||
    point[187]==1 ||
    point[190]==1 ||
    point[191]==1 ||
    point[193]==1 ||
    point[201]==1 ||
    point[203]==1 ||
    point[204]==1 )
    {
        check[1]=true;
    }

    //{162,170}
    //2
    if(
    point[162]==1 ||
    point[170]==1 )
    {
        //เพื่อความถูกต้องอาจ check ด้วยว่าหัว ข มันทาบหัว ช ได้ดีหรือไม่
        check[2]=true;
    }
    //{163,166,171,177}
    //3
    if(
    point[163]==1 ||
    point[166]==1 ||
    point[171]==1 ||
    point[177]==1 )
    {
        //เพื่อความถูกต้องอาจ check ด้วยว่าหัว ฃ มันทาบหัว ซ ได้ดีหรือไม่
        check[3]=true;
    }
    //{178,180,181}
    //4
    if(
    point[178]==1 ||
    point[180]==1 ||
    point[181]==1 )
    {
        check[4]=true;
    }
    //5
    //{164,165,200}
    if(
    point[164]==1 ||
    point[165]==1 ||
    point[200]==1 )
    {
        check[5]=true;
    }
    //{188,189}
    //6
    if(
    point[188]==1 ||
    point[189]==1 )
    {
        check[6]=true;
    }
    for(i=0;i<=6;i++)
    {
        if(check[i]==true)
        {
            dis[i]=compare(n,f,num[i]);               //*****ส่วนสำคัญของการประมวลผล
        }
    }
    //______________________________________________
    //ส่วนของตัวอักษรเต็ม
    for(i=7;i<24;i++)
    {
        if(point[num[i]]==1)
        {
            check[i]=true;
            dis[i]=distance(f->ascii[num[i]],n);      //*****ส่วนสำคัญของการประมวลผล
        }
    }
    ////เปรียบเทียบ distance ที่ได้มา หา distance ที่น้อยที่สุด
    min=10000;
    for(i=0;i<24;i++)
    {
        if(dis[i]<min && dis[i]!=-1)
        {                            
            min=dis[i];
            j=i;
        }
    }

    if(j>6)    //เหลือเพียงแค่ตัวเดียวแล้ว
    {
        point[j]=1;
        n->c[1]=f->ascii[num[j]]->c[1];
        n->ans=num[j];
    }
    else      //เหลืออยู่ในกลุ่ม
    {
        if(j==0)
        {
            for(i=0;i<all;i++)
            {
                if(point[i]==1)
                {
                    if(!(
                    161==i ||
                    172==i ||
                    173==i ||
                    174==i ||
                    175==i ||
                    179==i ||
                    182==i ||
                    192==i ||
                    196==i ||
                    198==i ||
                    261==i))
                    {
                        point[i]=0;
                    }
                    else
                    {
                        count++;
                    }
                }
            }
            //น่าจะมี ฟังก์ชันรองรับพวก 'ก'
        }
        else if(j==1)
        {
            for(i=0;i<all;i++)
            {
                if(point[i]==1)
                {
                    if(!(
                    183==i ||
                    185==i ||
                    186==i ||
                    187==i ||
                    190==i ||
                    191==i ||
                    193==i ||
                    201==i ||
                    203==i ||
                    204==i ))
                    {
                        point[i]=0;
                    }
                    else
                    {
                        count++;
                    }
                }

            }
        }
        else if(j==2)
        {
            for(i=0;i<all;i++)
            {
                if(point[i]==1)
                {
                    if(!(
                    162==i ||
                    170==i ))
                    {
                        point[i]=0;
                    }
                    else
                    {
                        count++;
                    }
                }

            }
        }
        else if(j==3)
        {
            for(i=0;i<all;i++)
            {
                if(point[i]==1)
                {
                    if(!(
                    163==i ||
                    166==i ||
                    171==i ||
                    177==i ))
                    {
                        point[i]=0;
                    }
                    else
                    {
                        count++;
                    }
                }

            }
        }
        else if(j==4)
        {
            for(i=0;i<all;i++)
            {
                if(point[i]==1)
                {
                    if(!(
                    178==i ||
                    180==i ||
                    181==i ))
                    {
                        point[i]=0;
                    }
                    else
                    {
                        count++;
                    }
                }

            }
        }
        else if(j==5)
        {
            for(i=0;i<all;i++)
            {
                if(point[i]==1)
                {
                    if(!(
                    164==i ||
                    165==i ||
                    200==i ))
                    {
                        point[i]=0;
                    }
                    else
                    {
                        count++;
                    }
                }

            }
        }
        else if(j==6)
        {
            for(i=0;i<all;i++)
            {
                if(point[i]==1)
                {
                    if(!(
                    188==i ||
                    189==i ))
                    {
                        point[i]=0;
                    }
                    else
                    {
                        count++;
                    }
                }
            }
        }
        if(count==1)
        {

            for(k=0;k<all;k++)
            {
                if(point[k]==1)
                {
                    n->c[1]=f->ascii[k]->c[1];
                    n->ans=k;
                    break;
                }
            }

        }
        else if(count>1)
        {

            min=10000;
            for(k=0;k<all;k++)
            {
                if(point[k]==1)
                {
                    g=distance2(n,f->ascii[k]);

                    if(g<min)
                    {
                        min=g;
                        answer=k;
                    }
                }
            }
            n->c[1]=f->ascii[answer]->c[1];
            n->ans=answer;
        }
        else if(count==0)
        {
            ShowMessage("ChooseLevel2 had a problem");
            n->c[1]='\0';
            n->ans=-1;
        }
    }
}
char **MakeDoc(alpha ***c,int row,int *num,int *gap)
{
    int i,j,k,n,count,sumgap;
    char **r;
    r=(char **)malloc(sizeof(char *)*row);
    for(i=0;i<row;i++)
    {
        //หาเนื้อที่ที่ต้องใช้แต่ละบรรทัด = num[i]+จำนวน gap
        sumgap=0;
        for(j=0;j<num[i];j++)
        {
            if(c[i][j]->gap>0)
            {
                sumgap+=c[i][j]->gap;
            }
        }
        n=sumgap/gap[i];
        r[i]=(char *)malloc(sizeof(char)*(num[i]+n));

        count=0;
        for(j=0;j<num[i]-1;j++)
        {
            //gap
            n=c[i][j]->gap/gap[i];
            for(k=0;k<n;k++)
            {
                r[i][count]=' ';
                count++;
            }
            //ญ
            if(c[i][j]->c[1]=='ญ' && c[i][j]->type==2)
            {
                if(c[i][j+1]->c[1]=='ญ' && c[i][j+1]->type==1)
                {
                    r[i][count]='ญ';
                    count++;
                    j++;
                }
                else
                {
                    r[i][count]='ญ';
                    count++;
                }
            }
            //ฐ
            else if(c[i][j]->c[1]=='ฐ' && c[i][j]->type==2)
            {
                if(c[i][j+1]->c[1]=='ฐ' && c[i][j+1]->type==1)
                {
                    r[i][count]='ฐ';
                    count++;
                    j++;
                }
                else
                {
                    r[i][count]='ฐ';
                    count++;
                }
            }
            //ะ
            else if(c[i][j]->c[1]=='ะ' && c[i][j]->type==2)
            {
                if(c[i][j+1]->c[1]=='ะ' && c[i][j+1]->type==2)
                {
                    r[i][count]='ะ';
                    count++;
                    j++;
                }
                else
                {
                    r[i][count]='ฐ';
                    count++;
                }
            }
            //ํ ำ
            else if(c[i][j]->c[1]=='ํ' && c[i][j]->type==3)
            {
                if(c[i][j+1]->c[1]=='า' && c[i][j+1]->type==2)
                {
                    r[i][count]='ำ';
                    count++;
                    j++;
                }
                else if(c[i][j+1]->type==4 && c[i][j+2]->c[1]=='า')
                {
                    r[i][count]=c[i][j+1]->c[1];
                    count++;
                    r[i][count]='ำ';
                    count++;
                    j+=2;
                }
                else
                {
                    r[i][count]='ํ';
                    count++;
                }
            }
            //แ เ
            else if(c[i][j]->c[1]=='เ' || c[i][j]->c[1]=='แ')
            {
                if(c[i][j+1]->c[1]!='เ' && c[i][j+1]->c[1]!='แ')
                {
                    if(c[i][j]->ans==224 ||c[i][j]->ans==262 ||c[i][j]->ans==263)
                    {
                        r[i][count]='เ';
                        count++;
                    }
                    else if(c[i][j]->ans==225)
                    {
                        r[i][count]='แ';
                        count++;
                    }
                }
                else
                {
                    r[i][count]='แ';
                    count++;
                    j++;
                }
            }
            //enter
            else if(c[i][j]->type==0)
            {
                r[i][count]='\n';
                count++;
            }
            //general
            else if(c[i][j]->ans!=-1)
            {
                r[i][count]=c[i][j]->c[1];
                count++;
            }
            //ตัวที่มีขนาด หรือตำแหน่งผิดปกติ หาคำตอบไม่ได้ คาดว่าเป็นตัวติดกัน
            else
            {
                r[i][count]='#';
                count++;
            }
        }
        r[i][count]='\0';
        num[i]=count;
    }
    return r;
}
void FreeLink(link *map)
{
    int i;

        for(i=0;i<map->d;i++)
        {
            free(map->note[i]);
        }
        free(map->note);
        free(map);
}
void FreeThindata(thindata *t)
{
    int i,j,k;
    /*
    for(i=0;i<t->loop_num;i++)
    {
        free(t->loop_ji[i]);
        free(t->loop_jj[i]);

    }
    free(t->loop_ji);
    free(t->loop_jj);
    free(t->loop_juncnum);

    free(t->loop_pi);
    free(t->loop_pj);
    */
    if(t->node_num<6)
    {
        for(i=0;i<t->node_num;i++)
        {
            for(j=0;j<6;j++)
            {
                free(t->order[i][j]);
            }
            free(t->order[i]);
        }
        free(t->order);
    }
    else
    {
        for(i=0;i<t->node_num;i++)
        {
            for(j=0;j<t->node_num;j++)
            {
                free(t->order[i][j]);
            }
            free(t->order[i]);
        }
        free(t->order);
    }

    free(t->node_pi);
    free(t->node_pj);

    for(i=0;i<t->node_num;i++)
    {
        for(j=0;j<t->node_num;j++)
        {
            for(k=0;k<t->have[i][j];k++)
            {
                FreeLink(t->map[i][j][k]);
            }
            if(t->have[i][j]>0)
            {
                free(t->map[i][j]);
            }
            
        }
        free(t->map[i]);
        free(t->use[i]);
    }
    free(t->map);
    free(t->use);
    free(t->linknode);

    if(t->node_num>0)
    {
        for(i=0;i<t->path_num;i++)
        {
            free(t->branch[i]);
            free(t->branchno[i]);
        }
        free(t->branch);
        free(t->branchno);
        free(t->branch_last);
        free(t->branch_long);
        free(t->branch_num);
        free(t->path);
        free(t->pathno);
    }
    FreePicar(t->z);

    for(i=0;i<t->node_num;i++)
    {
        free(t->have[i]);
    }
    free(t->have);

    free(t);
}
/*
void FindLongest1(thindata *t,int s,walkpath *mempath)
{
    int i,j,sum=0;
    for(i=0;i<t->node_num;i++)
    {
        if(t->have[s][i] && i!=mempath->path[mempath->n-2]) //มีการ link ไป &&  ไม่กลับไปทางเก่า
        {
            mempath->path[mempath->n]=i;
            mempath->n++;
            mempath->d+=t->map[s][i]->d+1;
            FindLongest1(t,i,mempath);
            mempath->d-=t->map[s][i]->d+1;
            mempath->n--;
            mempath->path[mempath->n]=-1;
            sum++;
        }
    }
    if(sum==0) //เป็นจุดปลายแล้ว ตันแล้ว
    {
        if(mempath->d+1 > t->path_long)     //distance มากกว่าเดิม copy ไปได้
        {
            for(i=0;i<mempath->n;i++)
            {
                t->path[i]=mempath->path[i];
            }
            for(;i<t->node_num;i++)
            {
                t->path[i]=-1;
            }
            t->path_long=mempath->d+1;
            t->path_num=mempath->n;
        }
    }
}

void FindLongestMain1(thindata *t)
{
    int i,j,sum=0;
    int *n;
    walkpath *mempath;
    mempath=(walkpath *)malloc(sizeof(walkpath));

    mempath->path=(int *)malloc(sizeof(int)*t->node_num);

    t->path_num=0;
    t->path_long=0;
    t->path=(int *)malloc(sizeof(int)*t->node_num);

    n=(int *)malloc(sizeof(int)*t->node_num);
    for(i=0;i<t->node_num;i++)
    {
        t->path[i]=-1;
        mempath->path[i]=-1;
        n[i]=0;
        for(j=0;j<t->node_num;j++)
        {
            if(t->have[i][j]==true)
            {
                n[i]++;
                sum++;
            }
        }
    }
    if(sum!=0)
    {
        for(i=0;i<t->node_num;i++)
        {
            if(n[i]==1)  //จะเริ่มเมื่อเป็นจุดปลาย
            {
                for(j=0;j<t->node_num;j++)
                {
                    if(t->have[i][j]==true)
                    {
                        mempath->n=2;
                        mempath->path[0]=i;
                        mempath->path[1]=j;
                        mempath->d=t->map[i][j]->d+1;
                        FindLongest1(t,j,mempath);
                        break;
                    }
                }
            }
        }
    }
    free(n);
    free(mempath);

    if(sum==0 && t->node_num==1)
    {
        t->path_long=1;
        t->path_num=1;
        t->path[0]=0;
    }
    else    //สลับเอาจุดเริ่มต้นมาไว้ด้านซ้าย
    {
        if(t->node_pj[t->path[0]] > t->node_pj[t->path[t->path_num-1]])
        {
             n=(int *)malloc(sizeof(int)*t->path_num);
             j=t->path_num-1;
             for(i=0;i<t->path_num;i++,j--)
             {
                n[j]=t->path[i];
             }
             for(i=0;i<t->path_num;i++)
             {
                t->path[i]=n[i];
             }
             free(n);
        }
        else if(
                (t->node_pj[t->path[0]] == t->node_pj[t->path[t->path_num-1]])
                &&(t->node_pi[t->path[0]] > t->node_pi[t->path[t->path_num-1]])
               )
        {
             n=(int *)malloc(sizeof(int)*t->path_num);
             j=t->path_num-1;
             for(i=0;i<t->path_num;i++,j--)
             {
                n[j]=t->path[i];
             }
             for(i=0;i<t->path_num;i++)
             {
                t->path[i]=n[i];
             }
             free(n);
        }
    }
}
*/
void FindBranch(thindata *t,int *s,walkpath *mempath,bool *already,int now)
{
    int i,j,sum=0;
    mempath->path[mempath->n]=s[0];
    mempath->n++;
    if(mempath->n==1 && mempath->d==0) //ส่งมาจากจุดเริ่มต้น
    {
        for(i=0;i<t->linknode[s[0]];i++)
        {
            if(t->use[s[0]] [t->order[s[0]][i][0]] [t->order[s[0]][i][1]]==0) ////เพิ่มทีหลัง
            {
                mempath->d+=t->map[s[0]] [t->order[s[0]][i][0]] [t->order[s[0]][i][1]]->d+1;
                t->use[s[0]] [t->order[s[0]][i][0]] [t->order[s[0]][i][1]]=1; ////เพิ่มทีหลัง
                t->use[t->order[s[0]][i][0]] [s[0]] [t->order[s[0]][i][1]]=1; ////เพิ่มทีหลัง
                //already[s[0]]=true;  ไม่จำเป็นเพราะเป็น true อยู่แล้ว
                FindBranch(t,t->order[s[0]][i],mempath,already,now);
                t->use[s[0]] [t->order[s[0]][i][0]] [t->order[s[0]][i][1]]=0; ////เพิ่มทีหลัง
                t->use[t->order[s[0]][i][0]] [s[0]] [t->order[s[0]][i][1]] =0; ////เพิ่มทีหลัง
                //already[s[0]]=false; เปลี่ยนไม่ได้เพราะมันเป็นของเดิม
                mempath->d-=t->map[s[0]] [t->order[s[0]][i][0]] [t->order[s[0]][i][1]]->d+1;
                sum++;
            }
        }
        if(sum==0)   //ไม่มีที่ให้ไปต่อตั้งแต่เริ่มต้น
        {
            t->branch[now][0]=t->path[now];
            t->branch_long[now]=0;
            t->branch_num[now]=1;
        }
    }
    else if(already[s[0]]==true || t->linknode[s[0]]==1)         //จบแล้ว
    {
        int v;
        mempath->pathno[mempath->n-2]=s[1];
        if(already[s[0]]==true)
        {
            v=0;        //วกมา
        }
        else if(t->linknode[s[0]]==1)
        {
            v=1;        //จบที่ปลาย
        }
        if(mempath->d+v > t->branch_long[now])     //distance มากกว่าเดิม copy ไปได้
        {
            for(i=0;i<mempath->n;i++)
            {
                t->branch[now][i]=mempath->path[i];
                t->branchno[now][i]=mempath->pathno[i];
            }
            /*
            t->branchno[now][i-1]=-1;
            for(;i<t->node_num;i++)
            {
                t->branch[now][i]=-1;
                t->branchno[now][i]=-1;
            }
            */
            t->branch_long[now]=mempath->d+v-1; ////////มาเพิ่ม -1 ทีหลัง
            t->branch_num[now]=mempath->n;
            if(t->linknode[s[0]]==1)
            {
                t->branch_last[now]=1;
            }
            else if(already[s[0]]==true)
            {
                t->branch_last[now]=2;
            }
            
        }
        //mempath->pathno[mempath->n-2]=-1;
    }
    else if(already[s[0]]==false)
    {
        mempath->pathno[mempath->n-2]=s[1];
        for(i=0;i<t->linknode[s[0]];i++)    //หาจุดที่เก่าที่เดินผ่านมา
        {
            if(t->order[s[0]][i][0]==mempath->path[mempath->n-2]
            && t->order[s[0]][i][1]==mempath->pathno[mempath->n-2]     //มาเพิ่มทีหลัง
            )
            {
                break;
            }
        }
        while(true)             //ไล่เข้าตามเข็มนาฬิกา
        {
            i++;
            if(i>=t->linknode[s[0]])
            {
                i=0;
            }
            if( (t->order[s[0]][i][0]==mempath->path[mempath->n-2])
            && (t->order[s[0]][i][1]==mempath->pathno[mempath->n-2]))
            {
                break;
            }

            if( t->use[s[0]] [t->order[s[0]][i][0]] [t->order[s[0]][i][1]] ==0)
            {
                mempath->d+=t->map[s[0]] [t->order[s[0]][i][0]] [t->order[s[0]][i][1]]->d+1;
                t->use[s[0]] [t->order[s[0]][i][0]] [t->order[s[0]][i][1]]=1;
                t->use[t->order[s[0]][i][0]] [s[0]] [t->order[s[0]][i][1]]=1;
                already[s[0]]=true;
                FindBranch(t,t->order[s[0]][i],mempath,already,now);
                already[s[0]]=false;
                t->use[s[0]] [t->order[s[0]][i][0]] [t->order[s[0]][i][1]]=0;
                t->use[t->order[s[0]][i][0]] [s[0]] [t->order[s[0]][i][1]] =0;
                mempath->d-=t->map[s[0]] [t->order[s[0]][i][0]] [t->order[s[0]][i][1]]->d+1;
            }
        }
        //mempath->pathno[mempath->n-2]=-1;
    }

    mempath->n--;
    //mempath->path[mempath->n]=-1;

}

void FindLongest2(thindata *t,int *s,walkpath *mempath,bool *already)
{
    int i,j,sum=0,v;
    mempath->path[mempath->n]=s[0];
    mempath->n++;
    if(mempath->n==1 && mempath->d==0) //ส่งมาจากจุดเริ่มต้น
    {
        for(i=0;i<t->linknode[s[0]];i++)
        {

            mempath->d+=t->map[s[0]] [t->order[s[0]][i][0]] [t->order[s[0]][i][1]]->d+1;

            already[s[0]]=true;
            FindLongest2(t,t->order[s[0]][i],mempath,already);

            already[s[0]]=false;
            mempath->d-=t->map[s[0]] [t->order[s[0]][i][0]] [t->order[s[0]][i][1]]->d+1;

        }
    }
    else if(already[s[0]]==true || t->linknode[s[0]]==1)         //จบแล้ว
    {

        mempath->pathno[mempath->n-2]=s[1];
        if(already[s[0]]==true)
        {
            v=0;    //จบที่จุดซ้อนทับ บวกเพิ่มอีกไม่ได้
        }
        else if(t->linknode[s[0]]==1)
        {
            v=1;    //จบที่จุดปลาย ต้องบวกเพิ่มอีก
        }
        if(mempath->d +v > t->path_long)     //distance มากกว่าเดิม copy ไปได้
        {

            for(i=0;i<mempath->n;i++)
            {
                t->path[i]=mempath->path[i];
                t->pathno[i]=mempath->pathno[i];
            }

            /*น่าสงสัย
            t->pathno[i-1]=-1;
            for(;i<t->node_num;i++)
            {
                t->path[i]=-1;
                t->pathno[i]=-1;
            }
            */

            t->path_long=mempath->d+v;
            t->path_num=mempath->n;
            if(already[s[0]]==true)
            {
                t->atlast=2;
            }
            else if(t->linknode[s[0]]==1)
            {
                t->atlast=1;
            }
            
        }
        //mempath->pathno[mempath->n-2]=-1;   น่าสงสัย
    }
    else if(already[s[0]]==false)
    {
        mempath->pathno[mempath->n-2]=s[1];
        for(i=0;i<t->linknode[s[0]];i++)    //หาจุดที่เก่าที่เดินผ่านมา
        {
            if(t->order[s[0]][i][0]==mempath->path[mempath->n-2]
            && t->order[s[0]][i][1]==mempath->pathno[mempath->n-2]     //มาเพิ่มทีหลัง
            )
            {
                break;
            }
        }
        while(1)             //ไล่เข้าตามเข็มนาฬิกา
        {
            i++;
            if(i>=t->linknode[s[0]])
            {
                i=0;
            }
            
            if(t->order[s[0]][i][0]==mempath->path[mempath->n-2]
            && t->order[s[0]][i][1]==mempath->pathno[mempath->n-2])
            {
                break;
            }
            mempath->d+=t->map[s[0]] [t->order[s[0]][i][0]] [t->order[s[0]][i][1]]->d+1;

            already[s[0]]=true;
            FindLongest2(t,t->order[s[0]][i],mempath,already);
            already[s[0]]=false;

            mempath->d-=t->map[s[0]] [t->order[s[0]][i][0]] [t->order[s[0]][i][1]]->d+1;

        }
        //mempath->pathno[mempath->n-2]=-1;   น่าสงสัย
    }

    mempath->n--;
    //mempath->path[mempath->n]=-1;  น่าสงสัย

}

void FindLongestMain2(thindata *t)  //จะหาโดยไม่รวบ loop มาก่อน และ จะใส่กิ่งเข้าไปด้วย
{
    int i,j,sum=0;
    int *n;
    bool *already;
    int s[2];
    walkpath *mempath;

    mempath=(walkpath *)malloc(sizeof(walkpath));
    mempath->path=(int *)malloc(sizeof(int)*(t->node_num+1));
    mempath->pathno=(int *)malloc(sizeof(int)*t->node_num+1);

    already=(bool *)malloc(sizeof(bool)*t->node_num);
    t->path_num=0;
    t->path_long=0;
    t->path=(int *)malloc(sizeof(int)*t->node_num+1);
    t->pathno=(int *)malloc(sizeof(int)*t->node_num+1);
    //n=(int *)malloc(sizeof(int)*t->node_num);
    for(i=0;i<t->node_num;i++)
    {
        already[i]=false;
        //t->path[i]=-1;
        //t->pathno[i]=-1;
        //mempath->path[i]=-1;

        for(j=0;j<t->node_num;j++)
        {
            //n[i]+=t->have[i][j];
            sum+=t->have[i][j];
        }
    }
    mempath->d=0;
    mempath->n=0;
    //s[1]=-1;
    if(sum!=0)
    {
        if(t->end_num>0)
        {
            for(i=0;i<t->node_num;i++)
            {
                if(t->linknode[i]==1)  //จะเริ่มเมื่อเป็นจุดปลาย
                {
                    s[0]=i;
                    FindLongest2(t,s,mempath,already);
                }
            }
        }
        else
        {
            for(i=0;i<t->node_num;i++)
            {
                if(t->linknode[i]>2)  //จะเริ่มเมื่อเป็นทางแยก
                {
                    s[0]=i;
                    FindLongest2(t,s,mempath,already);
                }
            }
        }
    }
    //free(n);

    //ดูว่าใช้เส้นทาง และ node ไหนไปบ้าง จะได้ไม่ใช้ซ้ำ ในการหากิ่ง
    for(i=0;i<t->path_num-1;i++)
    {
        t->use
        [ t->path[i]   ]
        [ t->path[i+1] ]
        [ t->pathno[i] ]
        =1;
        t->use
        [ t->path[i+1] ]
        [ t->path[i]   ]
        [ t->pathno[i] ]
        =1;
        already[ t->path[i]   ]=true;
    }
    already[ t->path[t->path_num-1] ]=true;
    //จัดก่อนใช้ FindBranch

    t->branch_num=(int *)malloc(sizeof(int)*t->path_num);
    t->branch_long=(int *)malloc(sizeof(int)*t->path_num);
    t->branch_last=(int *)malloc(sizeof(int)*t->path_num);

    t->branch=(int **)malloc(sizeof(int *)*t->path_num);
    t->branchno=(int **)malloc(sizeof(int *)*t->path_num);
    for(i=0;i<t->path_num;i++)
    {
        t->branch[i]=(int *)malloc(sizeof(int)*t->node_num+1);
        t->branchno[i]=(int *)malloc(sizeof(int)*t->node_num);
        t->branch_num[i]=0;
        t->branch_long[i]=0;
        mempath->n=0;
        mempath->d=0;
        s[0]=t->path[i];
        //s[1]=-1;
        FindBranch(t,s,mempath,already,i);
        //เคลียร์ให้ใช้ทางเดิมไม่ได้
        for(j=0;j<t->branch_num[i]-1;j++)
        {
            t->use
            [ t->branch[i][j]   ]
            [ t->branch[i][j+1] ]
            [ t->branchno[i][j] ]
            =1;
            t->use
            [ t->branch[i][j+1] ]
            [ t->branch[i][j]   ]
            [ t->branchno[i][j] ]
            =1;
            already[ t->branch[i][j] ]=true;
        }
        already[ t->branch[i][ t->branch_num[i]-1 ] ]=true;
    }


    /*  //สลับเอาจุดเริ่มต้นมาไว้ด้านซ้าย
    if(sum==0 && t->node_num==1)
    {
        t->path_long=1;
        t->path_num=1;
        t->path[0]=0;
    }
    else    //สลับเอาจุดเริ่มต้นมาไว้ด้านซ้าย
    {
        if(t->node_pj[t->path[0]] > t->node_pj[t->path[t->path_num-1]])
        {
             n=(int *)malloc(sizeof(int)*t->path_num);
             j=t->path_num-1;
             for(i=0;i<t->path_num;i++,j--)
             {
                n[j]=t->path[i];
             }
             for(i=0;i<t->path_num;i++)
             {
                t->path[i]=n[i];
             }
             free(n);
        }
        else if(
                (t->node_pj[t->path[0]] == t->node_pj[t->path[t->path_num-1]])
                &&(t->node_pi[t->path[0]] > t->node_pi[t->path[t->path_num-1]])
               )
        {
             n=(int *)malloc(sizeof(int)*t->path_num);
             j=t->path_num-1;
             for(i=0;i<t->path_num;i++,j--)
             {
                n[j]=t->path[i];
             }
             for(i=0;i<t->path_num;i++)
             {
                t->path[i]=n[i];
             }
             free(n);
        }
    }
    */

    free(mempath->path);
    free(mempath->pathno);
    free(mempath);
    free(already);

}

picar *DrawPath(thindata *t)
{
    int i,j,k,count=0;
    picar *r;
    r=NewPicar(t->z->h,t->z->w,2);
    for(i=0;i<r->h;i++)
    {
        for(j=0;j<r->w;j++)
        {
            r->a[i][j]=255;
        }
    }
    for(i=0;i<t->path_num-1;i++)
    {
        r->a[t->node_pi[t->path[i]]] [t->node_pj[t->path[i]]]=count*3;
        count++;
        for(j=0;j<t->map[t->path[i]] [t->path[i+1]] [t->pathno[i]]->d;j++)
        {
            r->a[t->map[t->path[i]] [t->path[i+1]] [t->pathno[i]]->note[j][0]]
                [t->map[t->path[i]] [t->path[i+1]] [t->pathno[i]]->note[j][1]]=count*3;
            count++;
        }
    }
    if(t->atlast==1)
    {
        r->a[t->node_pi[t->path[t->path_num-1]]] [t->node_pj[t->path[t->path_num-1]]]=count*3;
        count++;
    }
    //ShowImage(r,Form1->Image2,Rect2);
    //เขียนกิ่งต่อ
    for(i=0;i<t->path_num;i++)
    {
        for(j=0;j<t->branch_num[i]-1;j++)
        {
            for(k=0;k<t->map[t->branch[i][j]] [t->branch[i][j+1]] [t->branchno[i][j]]->d;k++)
            {
                r->a[t->map [t->branch[i][j]] [t->branch[i][j+1]] [t->branchno[i][j]] ->note[k][0] ]
                    [t->map [t->branch[i][j]] [t->branch[i][j+1]] [t->branchno[i][j]] ->note[k][1] ]
                    =count*3;
                count++;
            }
            if(j<t->branch_num[i]-2)
            {
                r->a[t->node_pi[t->branch[i][j+1]]] [t->node_pj[t->branch[i][j+1]]]=count*3;
                count++;
            }
        }
        if(t->branch_last[i]==1 && t->branch_num[i]>1)
        {
            r->a[t->node_pi[ t->branch[i][t->branch_num[i]-1] ]] [t->node_pj[t->branch[i][t->branch_num[i]-1]]]=count*3;
            count++;
        }
    }
    return r;
}

link *MakePath1(thindata *t)     //เอาเฉพาะสายกลางเท่านั้น
{
    int i,j,count=0;
    link *r;
    r=(link *)malloc(sizeof(link));
    r->d=t->path_long;
    r->note=(int **)malloc(sizeof(int *)*t->path_long);
    for(i=0;i<t->path_long;i++)
    {
        r->note[i]=(int *)malloc(sizeof(int)*2);
    }
    for(i=0;i<t->path_num-1;i++)
    {
        r->note[count][0]=t->node_pi[t->path[i]];
        r->note[count][1]=t->node_pj[t->path[i]];
        count++;
        for(j=0;j<t->map[t->path[i]] [t->path[i+1]] [t->pathno[i]]->d;j++)
        {
            r->note[count][0]=t->map[t->path[i]] [t->path[i+1]] [t->pathno[i]]->note[j][0];
            r->note[count][1]=t->map[t->path[i]] [t->path[i+1]] [t->pathno[i]]->note[j][1];
            count++;
        }
        ////////////////////
    }
    if(t->atlast==1)
    {
        r->note[count][0]=t->node_pi[t->path[t->path_num-1]];
        r->note[count][1]=t->node_pj[t->path[t->path_num-1]];
        count++;
    }
    return r;
}
link *MakePath2(thindata *t)     //สายกลาง + กิ่งด้วย เดินไป โดดกลับ
{
    int i,j,k,count=0;
    link *r;
    r=(link *)malloc(sizeof(link));
    r->d=t->path_long;
    for(i=0;i<t->path_num;i++)
    {
        r->d+=t->branch_long[i];
    }
    r->note=(int **)malloc(sizeof(int *)*r->d);
    for(i=0;i<r->d;i++)
    {
        r->note[i]=(int *)malloc(sizeof(int)*2);
    }
    for(i=0;i<t->path_num-1;i++)
    {
        r->note[count][0]=t->node_pi[t->path[i]];
        r->note[count][1]=t->node_pj[t->path[i]];
        count++;
        //ถึงnodeแล้วทำกิ่งก่อน
        for(j=0;j<t->branch_num[i]-1;j++)
        {
            for(k=0;k<t->map[t->branch[i][j]] [t->branch[i][j+1]] [t->branchno[i][j]]->d;k++)
            {
                r->note[count][0]=t->map [t->branch[i][j]] [t->branch[i][j+1]] [t->branchno[i][j]] ->note[k][0];
                r->note[count][1]=t->map [t->branch[i][j]] [t->branch[i][j+1]] [t->branchno[i][j]] ->note[k][1];
                count++;
            }
            if(j<t->branch_num[i]-2)
            {
                r->note[count][0]=t->node_pi[t->branch[i][j+1]];
                r->note[count][1]=t->node_pj[t->branch[i][j+1]];
                count++;
            }
        }
        if(t->branch_last[i]==1 && t->branch_num[i]>1)
        {
            r->note[count][0]=t->node_pi[ t->branch[i][t->branch_num[i]-1] ];
            r->note[count][1]=t->node_pj[ t->branch[i][t->branch_num[i]-1] ];
            count++;
        }
        //โดดกลับมาทำสายเพื่อเดินสู่ node ต่อไป
        for(j=0;j<t->map[t->path[i]] [t->path[i+1]] [t->pathno[i]]->d;j++)
        {
            r->note[count][0]=t->map[t->path[i]] [t->path[i+1]] [t->pathno[i]]->note[j][0];
            r->note[count][1]=t->map[t->path[i]] [t->path[i+1]] [t->pathno[i]]->note[j][1];
            count++;
        }
        ////////////////////
    }
    if(t->atlast==1)
    {
        r->note[count][0]=t->node_pi[t->path[t->path_num-1]];
        r->note[count][1]=t->node_pj[t->path[t->path_num-1]];
        count++;
    }
    return r;
}
link *MakePath3(thindata *t)     //สายกลาง + กิ่งด้วย เดินไป เดินกลับ
{
    int i,j,k,count=0,mem;
    link *r;
    r=(link *)malloc(sizeof(link));
    r->d=t->path_long;
    for(i=0;i<t->path_num;i++)
    {
        if(t->branch_long[i]>0)
        {
            r->d+=t->branch_long[i]+t->branch_long[i]-1;
        }
    }
    r->note=(int **)malloc(sizeof(int *)*r->d);
    for(i=0;i<r->d;i++)
    {
        r->note[i]=(int *)malloc(sizeof(int)*2);
    }
    for(i=0;i<t->path_num-1;i++)
    {
        r->note[count][0]=t->node_pi[t->path[i]];
        r->note[count][1]=t->node_pj[t->path[i]];
        count++;
        //ถึงnodeแล้วทำกิ่งก่อน
        for(j=0;j<t->branch_num[i]-1;j++)
        {
            for(k=0;k<t->map[t->branch[i][j]] [t->branch[i][j+1]] [t->branchno[i][j]]->d;k++)
            {
                r->note[count][0]=t->map [t->branch[i][j]] [t->branch[i][j+1]] [t->branchno[i][j]] ->note[k][0];
                r->note[count][1]=t->map [t->branch[i][j]] [t->branch[i][j+1]] [t->branchno[i][j]] ->note[k][1];
                count++;
            }
            if(j<t->branch_num[i]-2)
            {
                r->note[count][0]=t->node_pi[t->branch[i][j+1]];
                r->note[count][1]=t->node_pj[t->branch[i][j+1]];
                count++;
            }
        }
        if(t->branch_last[i]==1 && t->branch_num[i]>1)
        {
            r->note[count][0]=t->node_pi[ t->branch[i][t->branch_num[i]-1] ];
            r->note[count][1]=t->node_pj[ t->branch[i][t->branch_num[i]-1] ];
            count++;
        }
        //ถึงปลายกิ่งแล้ว ต้องกลับทางเดิม ไปจนถึงก่อนทางแยก
        mem=count-1;
        for(j=1;j<t->branch_long[i];j++)
        {
            r->note[count][0]=r->note[mem-j][0];
            r->note[count][1]=r->note[mem-j][1];
            count++;
        }
        //โดดกลับมาถึงสายหลัก เพื่อเดินสู่ node ต่อไป
        for(j=0;j<t->map[t->path[i]] [t->path[i+1]] [t->pathno[i]]->d;j++)
        {
            r->note[count][0]=t->map[t->path[i]] [t->path[i+1]] [t->pathno[i]]->note[j][0];
            r->note[count][1]=t->map[t->path[i]] [t->path[i+1]] [t->pathno[i]]->note[j][1];
            count++;
        }
        ////////////////////
    }
    if(t->atlast==1)
    {
        r->note[count][0]=t->node_pi[t->path[t->path_num-1]];
        r->note[count][1]=t->node_pj[t->path[t->path_num-1]];
        count++;
    }
    return r;
}
/*
float ElasticMatching(thindata *t1,thindata *t2)
{
    int **l,**s;  //longer and shorter
    int nl,ns,point=0;
    float min=0,d,sum=0;
    int i,j,k;
    if(t1->path_num==t2->path_num) //จะไล่แบบเอา node มาเป็นหลักทุกอันเลย
    {
        for(k=0;k<t1->path_num-1;k++)
        {
            if(t1->map[t1->path[k]][t1->path[k+1]]->d>t2->map[t2->path[k]][t2->path[k+1]]->d)
            {
                 l =t1->map[t1->path[k]][t1->path[k+1]]->note;
                 nl=t1->map[t1->path[k]][t1->path[k+1]]->d;
                 s =t2->map[t2->path[k]][t2->path[k+1]]->note;
                 ns=t2->map[t2->path[k]][t2->path[k+1]]->d;
            }
            else
            {
                 l =t2->map[t2->path[k]][t2->path[k+1]]->note;
                 nl=t2->map[t2->path[k]][t2->path[k+1]]->d;
                 s =t1->map[t1->path[k]][t1->path[k+1]]->note;
                 ns=t1->map[t1->path[k]][t1->path[k+1]]->d;
            }
            point+=ns;
            for(i=0;i<ns;i++)
            {
                if(ns==1)
                {
                    j=0;
                }
                else
                {
                    j=(nl-1)*i/(ns-1);
                }
                d=sqrt((s[i][0]-l[j][0])*(s[i][0]-l[j][0])
                      +(s[i][1]-l[j][1])*(s[i][1]-l[j][1]));
                if(d<min)
                {
                    min=d;
                }
                sum+=d;
            }
        }
        for(k=0;k<t1->path_num;k++)
        {
            d=sqrt((t1->node_pi[t1->path[k]]-t2->node_pi[t1->path[k]])*(t2->node_pi[t2->path[k]]-t2->node_pi[t2->path[k]]))
                 +((t1->node_pj[t1->path[k]]-t2->node_pi[t1->path[k]])*(t2->node_pi[t2->path[k]]-t2->node_pi[t2->path[k]]));
            if(d<min)
            {
                min=d;
            }
            sum+=d;
            point++;
        }
        return (sum/point)-min;
    }
    else //จะเอาแค่ หัวกับท้ายเป็นหลักยึดไว้
    {
        if(t1->path_long>t2->path_long)
        {
            l=MakePath(t1);
            nl=t1->path_long;
            s=MakePath(t2);
            ns=t2->path_long;
        }
        else
        {
            s=MakePath(t1);
            ns=t1->path_long;
            l=MakePath(t2);
            nl=t2->path_long;
        }
        for(i=0;i<ns;i++)
        {
            if(ns==1)
            {
                j=0;
            }
            else
            {
                j=(nl-1)*i/(ns-1);
            }
            d=sqrt((s[i][0]-l[j][0])*(s[i][0]-l[j][0])
                  +(s[i][1]-l[j][1])*(s[i][1]-l[j][1]));
            if(d<min)
            {
                min=d;
            }
            sum+=d;
        }
        //free memory
        for(i=0;i<nl;i++)
        {
            free(l[i]);
        }
        free(l);
        for(i=0;i<ns;i++)
        {
            free(s[i]);
        }
        free(s);
        return (sum/ns)-min;
    }

}
*/
void FindAnswer1(onerow **bb,alpha ***c,int *row,int *num,int *gap)  //หาคำตอบจากการดึง feature และการทาบวัดระยะ(การทดลองที่ 1)
{
    myfont *f;
    FILE *r,*r2;
    int i,j,k,last=0;
    int count=0;
    char dum;
    int point[all];
    int answer,n;
    float g,min;
    //r=fopen("C:\\PostCut.txt","w");
    //r2=fopen("C:\\ant2.txt","w");
    for(i=0;i<*row;i++)
    {
        if(bb[i]->down-bb[i]->up+1!=last)
        {
            if(last!=0)
                FreeFont(f);
            f=LoadFont(Form1->ComboBox1->Text,false,false,bb[i]->down-bb[i]->up+1,Form1->Image4);
            /*
            for(j=0;j<10;j++)
            {
                SaveArray(f->test[j]->z->a,f->test[j]->z->h,f->test[j]->z->w,"C:\\new\\"+IntToStr(j)+".bmp");
            }
            */
            last=bb[i]->down-bb[i]->up+1;
        }
        gap[i]=f->gap;
        //fprintf(r,"Row=%d, Height=%d, Size=%d\n",i,f->h,f->size);
        min=10000;
        for(j=0;j<num[i];j++)
        {
            //clear point
            if(c[i][j]->type!=0 && c[i][j]->type!=2)
            {
                for(k=0;k<all;k++)
                {
                    point[i]=0;
                }

                if(c[i][j]->type==1 )   //level1
                {
                    point[216]=1;
                    point[217]=1;
                    point[262]=1;
                    point[264]=1;
                }
                else if(c[i][j]->type==3)   //level3
                {
                    point[209]=1;
                    for(k=212;k<=215;k++)
                    {
                       point[k]=1;
                    }
                    for(k=231;k<=237;k++)
                    {
                       point[k]=1;
                    }
                }
                else if(c[i][j]->type==4)   //level4
                {
                    for(k=256;k<=260;k++)
                    {
                       point[k]=1;
                    }
                }

                n=CompareSize1(point,c[i][j],f);
                //---------debug-------------------------
                /*
                fprintf(r,"%c ->%d\n",IntToCharRead(count),n);
                for(k=0;k<all;k++)
                {
                    if(point[k]==1)
                    {
                        fprintf(r," %c \t",f->ascii[k]->c[1]);
                    }
                }
                fprintf(r,"\n");
                count++;
                */
                //---------------------------------------
                if(n>=1)
                {
                    min=10000;
                    for(k=0;k<all;k++)
                    {
                        if(point[k]==1)
                        {
                            if(n>1)
                            {
                                g=distance2(c[i][j]    ,f->ascii[k]);  //*****ส่วนสำคัญของการประมวลผล
                                //distance(f->ascii[k],c[i][j]    );
                                if(g<min)
                                {
                                    min=g;
                                    answer=k;
                                }
                            }
                            else if(n==1)
                            {
                                answer=k;
                                break;
                            }
                        }
                    }
                    if(!(c[i][j]->type==3 && c[i][j]->c[1]=='ื'))
                    {
                        c[i][j]->c[1]=f->ascii[answer]->c[1];
                        c[i][j]->ans=answer;
                    }
                }
                else if(n==0)   ///ไม่มีคำตอบ น่าจะเป็นตัวติดกัน
                {
                    c[i][j]->c[1]='\0';
                    c[i][j]->ans=-1;
                }
            }
            else if(c[i][j]->type==2)
            {
                for(k=0;k<all;k++)
                {
                    point[k]=1;
                }
                n=CompareSize1(point,c[i][j],f);
                //---------debug-------------------------
                /*
                fprintf(r,"%c ->%d\n",IntToCharRead(count),n);
                for(k=0;k<all;k++)
                {
                    if(point[k]==1)
                    {
                        fprintf(r," %c \t",f->ascii[k]->c[1]);
                    }
                }
                fprintf(r,"\n");
                count++;
                */
                //---------------------------------------
                if(n>0 && n<=6)  //เหลือตัวน้อยแล้ว ทาบไปเลย แก้ได้
                {
                    min=10000;
                    for(k=0;k<all;k++)
                    {
                        if(point[k]==1)
                        {
                            g=distance2(c[i][j]    ,f->ascii[k]);     //*****ส่วนสำคัญของการประมวลผล
                            //distance(f->ascii[k],c[i][j]    );
                            if(g<min)
                            {
                                min=g;
                                answer=k;
                            }
                        }
                    }
                    c[i][j]->c[1]=f->ascii[answer]->c[1];
                    c[i][j]->ans=answer;

                }
                else if(n==0)    ///ไม่มีคำตอบ น่าจะเป็นตัวติดกัน
                {
                    c[i][j]->c[1]='\0';
                    c[i][j]->ans=-1;
                }
                else             //กรณีที่เหลือเยอะ
                {
                    ChooseLevel2(point,c[i][j],f);                  //*****ส่วนสำคัญของการประมวลผล
                }
            }

        }

    }
    FreeFont(f);
    /* //ดูแบบดิบ ๆ
    r=fopen("C:\\ant.txt","w");
    for(i=0;i<*row;i++)
    {
        for(j=0;j<num[i];j++)
        {
            if(c[i][j]->c[1]=='\0')
                fprintf(r,"-\n");
            else
                fprintf(r,"%c\n",c[i][j]->c[1]);
        }
    }
    fclose(r);
    */
    //fclose(r2);
    //fclose(r);
    //หาคำตอบเรียบร้อย******************************************************
}

void FindAnswer2(alpha ***c,int *row,int *num,int *gap)  //ตัดตัวที่ไม่ใช่จาก feature ที่มีมากขึ้นจากการใช้ thindata และใช้ Elastic Matching แทนการทาบแล้ววัดระยะ
{
    struct time stime,etime;
    myfont *f;
    FILE *r,*r2;
    link *x,*y;
    int i,j,k,l,last=0;
    int count=0;
    char dum;
    int point[269];
    int answer,n,d;
    float g,min;

    int maxsize=0,newmax;
    myfont **allsize;
    bool *usesize;
    allsize=(myfont **)malloc(sizeof(myfont *));
    usesize=(bool *)malloc(sizeof(bool));
    r=fopen("C:\\PostCut.txt","w");
    //r2=fopen("C:\\ant2.txt","w");

    int cpro=1,lpro,tpro=0; //for use in progress bar
    lpro=Form1->ProgressBar1->Position;
    for(i=0;i<*row;i++)
    {
        tpro+=num[i];
    }

    for(i=0;i<*row;i++)
    {
        if(c[i][0]->down-c[i][0]->up+1<maxsize)
        {
            if(usesize[c[i][0]->down-c[i][0]->up+1]==true)
            {
                f=allsize[c[i][0]->down-c[i][0]->up+1];
            }
            else
            {
                Form1->Label1->Caption = "Loading Font";       //คุง
                Form1->Label1->Refresh();

                //แทรกส่วนจับเวลา ใช้บวกเพิ่มที่ตัวแปร minustime
                gettime(&stime);
                f=LoadFont(Form1->ComboBox1->Text,Form1->CheckBox1->Checked,Form1->CheckBox2->Checked,c[i][0]->down-c[i][0]->up+1,Form1->Image4);
                gettime(&etime);
                minustime+=
                (int(etime.ti_hour-stime.ti_hour)*3600)+
                (int(etime.ti_min -stime.ti_min )*60)+
                 int(etime.ti_sec -stime.ti_sec );

                Form1->ProgressBar2->Position=0;
                allsize[c[i][0]->down-c[i][0]->up+1]=f;
                usesize[c[i][0]->down-c[i][0]->up+1]=true;

            }
        }
        else
        {
            Form1->Label1->Caption = "Loading Font";       //คุง
            Form1->Label1->Refresh();
            //แทรกส่วนจับเวลา ใช้บวกเพิ่มที่ตัวแปร minustime
            gettime(&stime);
            f=LoadFont(Form1->ComboBox1->Text,false,false,c[i][0]->down-c[i][0]->up+1,Form1->Image4);
            gettime(&etime);
            minustime+=
            (int(etime.ti_hour-stime.ti_hour)*3600)+
            (int(etime.ti_min -stime.ti_min )*60)+
             int(etime.ti_sec -stime.ti_sec );

            Form1->ProgressBar2->Position=0;
            newmax=c[i][0]->down-c[i][0]->up+1;
            usesize=(bool *)realloc(usesize,sizeof(bool)*(newmax+1));
            allsize=(myfont **)realloc(allsize,sizeof(myfont *)*(newmax+1));
            allsize[newmax]=f;
            for(j=maxsize;j<newmax;j++)
            {
                usesize[j]=false;
            }
            usesize[newmax]=true;
            maxsize=newmax+1;
        }
        /*
        if(c[i][0]->down-c[i][0]->up+1!=last)
        {
            if(last!=0)
                FreeFont(f);
            f=LoadFont(Form1->ComboBox1->Text,false,false,c[i][0]->down-c[i][0]->up+1,Form1->Image4);
            */
            /*
            for(j=0;j<10;j++)
            {
                SaveArray(f->test[j]->z->a,f->test[j]->z->h,f->test[j]->z->w,"C:\\new\\"+IntToStr(j)+".bmp");
            }
            */
            /*
            last=c[i][0]->down-c[i][0]->up+1;
        }
        */
        gap[i]=f->gap;
        //fprintf(r,"Row=%d, Height=%d, Size=%d\n",i,f->h,f->size);
        min=10000;
        for(j=0;j<num[i];j++)
        {
            Form1->Label1->Caption = "Changing to text ( "+IntToStr(cpro)+" of "+IntToStr(tpro)+" )";          //คุงแก้
            Form1->Label1->Refresh();
            //clear point
            if(c[i][j]->type!=0)
            {
                for(k=0;k<all;k++)
                {
                    point[i]=0;
                }

                if(c[i][j]->type==1 )   //level1
                {
                    point[216]=1;
                    point[217]=1;
                    point[262]=1;
                    point[264]=1;
                }
                else if(c[i][j]->type==2)   //level2
                {
                    for(k=0;k<all;k++)
                    {
                        point[k]=1;
                    }
                }
                else if(c[i][j]->type==3)   //level3
                {
                    point[209]=1;
                    for(k=212;k<=215;k++)
                    {
                       point[k]=1;
                    }
                    for(k=231;k<=237;k++)
                    {
                       point[k]=1;
                    }
                }
                else if(c[i][j]->type==4)   //level4
                {
                    for(k=256;k<=260;k++)
                    {
                       point[k]=1;
                    }
                }
                if(c[i][j]->thin->node_num>0)
                {
                    n=CompareSize1(point,c[i][j],f);   //กรองขั้นแรก
                    if(n>1)
                    {
                        n=CompareSize2(point,c[i][j],f);  //กรองขั้นที่ 2 //ถ้าไม่ใช้ comment ได้เลย
                    }
                }
                else    //สำหรับพวกที่เป็นวง วงเดียว ไม่มีจุดปลายหรือทางแยก
                {
                    n=0;
                    for(k=0;k<all;k++)
                    {
                        if(point[k]==1 && f->have[k]==1)
                        {
                            if  (f->ascii[k]->thin->node_num==0)
                            {
                                n++;
                            }
                            else
                            {
                                point[k]=0;
                            }
                        }
                        else
                        {
                            point[k]=0;
                        }
                    }
                    if(n>1)
                    {
                        n=CompareSize1(point,c[i][j],f);   //กรองอีกนิดด้วยขนาด
                    }
                }
                //---------debug--------จะบอกตัวที่เหลือจากการถูกคัดกรองสองรอบ


                fprintf(r,"%s\t%s\t->%d\t",IntToStr(i),IntToStr(j),n);
                for(k=0;k<all;k++)
                {
                    if(point[k]==1)
                    {
                        fprintf(r," %c \t",f->ascii[k]->c[1]);
                    }
                }
                fprintf(r,"\n");
                count++;

                //---------end-debug---------------------
                if(n>=1)
                {
                    min=10000;
                    for(k=0;k<all;k++)
                    {
                        if(point[k]==1)
                        {
                            if(n>1)
                            {
                                if(f->have[k]>1)   //ตัวมาจาก font ที่มีหลายชิ้น
                                {
                                    g=distance2(c[i][j],f->ascii[k]);
                                }
                                else if(c[i][j]->thin->node_num==0 || f->ascii[k]->thin->node_num)
                                {
                                    g=distance2(c[i][j],f->ascii[k]);
                                }
                                else if(c[i][j]->thin->node_num>0 && f->ascii[k]->thin->node_num>0)
                                {
                                    //g=ElasticMatching(c[i][j]->thin,f->ascii[k]->thin);
                                    x=MakePath2(c[i][j]->thin);
                                    y=MakePath2(f->ascii[k]->thin);
                                    for(l=0;l<x->d;l++)
                                    {
                                        x->note[l][0]-=c[i][j]->up;
                                    }
                                    for(l=0;l<y->d;l++)
                                    {
                                        y->note[l][0]-=f->ascii[k]->up;
                                    }
                                    g=DTW(x,y,&d);
                                    g=g/d;
                                    FreeLink(x);
                                    FreeLink(y);
                                }

                                if(g<min)
                                {
                                    min=g;
                                    answer=k;
                                }
                            }
                            else if(n==1)
                            {
                                answer=k;
                                break;
                            }
                        }
                    }
                    if(!(c[i][j]->type==3 && c[i][j]->c[1]=='ื'))
                    {
                        c[i][j]->c[1]=f->ascii[answer]->c[1];
                        c[i][j]->ans=answer;
                    }
                }
                else if(n==0)   ///ไม่มีคำตอบ น่าจะเป็นตัวติดกัน
                {
                    c[i][j]->c[1]='\0';
                    c[i][j]->ans=-1;

                    //ถ้าไม่มีคำตอบจะใช้ HD มาคิดแทน เพราะตัวที่ไม่มีมักจะเกิดจากสร้างลำดับผิด

                    if(c[i][j]->type==1 )   //level1
                    {
                        point[216]=1;
                        point[217]=1;
                        point[262]=1;
                        point[264]=1;
                    }
                    else if(c[i][j]->type==2)   //level2
                    {
                        for(k=0;k<all;k++)
                        {
                            point[k]=1;
                        }
                    }
                    else if(c[i][j]->type==3)   //level3
                    {
                        point[209]=1;
                        for(k=212;k<=215;k++)
                        {
                           point[k]=1;
                        }
                        for(k=231;k<=237;k++)
                        {
                           point[k]=1;
                        }
                    }
                    else if(c[i][j]->type==4)   //level4
                    {
                        for(k=256;k<=260;k++)
                        {
                           point[k]=1;
                        }
                    }
                    n=CompareSize1(point,c[i][j],f);   //กรองขั้นแรก
                    if(n>0)
                    {
                        min=10000;
                        for(k=0;k<all;k++)
                        {
                            if(point[k]==1)
                            {
                                g=distance2(c[i][j],f->ascii[k]);
                                if(g<min)
                                {
                                    min=g;
                                    answer=k;
                                }
                            }
                        }
                        c[i][j]->c[1]=f->ascii[answer]->c[1];
                        c[i][j]->ans=answer;
                    }
                }

            }
            Form1->ProgressBar1->Position=lpro+int(cpro*65/tpro); //คุง
            Form1->ProgressBar1->Refresh();
            cpro++;
        }
    }
    //FreeFont(f);
    for(i=0;i<maxsize;i++)
    {
        if(usesize[i])
        {
            FreeFont(allsize[i]);
        }
    }
    free(allsize);
    free(usesize);
     //ดูแบบดิบ ๆ
    r=fopen("C:\\raw.txt","w");
    for(i=0;i<*row;i++)
    {
        for(j=0;j<num[i];j++)
        {
            if(c[i][j]->c[1]=='\0')
                fprintf(r,"-\n");
            else
                fprintf(r,"%c\n",c[i][j]->c[1]);
        }
    }
    fclose(r);

    //fclose(r2);
    fclose(r);
    //หาคำตอบเรียบร้อย******************************************************
}
void ViewInFile(picar *z)
{
    FILE *f;
    int i,j;
    f=fopen("C:\\ViewInFile.txt","w");
    for(i=0;i<z->h;i++)
    {
        for(j=0;j<z->w;j++)
        {
            fprintf(f,"%d\t",z->a[i][j]);
        }
        fprintf(f,"\n");
    }
    fclose(f);
}
float euclidian(int i1,int i2,int j1,int j2)
{
    return sqrt( ((i1-i2)*(i1-i2)) + ((j1-j2)*(j1-j2)) );
}
float DTW(link *m,link *n,int *k)
{
    //สร้างตารางก่อน
    float **a,**b,min;
    int i,j,count;
    a=(float **)malloc(sizeof(float *)*m->d);
    b=(float **)malloc(sizeof(float *)*m->d);
    for(i=0;i<m->d;i++)
    {
        a[i]=(float *)malloc(sizeof(float)*n->d);
        b[i]=(float *)malloc(sizeof(float)*n->d);

        for(j=0;j<n->d;j++)
        {
            a[i][j]=euclidian(m->note[i][0],n->note[j][0],m->note[i][1],n->note[j][1]);
            min=-1;
            if(i-1>=0)
            {
                if(b[i-1][j]<min || min==-1)
                {
                    min=b[i-1][j];
                }
            }
            if(j-1>=0)
            {
                if(b[i][j-1]<min || min==-1)
                {
                    min=b[i][j-1];
                }
            }
            if(i-1>=0 && j-1>=0)
            {
                if(b[i-1][j-1]<min || min==-1)
                {
                    min=b[i-1][j-1];
                }
            }

            if(min==-1)
                min=0;

            b[i][j]=a[i][j]+min;
        }
    }
    i=m->d-1;
    j=n->d-1;
    count=0;
    while(true)
    {
        count++;
        if(i-1>=0 && j-1>=0)
        {
            if(b[i-1][j-1]<b[i-1][j] && b[i-1][j-1]<b[i][j-1])
            {
                i--;
                j--;
            }
            else if(b[i-1][j]<b[i][j-1])
            {
                i--;
            }
            else
            {
                j--;
            }
        }
        else if(i-1>=0)
        {
            i--;
        }
        else if(j-1>=0)
        {
            j--;
        }
        else
        {
            break;
        }
    }
    *k=count;
    min=b[m->d-1][n->d-1];

    for(i=0;i<m->d;i++)
    {
        free(a[i]);
        free(b[i]);
    }
    free(a);
    free(b);
    return min;
}

link *MakeChain(Graphics::TBitmap *bitmap) //ทำให้กุ้งใช้!!!
{
    picar *z,*x;
    thindata *t;
    link *r=NULL;
    t=(thindata *)malloc(sizeof(thindata));
    z=PicToArray256(bitmap);
    GrayToMono(z,150);

    x=skeleton(z);
    t->z=thining(x);

    PreChain2(t);
    if(t->node_num>0)
    {
        FindLongestMain2(t);

        r=MakePath2(t);     //<---แก้ไขได้
                        //1=ไม่เก็บกิ่ง
                        //2=เก็บกิ่งแล้วโดดกลับ (เส้นจะขาดไปบ้างตรงที่โดดกลับ)
                        //3=เก็บกิ่งแล้วเดินซ้ำกิ่งกลับ (เส้นจะต่อเนื่องกันตลอด)
    }
    FreeThindata(t);

    FreePicar(z);
    FreePicar(x);
    return r;
}

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
        try
        {
              if(OpenPictureDialog1->Execute())
              {
                  bitmap1->LoadFromFile(OpenPictureDialog1->FileName);
                  Image1->Canvas->StretchDraw(Rect1,bitmap1);
              }
        }
        catch(...)
        {
              ShowMessage("Cannot open Image");
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    for(int i = 0; i < Screen->Fonts->Count; i++)
        ComboBox1->Items->Add(Screen->Fonts->Strings[i]);

    bitmap1 = new Graphics::TBitmap;
    bitmap2 = new Graphics::TBitmap;
    Rect1 = Rect(0,0,Image1->Width,Image1->Height);
    Rect2 = Rect(0,0,Image2->Width,Image2->Height);
    Rect3 = Rect(0,0,Image3->Width,Image3->Height);
    Rect5 = Rect(0,0,Image5->Width,Image5->Height);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
    Form1->ProgressBar1->Position = 0;
    struct time stime,etime;
    minustime=0;
    gettime(&stime);              //เริ่มจับเวลา

    picar *array,*ro,**d;
    int i,j,k,h,w,last=0;
    int *num,*row,*gap;
    float deg;

    array=PicToArray256(bitmap1);
    if(array==NULL)
    {
        ShowMessage("Can't malloc");
    }
    h=array->h;
    w=array->w;

    if(array->type==2)
    {
        Label1->Caption = "Changing image to monochrome";      //คุงแก้
        Label1->Refresh();
        GrayToMono(array,150);     //********สำคัญมาก*** ค่า threshold *****
    }
    ProgressBar1->StepBy(3);            //คุงแก้
    ProgressBar1->Refresh();
    CheckBlackWhite(array);

    Label1->Caption = "Deleting Noise";          //คุงแก้
    Label1->Refresh();
    DelNoiseHis(array);
    ProgressBar1->StepBy(3);            //คุงแก้
    ProgressBar1->Refresh();
    //ShowImage(array,Image2,Rect2);    //ถ้าใส่แล้วมีแถบขาว

    Label1->Caption = "Rotating image";          //คุงแก้
    Label1->Refresh();
    deg=FindRotate(array);          // มี step อยู่ในนี้อีก 2+2
    //Edit1->Text=FloatToStr(deg);

    ro=RotateArrayBig(array,deg);
    //ro=array;
    ProgressBar1->StepBy(1);            //คุงแก้
    ProgressBar1->Refresh();

    SavePicar(ro,"C:\\pic.bmp");
    //for(i=0;i<StrToInt(Edit1->Text);i++)
    //    Del1Noise(array);
    //dilation(array);
    //ShowImage(array,Image2,Rect2);
    //erosion(array);

    //DelNoiseHis(ro);
    //ShowImage(ro,Image2,Rect2);

    AnsiString str;
    //onerow **bb;
    paragraph *para;
    alpha ***c;
    Label1->Caption = "Finding alphabet";          //คุงแก้
    Label1->Refresh();
    para=GetAlpha2(ro);
    ProgressBar1->StepBy(10);            //คุงแก้
    ProgressBar1->Refresh();
    c  =para->c;
    row=para->row;
    num=para->num;
    free(para);
    gap=(int *)malloc(sizeof(int)*(*row));
    /*
    row=(int *)malloc(sizeof(int));
    bb=DivideRow2(ro,row);      // row จะเก็บไว้ว่าแบ่งได้เท่าไหร่


    Edit1->Text=IntToStr(*row);

    for(i=0;i<*row;i++)
    {
        str="C:\\testpic2\\pic"+IntToStr(i)+".bmp";
        SaveArray(bb[i]->z->a,bb[i]->z->h,bb[i]->z->w,str);
    }

    c=(alpha ***)malloc(sizeof(alpha **)*(*row));
    num=(int *)malloc(sizeof(int)*(*row));
    gap=(int *)malloc(sizeof(int)*(*row));
    for(i=0;i<*row;i++)
    {
            c[i]=GetAlpha(bb[i],&num[i]); //bb จะเก็บภาพที่ตัดแล้ว
                                            //num จะเก็บจำนวนอักขระในแต่ละบรรทัด
    }
    */

    //save alpha pic  //debug point

    /*
    for(i=0;i<*row;i++)
    {
        for(j=0;j<num[i];j++)
        {

                str="C:\\testpic2\\"+IntToStr(i)+","+IntToStr(j)+","+IntToStr(c[i][j]->up)+","+IntToStr(c[i][j]->down)+".bmp";
            */
            /*
            else if(c[i][j]->type==0)
            {
                fopen("C:\\testpic\\alpha"+IntToStr(i)+","+IntToStr(j)+",0.ent",w);
                continue;
            }
            */
            /*
            if(c[i][j]->type!=0)
                SavePicar(c[i][j]->z,str);
        }
    }
    */
    //ทำ thindata
    picar *sk;
    int tpro=0,lpro,cpro=1;
    lpro=ProgressBar1->Position;
    for(i=0;i<*row;i++)
    {
        tpro+=num[i];
    }
    for(i=0;i<*row;i++)
    {
        for(j=0;j<num[i];j++)
        {
            Label1->Caption = "Thining Alphabet ( "+IntToStr(cpro)+" of "+IntToStr(tpro)+" )";          //คุงแก้
            Label1->Refresh();
            /*
            if(cpro==608)     //debug in angsana 9th tasecase
            {
                cpro=cpro;
                SavePicar(c[i][j]->z,"C://bugging"+IntToStr(i)+","+IntToStr(j)+".bmp");
            }
            */
            if(c[i][j]->type!=0)
            {
                c[i][j]->thin=(thindata *)malloc(sizeof(thindata));

                sk=skeleton(c[i][j]->z);
                c[i][j]->thin->z=thining(sk);
                FreePicar(sk);

                //SavePicar(c[i][j]->thin->z,"c://skeleton//"+IntToStr(i)+","+IntToStr(j)+".bmp");

                //c[i][j]->thin->z=thining(c[i][j]->z);

                PreChain2(c[i][j]->thin);

                if(c[i][j]->thin->node_num>0)
                {
                    FindLongestMain2(c[i][j]->thin);
                }
            }
            ProgressBar1->Position=lpro+int(cpro*10/tpro); //คุง
            ProgressBar1->Refresh();
            cpro++;
        }
    }
    //หาคำตอบ                          //##########################################################
    //FindAnswer1(bb,c,row,num,gap);     //จะใช้ HousDorff Distance###############################
    FindAnswer2(c,row,num,gap);     //จะใช้ DTW###########################
    //ประกอบกันให้เป็นเอกสารที่สมบูรณ์
    FILE *r3;
    char **text;
    int *oldnum;
    oldnum=(int *)malloc(sizeof(int)*(*row));
    for(i=0;i<*row;i++)
    {
        oldnum[i]=num[i];
    }
    Label1->Caption = "Making document";
    Label1->Refresh();
    r3=fopen("C:\\answer.txt","w");
    text=MakeDoc(c,*row,num,gap);
    ProgressBar1->StepBy(3);            //คุงแก้
    ProgressBar1->Refresh();
    for(i=0;i<*row;i++)
    {
        fprintf(r3,"%s\n",text[i]);
    }
    fclose(r3);

    //write in memo1
    AnsiString mem1="";
    int ccc;
    Memo1->Text = "";
    for(i=0;i<*row;i++)
    {
        ccc=0;
        mem1="";
        while(1)
        {
            if(text[i][ccc]=='\0')
                break;
            else
                mem1+=text[i][ccc];
            ccc++;
        }
        Memo1->Lines->Add(mem1);
    }

    //free memory
    for(i=0;i<*row;i++)
    {
        free(text[i]);
    }
    free(text);
    for(i=0;i<*row;i++)
    {
        for(j=0;j<oldnum[i];j++)
        {
            if(c[i][j]->type!=0)
            {
                FreePicArray((void **)(c[i][j]->z->a),c[i][j]->z->h);
                FreeThindata(c[i][j]->thin);
            }
            free(c[i][j]);
        }
        free(c[i]);
    }
    free(c);
    /*
    for(i=0;i<*row;i++)
    {
        free(bb[i]);
    }
    free(bb);
    */


    free(row);
    free(num);
    free(oldnum);
    FreePicar(ro);

    gettime(&etime);         //หยุดจับเวลา
    int ta,tb,tc,tsec;
    ta=int(etime.ti_hour-stime.ti_hour);
    tb=int(etime.ti_min -stime.ti_min );
    tc=int(etime.ti_sec -stime.ti_sec );
    tsec=(ta*3600)+(tb*60)+tc;
    Edit3->Text=IntToStr(tsec);
    Edit4->Text=IntToStr(tsec-minustime);
    /*
    ta=tsec/3600;
    tb=(tsec%3600)/60;
    tc=tsec%60;
    Edit3->Text=
    IntToStr(ta)+" : "+
    IntToStr(tb)+" : "+
    IntToStr(tc);
    */
    ProgressBar1->Position = 100;
    Label1->Caption = "Complete";
}

//---------------------------------------------------------------------------


void __fastcall TForm1::Button4Click(TObject *Sender)
{
    myfont *f;
    f=LoadFont(ComboBox1->Text,false,false,20,Image4);

    FreeFont(f);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
    picar *z,*r,*s,*p;
    int i,j,k,h,w;
    thindata *t;
    link *u;
    t=(thindata *)malloc(sizeof(thindata));
    z=PicToArray256(bitmap1);
    GrayToMono(z,150);

    r=skeleton(z);
    t->z=thining(r);
    //t->z=thining(z);
    //FreePicar(r);
    //ShowImage(   r,Image2,Rect2);
    //ShowImage(   s,Image3,Rect3);
    //ShowImage(   p,Image5,Rect5);
    //SavePicar(r,"C:\\skeleton.bmp");
    //SavePicar(s,"C:\\thining.bmp");
    SavePicar(t->z,"C:\\plus.bmp");
    //ShowImage(r   ,Image3,Rect3);
    ShowImage(t->z,Image5,Rect5);
    PreChain2(t);

    //ViewInFile(t->z);

    FindLongestMain2(t);

    Edit1->Text=IntToStr(t->path_long);
    //r=DrawPath(t);
    //ShowImage(r,Image3,Rect3);

    //u=MakePath3(t);

    //r=NewPicar(z->h,z->w,1);
    /*
    for(i=0;i<u->d;i++)
    {
        r->a[u->note[i][0]] [u->note[i][1]]=1;
        SavePicar(r,"C://motion//"+IntToStr(i)+".bmp");
    }
    */

    FreeThindata(t);
    FreePicar(z);
    FreePicar(r);
    FreeLink(u);

    //FreePicar(s);
    //FreePicar(p);

}
//---------------------------------------------------------------------------



void __fastcall TForm1::Button3Click(TObject *Sender)
{
    link *f;
    picar *r;
    int i;
    f=MakeChain(bitmap1);
    r=NewPicar(bitmap1->Height,bitmap1->Width,1);
    for(i=0;i<f->d;i++)
    {
        r->a[f->note[i][0]] [f->note[i][1]]=1;
        SavePicar(r,"C://motion//"+IntToStr(i)+".bmp");
    }
    FreeLink(f);
    FreePicar(r);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
    picar *array;
    array=PicToArray256(bitmap1);
    ShowImage(array,Image2,Rect2);
    SavePicar(array,"C://pic.bmp");
    if(array->type==2)
    {
        //GrayToMono(array,50);
        adaptive(array,3,2); //row and colum
    }
    SavePicar(array,"C://pic2.bmp");
    ShowImage(array,Image3,Rect3);
    

    FreePicar(array);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button7Click(TObject *Sender)
{
    if (SaveDialog1->Execute())
    {
        Memo1->Lines->SaveToFile(SaveDialog1->FileName);
    }
}
//---------------------------------------------------------------------------

