#include "DxLib.h"
#include <stdio.h>
#include <math.h>
#include <string>
using namespace std;

void loadg();

//String 使用

//プログラム中
//main-10
//タイトル-100
int main=100,maintm=0;

//ステージ
int stagecolor=0;
int sta=1,stb=4,stc=0;

//クイック
int fast=1;

//トラップ表示
int trap=1;

//中間ゲート
int tyuukan=0;


//スタッフロール
int ending=0;


//ステージ読み込みループ(いじらない)
int stagerr,stagepoint;
//オーバーフローさせる
int over=0;

//ステージスイッチ
int stageonoff=0;


//メインプログラム
void Mainprogram();
void rpaint();
int maint;


//サブクラス
//(ウエイト系
void wait(int interval);
void wait2(long stime, long etime,int FLAME_TIME);
int rand(int Rand);
void end();

//描画
int color;
void setfont(int a);
void setcolor(int red, int green, int blue);
void setc0();
void setc1();
void drawpixel(int a,int b);
void drawline(int a,int b,int c,int d);
void drawrect(int a,int b,int c,int d);
void fillrect(int a,int b,int c,int d);
void drawarc(int a,int b,int c,int d);
void fillarc(int a,int b,int c,int d);
int grap[161][8],mgrap[51];
int loadimage(string b);
int loadimage(int a,int x,int y,int r,int z);
int mirror;
void drawimage(int mx,int a,int b);
void drawimage(int mx,int a,int b,int c,int d,int e,int f);
void setre();
void setre2();
void setno();
int oto[151];
void ot(int x);void bgmchange(int x);

//文字
void str(string c,int a,int b);


//)

void stagecls();
void stage();
void stagep();





//1-ステージ
//10-ステージ前
//



//ループ
int t,tt,t1,t2,t3,t4;


//初期化
int zxon,zzxon;

//キーコンフィグ
int key,keytm;

//三角関数
double pai=3.1415926535;


//地面
#define smax 31
int sx,sco;
int sa[smax],sb[smax],sc[smax],sd[smax],stype[smax],sxtype[smax],sr[smax];
int sgtype[smax];



//プレイヤー
int mainmsgtype;
int ma,mb,mnobia,mnobib,mhp;
int mc,md,macttype,atkon,atktm,mactsok,msstar,nokori=2,mactp,mact;

int mtype,mxtype,mtm,mzz;
int mzimen,mrzimen,mkasok,mmuki,mmukitm,mjumptm,mkeytm,mcleartm;
int mmutekitm,mmutekion;
int mztm,mztype;
int actaon[7];
//メッセージ
int mmsgtm,mmsgtype;

int mascrollmax=21000;//9000




//ブロック
void tyobi(int x,int y,int type);
void brockbreak(int t);
#define tmax 641
int tco;
int ta[tmax],tb[tmax],tc[tmax],td[tmax],thp[tmax],ttype[tmax];
int titem[tmax],txtype[tmax];

//メッセージブロック
int tmsgtm,tmsgtype,tmsgx,tmsgy,tmsgnobix,tmsgnobiy,tmsg;
void ttmsg();void txmsg(string x,int a);
void setfont(int x,int y);

//効果を持たないグラ
void eyobi(int xa,int xb,int xc,int xd,int xe,int xf,int xnobia,int xnobib,int xgtype,int xtm);
#define emax 201
int eco;
int ea[emax],eb[emax],enobia[emax],enobib[emax],ec[emax],ed[emax];
int ee[emax],ef[emax],etm[emax];
int egtype[emax];



//敵キャラ
void ayobi(int xa,int xb,int xc,int xd,int xnotm,int xtype,int xxtype);
void tekizimen();
#define amax 24
int aco;
int aa[amax],ab[amax],anobia[amax],anobib[amax],ac[amax],ad[amax];
int ae[amax],af[amax],abrocktm[amax];
int aacta[amax],aactb[amax],azimentype[amax],axzimen[amax];
int atype[amax],axtype[amax],amuki[amax],ahp[amax];
int anotm[amax],anx[160],any[160];
int atm[amax],a2tm[amax];
int amsgtm[amax],amsgtype[amax];

//敵出現
#define bmax 81
int bco;
int ba[bmax],bb[bmax],btm[bmax];
int btype[bmax],bxtype[bmax],bz[bmax];


//背景
#define nmax 41
int nxxmax,nco;
int na[nmax],nb[nmax],nc[nmax],nd[nmax],ntype[nmax];
int ne[nmax],nf[nmax],ng[nmax],nx[nmax];


//リフト
#define srmax 21
int srco;
int sra[srmax],srb[srmax],src[srmax],srd[srmax],sre[srmax],srf[srmax];
int srtype[srmax],srgtype[srmax],sracttype[srmax],srsp[srmax];
int srmuki[srmax],sron[srmax],sree[srmax];
int srsok[srmax],srmovep[srmax],srmove[srmax];





//スクロール範囲
int fx=0,fy=0,fzx,fzy,scrollx,scrolly;
//全体のポイント
int fma=0,fmb=0;
//強制スクロール
int kscroll=0;
//画面サイズ(ファミコンサイズ×2)(256-224)
int fxmax=48000,fymax=42000;



//ステージ
byte stagedate[17][2001];

//画面黒
int blacktm=1,blackx=0;



//自由な値
int xx[91];
double xd[11];
string xs[31];


//タイマー測定
long stime;