#include "dxlib.h"

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <X11/Xlib.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <emscripten/html5.h>
#include <sys/time.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


static Display *x_display = NULL;
static EGLDisplay eglDisplay = NULL;
static EGLContext eglContext = NULL;
static EGLSurface eglSurface = NULL;

//------------------------------------------------------------------------------
// OpenGL util functions
//------------------------------------------------------------------------------

GLuint esLoadShader ( GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;
   
   // Create the shader object
   shader = glCreateShader ( type );

   if ( shader == 0 )
   	return 0;

   // Load the shader source
   glShaderSource ( shader, 1, &shaderSrc, NULL );
   
   // Compile the shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled ) 
   {
      GLint infoLen = 0;
      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 && infoLen < 1024)
      {
        char infoLog[infoLen];
        glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
        printf ( "Error compiling shader:\n%s\n", infoLog );            
      }

      glDeleteShader ( shader );
      return 0;
   }

   return shader;

}

GLuint esLoadProgram ( const char *vertShaderSrc, const char *fragShaderSrc )
{
   GLuint vertexShader;
   GLuint fragmentShader;
   GLuint programObject;
   GLint linked;

   // Load the vertex/fragment shaders
   vertexShader = esLoadShader ( GL_VERTEX_SHADER, vertShaderSrc );
   if ( vertexShader == 0 )
      return 0;

   fragmentShader = esLoadShader ( GL_FRAGMENT_SHADER, fragShaderSrc );
   if ( fragmentShader == 0 )
   {
      glDeleteShader( vertexShader );
      return 0;
   }

   // Create the program object
   programObject = glCreateProgram ( );
   
   if ( programObject == 0 )
      return 0;

   glAttachShader ( programObject, vertexShader );
   glAttachShader ( programObject, fragmentShader );

   // Link the program
   glLinkProgram ( programObject );

   // Check the link status
   glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

   if ( !linked ) 
   {
      GLint infoLen = 0;

      glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 && infoLen < 1024)
      {
         char infoLog[infoLen];
         glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
         printf ( "Error linking program:\n%s\n", infoLog );            
      }

      glDeleteProgram ( programObject );
      return 0;
   }

   // Free up no longer needed shader resources
   glDeleteShader ( vertexShader );
   glDeleteShader ( fragmentShader );

   return programObject;
}

EGLBoolean CrateWindow(int width, int height)
{
    Window root;
    XSetWindowAttributes swa;
    Window win;

    /*
     * X11 native display initialization
     */

    x_display = XOpenDisplay(NULL);
    if ( x_display == NULL )
    {
        return EGL_FALSE;
    }

    root = DefaultRootWindow(x_display);

    swa.event_mask  =  ExposureMask | PointerMotionMask | KeyPressMask;
    win = XCreateWindow(
               x_display, root,
               0, 0, width, height, 0,
               CopyFromParent, InputOutput,
               CopyFromParent, CWEventMask,
               &swa );

    return EGL_TRUE;
}

EGLBoolean CreateEGLContext (EGLint attribList[])
{
   EGLint numConfigs;
   EGLint majorVersion;
   EGLint minorVersion;
   EGLDisplay display;
   EGLContext context;
   EGLSurface surface;
   EGLConfig config;
   EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

   // Get Display
   display = eglGetDisplay(EGL_DEFAULT_DISPLAY );
   if ( display == EGL_NO_DISPLAY )
   {
      return EGL_FALSE;
   }

   // Initialize EGL
   if ( !eglInitialize(display, &majorVersion, &minorVersion) )
   {
      return EGL_FALSE;
   }

   // Get configs
   if ( !eglGetConfigs(display, NULL, 0, &numConfigs) )
   {
      return EGL_FALSE;
   }

   // Choose config
   if ( !eglChooseConfig(display, attribList, &config, 1, &numConfigs) )
   {
      return EGL_FALSE;
   }

   // Create a surface
   surface = eglCreateWindowSurface(display, config, NULL, NULL);
   if ( surface == EGL_NO_SURFACE )
   {
      return EGL_FALSE;
   }

   // Create a GL context
   context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
   if ( context == EGL_NO_CONTEXT )
   {
      return EGL_FALSE;
   }   
   
   // Make the context current
   if ( !eglMakeCurrent(display, surface, surface, context) )
   {
      return EGL_FALSE;
   }
   
   eglDisplay = display;
   eglSurface = surface;
   eglContext = context;
   return EGL_TRUE;
} 

//------------------------------------------------------------------------------
// Input functions
//------------------------------------------------------------------------------
static byte key_state[256] = {0};
static int joypad_state = 0;

static EM_BOOL key_callback(int eventType, const EmscriptenKeyboardEvent *e, void *userData)
{
  if (e->repeat)
    return 1;

  if (e->keyCode <= 0 || e->keyCode >= 256)
    return 1;

  if (eventType == 2) {
    key_state[e->keyCode] = 1;
  }
  else if (eventType == 3) {
    key_state[e->keyCode] = 0;
  }

  joypad_state = 0;
  if (key_state[37]) joypad_state |= PAD_INPUT_LEFT;
  if (key_state[38]) joypad_state |= PAD_INPUT_UP;
  if (key_state[39]) joypad_state |= PAD_INPUT_RIGHT;
  if (key_state[40]) joypad_state |= PAD_INPUT_DOWN;

  /*
  printf("%d, key: \"%s\", code: \"%s\", location: %lu,%s%s%s%s repeat: %d, locale: \"%s\", char: \"%s\", charCode: %lu, keyCode: %lu, which: %lu\n",
    eventType, e->key, e->code, e->location, 
    e->ctrlKey ? " CTRL" : "", e->shiftKey ? " SHIFT" : "", e->altKey ? " ALT" : "", e->metaKey ? " META" : "", 
    e->repeat, e->locale, e->charValue, e->charCode, e->keyCode, e->which);
  */

  return 1;
}

EM_BOOL mouse_callback(int eventType, const EmscriptenMouseEvent *e, void *userData)
{
  if (eventType == 5) {
    key_state[KEY_INPUT_RETURN] = 1;
  }
  else if (eventType == 6) {
    key_state[KEY_INPUT_RETURN] = 0;
  }

  /*
  printf("%d, screen: (%ld,%ld), client: (%ld,%ld),%s%s%s%s button: %hu, buttons: %hu, movement: (%ld,%ld), canvas: (%ld,%ld)\n",
    eventType, e->screenX, e->screenY, e->clientX, e->clientY,
    e->ctrlKey ? " CTRL" : "", e->shiftKey ? " SHIFT" : "", e->altKey ? " ALT" : "", e->metaKey ? " META" : "", 
    e->button, e->buttons, e->movementX, e->movementY, e->canvasX, e->canvasY);
    */

  return 0;
}

EM_BOOL touch_callback(int eventType, const EmscriptenTouchEvent *e, void *userData)
{
  if (eventType == 22) {
    key_state[KEY_INPUT_RETURN] = 1;
  }
  else if (eventType == 23) {
    key_state[KEY_INPUT_RETURN] = 0;
  }
  /*
  printf("%d, numTouches: %d %s%s%s%s\n",
    eventType, e->numTouches,
    e->ctrlKey ? " CTRL" : "", e->shiftKey ? " SHIFT" : "", e->altKey ? " ALT" : "", e->metaKey ? " META" : "");
  for(int i = 0; i < e->numTouches; ++i)
  {
    const EmscriptenTouchPoint *t = &e->touches[i];
    printf("  %ld: screen: (%ld,%ld), client: (%ld,%ld), page: (%ld,%ld), isChanged: %d, onTarget: %d, canvas: (%ld, %ld)\n",
      t->identifier, t->screenX, t->screenY, t->clientX, t->clientY, t->pageX, t->pageY, t->isChanged, t->onTarget, t->canvasX, t->canvasY);
  }
  */
  
  return 0;
}

static void InputInit() {
  emscripten_set_keydown_callback(0, 0, 1, key_callback);
  emscripten_set_keyup_callback(0, 0, 1, key_callback);
  emscripten_set_mousedown_callback(0, 0, 1, mouse_callback);
  emscripten_set_mouseup_callback(0, 0, 1, mouse_callback);

  emscripten_set_touchstart_callback(0, 0, 1, touch_callback);
  emscripten_set_touchend_callback(0, 0, 1, touch_callback);

}
  
//------------------------------------------------------------------------------
// Graphics functions
//------------------------------------------------------------------------------

static const char vShaderStr[] =  
"attribute vec4 a_position;\n"
"attribute vec2 a_texCoord;\n"
"uniform vec4 u_posTrans;\n"
"uniform vec4 u_uvTrans;\n"
"varying vec2 v_texCoord;\n"
"void main() {\n"
"  vec2 pos = a_position.xy * u_posTrans.zw + u_posTrans.xy;\n"
"  gl_Position = vec4(pos, 0, 1);\n"
"  v_texCoord = a_texCoord * u_uvTrans.zw + u_uvTrans.xy;\n"
"}\n";

static const char fShaderStr[] =  
"precision mediump float;\n"
"varying vec2 v_texCoord;\n"
"uniform sampler2D s_texture;\n"
"uniform vec4 u_color;\n"
"void main() {\n"
"  gl_FragColor = texture2D(s_texture, v_texCoord) * u_color;\n"
//"  gl_FragColor = vec4(1, 1, 1, 1);\n"
"}\n";

static GLuint programObject = 0;
static GLint a_position = 0;
static GLint a_texCoord = 0;
static GLint u_posTrans = 0;
static GLint u_uvTrans = 0;
static GLint u_color = 0;
static GLint s_texture = 0;

static GLuint vertexObject = 0;
static GLuint indexObject = 0;
static GLuint textureId = 0;
static GLuint whiteTexture = 0;

static int screenSizeX = 100;
static int screenSizeY = 100;
static int transColor = 0;

struct GraphData {
  GLint texture;
  GLint w;
  GLint h;
  float x;
  float y;
  float sx;
  float sy;
};

static GraphData graphArray[128] = {0};
static int graphLoadId = 1;

static TTF_Font *font = NULL;


// 画面モードを設定する
int SetGraphMode(int ScreenSizeX, int ScreenSizeY, int ColorBitDepth, int RefreshRate) {
  screenSizeX = ScreenSizeX;
  screenSizeY = ScreenSizeY;
  return 0;
}

GLuint Create1x1Texture(byte r, byte g, byte b, byte a)
{
   // Texture object handle
   GLuint textureId;
   
   // 2x2 Image, 3 bytes per pixel (R, G, B)
   GLubyte pixels[4 * 4] = {
     r, g, b, a,
     r, g, b, a,
     r, g, b, a,
     r, g, b, a,
   };

   // Use tightly packed data
   glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

   // Generate a texture object
   glGenTextures ( 1, &textureId );

   // Bind the texture object
   glBindTexture ( GL_TEXTURE_2D, textureId );

   // Load the texture
   glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

   // Set the filtering mode
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

   return textureId;
}


// ライブラリ初期化関数
int DxLib_Init(void) {
   EGLint attribList[] =
   {
       EGL_RED_SIZE,       5,
       EGL_GREEN_SIZE,     6,
       EGL_BLUE_SIZE,      5,
       EGL_ALPHA_SIZE,     EGL_DONT_CARE,
       EGL_DEPTH_SIZE,     EGL_DONT_CARE,
       EGL_STENCIL_SIZE,   EGL_DONT_CARE,
       EGL_SAMPLE_BUFFERS, 0,
       EGL_NONE
   };
   
   if ( !CrateWindow (screenSizeX, screenSizeY) ) {
      return -1;
   }

   if ( !CreateEGLContext (attribList) ) {
      return -1;
   }

  // Load the shaders and get a linked program object
  programObject = esLoadProgram ( vShaderStr, fShaderStr );

  // Get the attribute locations
  a_position = glGetAttribLocation ( programObject, "a_position" );
  a_texCoord = glGetAttribLocation ( programObject, "a_texCoord" );
  u_posTrans = glGetUniformLocation ( programObject, "u_posTrans" );
  u_uvTrans = glGetUniformLocation ( programObject, "u_uvTrans" );
  u_color = glGetUniformLocation ( programObject, "u_color" );

  // Get the sampler location
  s_texture = glGetUniformLocation ( programObject, "s_texture" );

  // Load the texture
  whiteTexture = Create1x1Texture(255, 255, 255, 255);

  // Setup the vertex data
  GLfloat vVertices[(20 + 16) * 5] = { 
    // normal
    0, 0, 0, 0, 0,
    0, 1, 0, 0, 1,
    1, 0, 0, 1, 0,
    1, 1, 0, 1, 1,

    // flip
    0, 0, 0, 1, 0,
    0, 1, 0, 1, 1,
    1, 0, 0, 0, 0,
    1, 1, 0, 0, 1,

    // flip horizontal
    0, 0, 0, 0, 1,
    0, 1, 0, 0, 0,
    1, 0, 0, 1, 1,
    1, 1, 0, 1, 0,

    // line loop
    0, 0, 0, 1, 0,
    0, 1, 0, 1, 1,
    1, 1, 0, 0, 1,
    1, 0, 0, 0, 0,

    // line
    0, 0, 0, 0, 0,
    1, 1, 0, 1, 1,

    // not used
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
  };
  GLushort indices[] = { 0, 1, 2, 1, 2, 3 };

  // build circle vertices
  for (int i = 0; i < 16; i++) {
    float x = cosf(M_PI * 2 * i / 15);
    float y = sinf(M_PI * 2 * i / 15);
    GLfloat *v = vVertices + (20 + i)* 5;
    v[0] = x;
    v[1] = y;
  }

  glGenBuffers(1, &vertexObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexObject );
  glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW );

  glGenBuffers(1, &indexObject);
  glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, indexObject );
  glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW );

  // Use the program object
  glUseProgram ( programObject );

  // Load the vertex position
  glBindBuffer (GL_ARRAY_BUFFER, vertexObject );
  glVertexAttribPointer ( a_position, 3, GL_FLOAT, GL_FALSE, 5 * 4, 0 );
  glVertexAttribPointer ( a_texCoord, 2, GL_FLOAT, GL_FALSE, 5 * 4, (void*)(3 * 4) );

  glEnableVertexAttribArray ( a_position );
  glEnableVertexAttribArray ( a_texCoord );

  // Bind the texture
  glActiveTexture ( GL_TEXTURE0 );
  glBindTexture ( GL_TEXTURE_2D, textureId );

  // Alpha blend
  glEnable ( GL_BLEND );
  glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  // Set the sampler texture unit to 0
  glUniform1i ( s_texture, 0 );
  glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, indexObject );

  // Font
  font = TTF_OpenFont("sans-serif", 40);

  InputInit();
  return 0;
}

// ライブラリ使用の終了関数
int DxLib_End(void) {
  return 0;
}


// 画像ファイルのメモリへの読みこみ
int LoadGraph(const char *FileName, int NotUse3DFlag) {
  if (graphLoadId >= sizeof(graphArray) / sizeof(graphArray[0])) {
    printf("Too many images.\n");
    return 0;
  }

  SDL_Surface *surface = IMG_Load(FileName);
  if (surface == NULL) {
    printf("Failed to load image: %s\n", FileName);
    return 0;
  }

  byte* data = (byte*)surface->pixels;
  int size = surface->w * surface->h;
  while (size > 0) {
    if (data[0] == 153 && data[1] == 255 && data[2] == 255) {
      data[3] = 0;
    }
    data += 4;
    size--;
  }

  GLuint texture = 0;
  glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
  glGenTextures( 1, &texture );
  glBindTexture( GL_TEXTURE_2D, texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels );

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );


  GraphData *g = &graphArray[graphLoadId];
  g->texture = texture;
  g->w = surface->w;
  g->h = surface->h;
  g->x = 0;
  g->y = 0;
  g->sx = 1;
  g->sy = 1;

  return graphLoadId++;
}

// 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックハンドルを作成する
int DerivationGraph(int SrcX, int SrcY, int Width, int Height, int SrcGraphHandle) {
  if (graphLoadId >= sizeof(graphArray) / sizeof(graphArray[0])) {
    printf("Too many images.");
    return 0;
  }

  GraphData *g1 = &graphArray[SrcGraphHandle];
  GraphData *g = &graphArray[graphLoadId];

  g->texture = g1->texture;
  g->w = Width;
  g->h = Height;
  g->x = (float)SrcX / g1->w;
  g->y = (float)SrcY / g1->h;
  g->sx = (float)Width / g1->w;
  g->sy = (float)Height / g1->h;

  return graphLoadId++;
}

// グラフィックのサイズを得る
int GetGraphSize(int GrHandle, int *SizeXBuf, int *SizeYBuf) {
  GraphData *g = &graphArray[GrHandle];
  *SizeXBuf = g->w;
  *SizeYBuf = g->h;
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
  byte Alpha = 0xff;
  return Red | (Green << 8) | (Blue << 16) | (Alpha << 24);
}

// 書式指定文字列を描画する
int DrawFormatString(int x, int y, int Color, const char *FormatString, ...) {
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
  struct timeval t1;
  struct timezone tz;
  gettimeofday(&t1, &tz);
  return ((t1.tv_sec & 0x0fffffff) * 1000 + (t1.tv_usec) / 1000);
}
// ジョイバッドの入力状態取得
int GetJoypadInputState(int InputType) {
  return joypad_state;
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
  if (KeyCode > 0 && KeyCode < 256) {
    return key_state[KeyCode];
  }
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

// 文字列の描画
int DrawString(int x, int y, const char *String, int Color, int EdgeColor) {
  return 0;
}
// メモリに読みこんだWAVEデータが再生中か調べる
int CheckSoundMem(int SoundHandle) {
  return 0;
}

int color;
int mirror;
int prevtexture = 0;

//画像関係
//{
//色かえ(指定)
void setcolor(int red, int green, int blue) {
  color = GetColor(red, green, blue);
}

void clearscreen() {
  float r = (color & 0xff) / 255.0f;
  float g = ((color >> 8) & 0xff) / 255.0f;
  float b = ((color >> 16) & 0xff) / 255.0f;
  glClearColor(r, g, b, 1);
  glClear ( GL_COLOR_BUFFER_BIT );
  prevtexture = 0;
}

//色かえ(黒)(白)
void setc0() {color = GetColor(0, 0, 0); }
void setc1() {color = GetColor(255, 255, 255); }

static void set_texture(int id) {
  if (prevtexture != id) {
    prevtexture = id;
    glBindTexture ( GL_TEXTURE_2D, id );
  }
}

static void set_transform(float x, float y, float w, float h) {
  float sx = 2.0f / screenSizeX;
  float sy = -2.0f / screenSizeY;
  glUniform4f (u_posTrans, x * sx - 1, y * sy + 1, w * sx, h * sy);
}
static void set_color(int color) {
  float r = (color & 0xff) / 255.0f;
  float g = ((color >> 8) & 0xff) / 255.0f;
  float b = ((color >> 16) & 0xff) / 255.0f;
  glUniform4f (u_color, r, g, b, 1);
}

//線
void drawline(int x, int y, int w, int h) {
  set_texture(whiteTexture);
  set_transform(x, y, w - x, h - y);
  set_color(color);
  glDrawArrays(GL_LINES, 16, 2);
}

//四角形(塗り無し)
void drawrect(int x, int y, int w, int h) {
  set_texture(whiteTexture);
  set_transform(x, y, w, h);
  set_color(color);

  glDrawArrays(GL_LINE_LOOP, 12, 4);
}

//四角形(塗り有り)
void fillrect(int x, int y, int w, int h) {
  set_texture(whiteTexture);
  set_transform(x, y, w, h);
  set_color(color);
  glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4);
}

//円(塗り無し)
void drawarc(int x, int y, int w, int h) {
  set_texture(whiteTexture);
  set_transform(x, y, w, h);
  set_color(color);
  glDrawArrays (GL_LINE_LOOP, 20, 15);
}
//円(塗り有り)
void fillarc(int x, int y, int w, int h) {
  set_texture(whiteTexture);
  set_transform(x, y, w, h);
  set_color(color);
  glDrawArrays (GL_TRIANGLE_FAN, 20, 15);
}

//画像表示
void drawimage(int mx, int x, int y) {
  GraphData *g = &graphArray[mx];
  set_texture(g->texture);
  set_transform(x, y, g->w, g->h);
  glUniform4f (u_uvTrans, g->x, g->y, g->sx, g->sy);
  glUniform4f (u_color, 1, 1, 1, 1);

  if (mirror == 0)
    glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4);
  if (mirror == 1)
    glDrawArrays ( GL_TRIANGLE_STRIP, 4, 4);
}

// グラフィックの回転描画
void drawimageflip(int mx, int x, int y) {
  GraphData *g = &graphArray[mx];
  set_texture(g->texture);
  set_transform(x, y, g->w, g->h);
  glUniform4f (u_uvTrans, g->x, g->y, g->sx, g->sy);
  glUniform4f (u_color, 1, 1, 1, 1);

  glDrawArrays ( GL_TRIANGLE_STRIP, 8, 4);
}

//反転
void setre() {}     //g.setFlipMode(Graphics.FLIP_HORIZONTAL);}
void setre2() {}     //g.setFlipMode(Graphics.FLIP_VERTICAL);}
void setno() {}     //g.setFlipMode(Graphics.FLIP_NONE);}

