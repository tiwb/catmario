#include <stdint.h>
#include <emscripten.h>

typedef uint8_t byte;
#define TRUE 1
#define FALSE 0


#define DX_FONTTYPE_EDGE                            (1)             // エッジつきフォント
#define DX_FONTTYPE_NORMAL                          (0)             // ノーマルフォント

#define DX_INPUT_KEY_PAD1                           (0x1001)        // キー入力とパッド１入力


#define PAD_INPUT_DOWN                              (0x00000001)
#define PAD_INPUT_LEFT                              (0x00000002)
#define PAD_INPUT_RIGHT                             (0x00000004)
#define PAD_INPUT_UP                                (0x00000008)

#define KEY_INPUT_F1                                1
#define KEY_INPUT_0                                 48
#define KEY_INPUT_1                                 49
#define KEY_INPUT_O                                 79
#define KEY_INPUT_Z                                 90
#define KEY_INPUT_ESCAPE                            27
#define KEY_INPUT_SPACE                             32
#define KEY_INPUT_RETURN                            13




extern "C" {
  void input_init();
  int input_waitkey(void);
  int input_keydown(int keycode);
  int input_getjoypad();

  int  graphics_init();

  int loadimage(const char *FileName);
  int subimage(int SrcX, int SrcY, int Width, int Height, int SrcGraphHandle);
  void getimagesize(int GrHandle, int *SizeXBuf, int *SizeYBuf);

  void clearscreen();
  void drawline(int a, int b, int c, int d);
  void drawrect(int a, int b, int c, int d);
  void fillrect(int a, int b, int c, int d);
  void drawarc(int a, int b, int c, int d);
  void fillarc(int a, int b, int c, int d);

  void setcolor(int red, int green, int blue);
  void setc0();
  void setc1();
  void setmirror(int mirror);
  void drawimage(int mx, int a, int b);
  void drawimageflip(int mx, int a, int b);

  void setfont(int x, int y);
  int setfonttype(int FontType);
  int drawstring(int x, int y, const char *String);

  void sound_init();
  void soundplay(int x);
  void soundstop(int x);
  int soundcheck(int x);
  void bgmchange(int x);
  void bgmplay();
  void bgmstop();

  int getrand(int value);
  int gettime();
}
