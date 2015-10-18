#include <stdint.h>
#include <emscripten.h>

typedef uint8_t byte;
#define TRUE 1
#define FALSE 0


#define DX_SOUNDDATATYPE_MEMPRESS                   (2)             // 圧縮された全データはシステムメモリに格納され、再生する部分だけ逐次解凍しながらサウンドメモリに格納する(鳴らし終わると解凍したデータは破棄されるので何度も解凍処理が行われる)
#define DX_SOUNDDATATYPE_MEMNOPRESS                 (0)             // 圧縮された全データは再生が始まる前にサウンドメモリにすべて解凍され、格納される

#define DX_SCREEN_BACK                              (0xfffffffe)
#define DX_FONTTYPE_EDGE                            (1)             // エッジつきフォント
#define DX_FONTTYPE_NORMAL                          (0)             // ノーマルフォント
#define DX_INPUT_KEY_PAD1                           (0x1001)        // キー入力とパッド１入力


#define DX_PLAYTYPE_LOOPBIT                         (0x0002)        // ループ再生ビット
#define DX_PLAYTYPE_BACKBIT                         (0x0001)        // バックグラウンド再生ビット

#define DX_PLAYTYPE_NORMAL                          (0)                                             // ノーマル再生
#define DX_PLAYTYPE_BACK                            (DX_PLAYTYPE_BACKBIT)                           // バックグラウンド再生
#define DX_PLAYTYPE_LOOP                            (DX_PLAYTYPE_LOOPBIT | DX_PLAYTYPE_BACKBIT)     // ループ再生



#define PAD_INPUT_LEFT                              (0x00000002)    // ←チェックマスク
#define PAD_INPUT_RIGHT                             (0x00000004)    // →チェックマスク
#define PAD_INPUT_DOWN                              (0x00000001)    // ↓チェックマスク
#define KEY_INPUT_F1                                1           // Ｆ１キー

#define KEY_INPUT_0                                 0           // ０キー
#define KEY_INPUT_O                                 0           // Ｏキー
#define PAD_INPUT_UP                                (0x00000008)    // ↑チェックマスク
#define KEY_INPUT_Z                                 0           // Ｚキー
#define KEY_INPUT_1                                 0           // Ｚキー
#define KEY_INPUT_ESCAPE                        0       // エスケープキー
#define KEY_INPUT_SPACE                     0       // エスケープキー
#define KEY_INPUT_RETURN                        0       // エスケープキー






// グラフィックに設定する透過色をセットする
extern int SetTransColor(int Red, int Green, int Blue);

// 画像ファイルのメモリへの読みこみ
extern int LoadGraph(const char *FileName, int NotUse3DFlag = false);

// 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックハンドルを作成する
extern int DerivationGraph(int SrcX, int SrcY, int Width, int Height, int SrcGraphHandle);

// グラフィックのサイズを得る
extern int GetGraphSize(int GrHandle, int *SizeXBuf, int *SizeYBuf);

// 作成するサウンドのデータ形式を設定する( DX_SOUNDDATATYPE_MEMNOPRESS 等 )
extern int SetCreateSoundDataType(int SoundDataType);

// サウンドデータを追加する
extern int LoadSoundMem(const char *WaveName, int BufferNum = 3, int UnionHandle = -1);

// メモリに読みこんだWAVEデータの再生にボリュームを設定する( パーセント指定 )
extern int ChangeVolumeSoundMem(int VolumePal, int SoundHandle);

// 画面モードを設定する
extern int SetGraphMode(int ScreenSizeX, int ScreenSizeY, int ColorBitDepth, int RefreshRate = 60);

// ライブラリ初期化関数
extern int DxLib_Init(void);
// ライブラリ使用の終了関数
extern int DxLib_End(void);


// 描画するフォントのサイズをセットする
extern int SetFontSize(int FontSize);
// フォントの太さをセット
extern int SetFontThickness(int ThickPal);
// 描画先画面のセット
extern int SetDrawScreen(int DrawScreen);
// 画面の状態を初期化する
extern int ClearDrawScreen();

// ３原色値から現在の画面モードに対応した色データ値を得る
extern uint32_t GetColor(int Red, int Green, int Blue);

// 書式指定文字列を描画する
extern int DrawFormatString(int x, int y, int Color, const char *FormatString, ...);
// グラフィックの回転描画
extern int DrawRotaGraph(int x, int y, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = FALSE);
// フォントタイプの変更
extern int ChangeFontType(int FontType);

// キーの入力待ち
extern int WaitKey(void);
// 裏画面と表画面を交換する
extern int ScreenFlip(void);
// ミリ秒単位の精度を持つカウンタの現在値を得る
extern int GetNowCount(int UseRDTSCFlag = FALSE);
// ジョイバッドの入力状態取得
extern int GetJoypadInputState(int InputType);
// メモリに読み込んだWAVEデータの再生を止める
extern int StopSoundMem(int SoundHandle);
// メモリに読みこんだWAVEデータを再生する
extern int PlaySoundMem(int SoundHandle, int PlayType, int TopPositionFlag = TRUE);
// キーボードの入力状態取得
extern int CheckHitKey(int KeyCode);
// 指定の時間だけ処理をとめる
extern int WaitTimer(int WaitTime);
// 乱数を取得する( RandMax : 返って来る値の最大値 )
extern int GetRand(int RandMax);

// 点を描画する
extern int DrawPixel(int x, int y, int Color);
// 線を描画
extern int DrawLine(int x1, int y1, int x2, int y2, int Color, int Thickness = 1);
// 四角形の描画
extern int DrawBox(int x1, int y1, int x2, int y2, int Color, int FillFlag);
// 楕円を描く
extern int DrawOval(int x, int y, int rx, int ry, int Color, int FillFlag);
// グラフィックの描画
extern int DrawGraph(int x, int y, int GrHandle, int TransFlag);
// 画像の左右反転描画
extern int DrawTurnGraph(int x, int y, int GrHandle, int TransFlag);
// 文字列の描画
extern int DrawString(int x, int y, const char *String, int Color, int EdgeColor = 0);
// メモリに読みこんだWAVEデータが再生中か調べる
extern int CheckSoundMem(int SoundHandle);
