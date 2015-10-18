#include "dxlib.h"

// グラフィックに設定する透過色をセットする
int SetTransColor(int Red, int Green, int Blue) {
  return 0;
}

// 画像ファイルのメモリへの読みこみ
int LoadGraph(const char *FileName, int NotUse3DFlag) {
  return 0;
}

// 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックハンドルを作成する
int DerivationGraph(int SrcX, int SrcY, int Width, int Height, int SrcGraphHandle) {
  return 0;
}

// グラフィックのサイズを得る
int GetGraphSize(int GrHandle, int *SizeXBuf, int *SizeYBuf) {
  return 0;
}

// 作成するサウンドのデータ形式を設定する( DX_SOUNDDATATYPE_MEMNOPRESS 等 )
int SetCreateSoundDataType(int SoundDataType) {
  return 0;
}

// サウンドデータを追加する
int LoadSoundMem(const char *WaveName, int BufferNum, int UnionHandle) {
  return 0;
}

// メモリに読みこんだWAVEデータの再生にボリュームを設定する( パーセント指定 )
int ChangeVolumeSoundMem(int VolumePal, int SoundHandle) {
  return 0;
}

// 画面モードを設定する
int SetGraphMode(int ScreenSizeX, int ScreenSizeY, int ColorBitDepth, int RefreshRate) {
  return 0;
}

// ライブラリ初期化関数
int DxLib_Init(void) {
  return 0;
}

// ライブラリ使用の終了関数
int DxLib_End(void) {
  return 0;
}

// 描画するフォントのサイズをセットする
int SetFontSize(int FontSize) {
  return 0;
}
// フォントの太さをセット
int SetFontThickness(int ThickPal) {
  return 0;
}
// 描画先画面のセット
int SetDrawScreen(int DrawScreen) {
  return 0;
}
// 画面の状態を初期化する
int ClearDrawScreen() {
  return 0;
}

// ３原色値から現在の画面モードに対応した色データ値を得る
uint32_t GetColor(int Red, int Green, int Blue) {
  return 0;
}

// 書式指定文字列を描画する
int DrawFormatString(int x, int y, int Color, const char *FormatString, ...) {
  return 0;
}
// グラフィックの回転描画
int DrawRotaGraph(int x, int y, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag) {
  return 0;
}
// フォントタイプの変更
int ChangeFontType(int FontType) {
  return 0;
}

// キーの入力待ち
int WaitKey(void) {
  return 0;
}
// 裏画面と表画面を交換する
int ScreenFlip(void) {
  return 0;
}
// ミリ秒単位の精度を持つカウンタの現在値を得る
int GetNowCount(int UseRDTSCFlag) {
  return 0;
}
// ジョイバッドの入力状態取得
int GetJoypadInputState(int InputType) {
  return 0;
}
// メモリに読み込んだWAVEデータの再生を止める
int StopSoundMem(int SoundHandle) {
  return 0;
}
// メモリに読みこんだWAVEデータを再生する
int PlaySoundMem(int SoundHandle, int PlayType, int TopPositionFlag) {
  return 0;
}
// キーボードの入力状態取得
int CheckHitKey(int KeyCode) {
  return 0;
}
// 指定の時間だけ処理をとめる
int WaitTimer(int WaitTime) {
  return 0;
}
// 乱数を取得する( RandMax : 返って来る値の最大値 )
int GetRand(int RandMax) {
  return 0;
}

// 点を描画する
int DrawPixel(int x, int y, int Color) {
  return 0;
}
// 線を描画
int DrawLine(int x1, int y1, int x2, int y2, int Color, int Thickness) {
  return 0;
}
// 四角形の描画
int DrawBox(int x1, int y1, int x2, int y2, int Color, int FillFlag) {
  return 0;
}
// 楕円を描く
int DrawOval(int x, int y, int rx, int ry, int Color, int FillFlag) {
  return 0;
}
// グラフィックの描画
int DrawGraph(int x, int y, int GrHandle, int TransFlag) {
  return 0;
}
// 画像の左右反転描画
int DrawTurnGraph(int x, int y, int GrHandle, int TransFlag) {
  return 0;
}
// 文字列の描画
int DrawString(int x, int y, const char *String, int Color, int EdgeColor) {
  return 0;
}
// メモリに読みこんだWAVEデータが再生中か調べる
int CheckSoundMem(int SoundHandle) {
  return 0;
}

