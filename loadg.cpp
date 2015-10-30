#include "lib.h"

extern int ma, t, tt;
extern int grap[161][8], mgrap[51];
extern int oto[151];

extern int anx[160], any[160];
extern int ne[40], nf[40];

void loadg(void) {

  for (t = 0; t < 7; t++) {
    mgrap[t] = 0;
  }

  //画像読み込み

  //プレイヤー
  mgrap[0] = loadimage("res/player.png");
  //ブロック
  mgrap[1] = loadimage("res/brock.png");
  //アイテム
  mgrap[2] = loadimage("res/item.png");
  //敵
  mgrap[3] = loadimage("res/teki.png");
  //背景
  mgrap[4] = loadimage("res/haikei.png");
  //ブロック2
  mgrap[5] = loadimage("res/brock2.png");
  //おまけ
  mgrap[6] = loadimage("res/omake.png");
  //おまけ2
  mgrap[7] = loadimage("res/omake2.png");
  //タイトル
  mgrap[30] = loadimage("res/syobon3.png");


  //プレイヤー読み込み
  grap[40][0] = subimage(0, 0, 30, 36, mgrap[0]);
  grap[0][0] = subimage(31 * 4, 0, 30, 36, mgrap[0]);
  grap[1][0] = subimage(31 * 1, 0, 30, 36, mgrap[0]);
  grap[2][0] = subimage(31 * 2, 0, 30, 36, mgrap[0]);
  grap[3][0] = subimage(31 * 3, 0, 30, 36, mgrap[0]);
  grap[41][0] = subimage(50, 0, 51, 73, mgrap[6]);

  //ブロック読み込み
  for (t = 0; t <= 6; t++) {
    grap[t][1] = subimage(33 * t, 0, 30, 30, mgrap[1]);
    grap[t + 30][1] = subimage(33 * t, 33, 30, 30, mgrap[1]);
    grap[t + 60][1] = subimage(33 * t, 66, 30, 30, mgrap[1]);
  }
  grap[8][1] = subimage(33 * 7, 0, 30, 30, mgrap[1]);
  grap[16][1] = subimage(33 * 6, 0, 24, 27, mgrap[2]);
  grap[10][1] = subimage(33 * 9, 0, 30, 30, mgrap[1]);
  grap[40][1] = subimage(33 * 9, 33, 30, 30, mgrap[1]);
  grap[70][1] = subimage(33 * 9, 66, 30, 30, mgrap[1]);

  //ブロック読み込み2
  for (t = 0; t <= 6; t++) {
    grap[t][5] = subimage(33 * t, 0, 30, 30, mgrap[5]);
  }
  grap[10][5] = subimage(33 * 1, 33, 30, 30, mgrap[5]);
  grap[11][5] = subimage(33 * 2, 33, 30, 30, mgrap[5]);
  grap[12][5] = subimage(33 * 0, 66, 30, 30, mgrap[5]);
  grap[13][5] = subimage(33 * 1, 66, 30, 30, mgrap[5]);
  grap[14][5] = subimage(33 * 2, 66, 30, 30, mgrap[5]);

  //アイテム読み込み
  for (t = 0; t <= 5; t++) {
    grap[t][2] = subimage(33 * t, 0, 30, 30, mgrap[2]);
  }

  //敵キャラ読み込み
  grap[0][3] = subimage(33 * 0, 0, 30, 30, mgrap[3]);
  grap[1][3] = subimage(33 * 1, 0, 30, 43, mgrap[3]);
  grap[2][3] = subimage(33 * 2, 0, 30, 30, mgrap[3]);
  grap[3][3] = subimage(33 * 3, 0, 30, 44, mgrap[3]);
  grap[4][3] = subimage(33 * 4, 0, 33, 35, mgrap[3]);
  grap[5][3] = subimage(0, 0, 37, 55, mgrap[7]);
  grap[6][3] = subimage(38 * 2, 0, 36, 50, mgrap[7]);
  grap[150][3] = subimage(38 * 2 + 37 * 2, 0, 36, 50, mgrap[7]);
  grap[7][3] = subimage(33 * 6 + 1, 0, 32, 32, mgrap[3]);
  grap[8][3] = subimage(38 * 2 + 37 * 3, 0, 37, 47, mgrap[7]);
  grap[151][3] = subimage(38 * 3 + 37 * 3, 0, 37, 47, mgrap[7]);
  grap[9][3] = subimage(33 * 7 + 1, 0, 26, 30, mgrap[3]);
  grap[10][3] = subimage(214, 0, 46, 16, mgrap[6]);

  //モララー
  grap[30][3] = subimage(0, 56, 30, 36, mgrap[7]);
  grap[155][3] = subimage(31 * 3, 56, 30, 36, mgrap[7]);
  grap[31][3] = subimage(50, 74, 49, 79, mgrap[6]);


  grap[80][3] = subimage(151, 31, 70, 40, mgrap[4]);
  grap[81][3] = subimage(151, 72, 70, 40, mgrap[4]);
  grap[130][3] = subimage(151 + 71, 72, 70, 40, mgrap[4]);
  grap[82][3] = subimage(33 * 1, 0, 30, 30, mgrap[5]);
  grap[83][3] = subimage(0, 0, 49, 48, mgrap[6]);
  grap[84][3] = subimage(33 * 5 + 1, 0, 30, 30, mgrap[3]);
  grap[86][3] = subimage(102, 66, 49, 59, mgrap[6]);
  grap[152][3] = subimage(152, 66, 49, 59, mgrap[6]);

  grap[90][3] = subimage(102, 0, 64, 63, mgrap[6]);

  grap[100][3] = subimage(33 * 1, 0, 30, 30, mgrap[2]);
  grap[101][3] = subimage(33 * 7, 0, 30, 30, mgrap[2]);
  grap[102][3] = subimage(33 * 3, 0, 30, 30, mgrap[2]);

  //grap[104][3] = subimage( 33*2, 0, 30, 30, mgrap[5]) ;
  grap[105][3] = subimage(33 * 5, 0, 30, 30, mgrap[2]);
  grap[110][3] = subimage(33 * 4, 0, 30, 30, mgrap[2]);


  //背景読み込み
  grap[0][4] = subimage(0, 0, 150, 90, mgrap[4]);
  grap[1][4] = subimage(151, 0, 65, 29, mgrap[4]);
  grap[2][4] = subimage(151, 31, 70, 40, mgrap[4]);
  grap[3][4] = subimage(0, 91, 100, 90, mgrap[4]);
  grap[4][4] = subimage(151, 113, 51, 29, mgrap[4]);
  grap[5][4] = subimage(222, 0, 28, 60, mgrap[4]);
  grap[6][4] = subimage(151, 143, 90, 40, mgrap[4]);

  //中間フラグ
  grap[20][4] = subimage(40, 182, 40, 60, mgrap[4]);


  //グラ
  grap[0][5] = subimage(167, 0, 45, 45, mgrap[6]);









  //敵サイズ収得
  //int GrHandle=0;
  for (t = 0; t <= 140; t++) {
    getimagesize(grap[t][3], &anx[t], &any[t]);
    anx[t] *= 100; any[t] *= 100;
  }
  anx[79] = 120 * 100; any[79] = 15 * 100;
  anx[85] = 25 * 100; any[85] = 30 * 10 * 100;

  //背景サイズ収得
  for (t = 0; t < 40; t++) {
    getimagesize(grap[t][4], &ne[t], &nf[t]);
    //ne[t]*=100;nf[t]*=100;
  }

  /*
     anx[0]=30;any[0]=30;
     anx[1]=30;any[1]=43;
     anx[2]=30;any[2]=30;
     anx[3]=30;any[3]=44;
     */




  /*
  SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
  oto[1] = LoadSoundMem("se/jump.mp3");
  //oto[2] = LoadSoundMem("se/brockcoin.mp3");
  oto[3] = LoadSoundMem("se/brockbreak.mp3");
  oto[4] = LoadSoundMem("se/coin.mp3");
  oto[5] = LoadSoundMem("se/humi.mp3");
  oto[6] = LoadSoundMem("se/koura.mp3");
  oto[7] = LoadSoundMem("se/dokan.mp3");
  oto[8] = LoadSoundMem("se/brockkinoko.mp3");
  oto[9] = LoadSoundMem("se/powerup.mp3");
  oto[10] = LoadSoundMem("se/kirra.mp3");
  oto[11] = LoadSoundMem("se/goal.mp3");
  oto[12] = LoadSoundMem("se/death.mp3");
  oto[13] = LoadSoundMem("se/Pswitch.mp3");
  oto[14] = LoadSoundMem("se/jumpBlock.mp3");
  oto[15] = LoadSoundMem("se/hintBlock.mp3");
  oto[16] = LoadSoundMem("se/4-clear.mp3");
  oto[17] = LoadSoundMem("se/allclear.mp3");
  oto[18] = LoadSoundMem("se/tekifire.mp3");

  //}catch( int num){end();}


  ChangeVolumeSoundMem(255 * 40 / 100, oto[103]);
*/

  //ループ設定-20000-20秒
  //SetLoopPosSoundMem( 1,oto[104]) ;
  //SetLoopSamplePosSoundMem(44100,oto[104]);
  //SetLoopSamplePosSoundMem(22050,oto[104]);
}
