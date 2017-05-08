#ifndef ESUTIL_H
#define ESUTIL_H


#include <stdlib.h>
#include "FileWrapper.h"

#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#ifdef __cplusplus
extern "C"
#endif

#ifdef WIN32
#define ESUTIL_API  __cdecl
#define ESCALLBACK  __cdecl
#else
#define ESUTIL_API
#define ESCALLBACK
#endif


// 缓冲区标志
#define ES_WINDOW_RGB           0
#define ES_WINDOW_ALPHA         1
#define ES_WINDOW_DEPTH         2
#define ES_WINDOW_STENCIL       4
#define ES_WINDOW_MULTISAMPLE   8


#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif


//4 x 4矩阵
typedef struct {
   GLfloat   m[4][4];
} ESMatrix;

typedef struct ESContext ESContext;

struct ESContext {
   void     *platformData;
   void     *userData;
    
   GLint    width;
   GLint    height;

#ifndef __APPLE__
   EGLNativeDisplayType eglNativeDisplay;
   EGLNativeWindowType  eglNativeWindow;

   EGLDisplay  eglDisplay;
   EGLContext  eglContext;
   EGLSurface  eglSurface;
#endif

   /// 回调函数
   void ( ESCALLBACK *drawFunc ) ( ESContext * );
   void ( ESCALLBACK *shutdownFunc ) ( ESContext * );
   void ( ESCALLBACK *keyFunc ) ( ESContext *, unsigned char, int, int );
   void ( ESCALLBACK *updateFunc ) ( ESContext *, float deltaTime );
};



GLboolean ESUTIL_API esCreateWindow ( ESContext *esContext, const char *title, GLint width, GLint height, GLuint flags );


void ESUTIL_API esRegisterDrawFunc ( ESContext *esContext, void ( ESCALLBACK *drawFunc ) ( ESContext * ) );
void ESUTIL_API esRegisterShutdownFunc ( ESContext *esContext, void ( ESCALLBACK *shutdownFunc ) ( ESContext * ) );
void ESUTIL_API esRegisterUpdateFunc ( ESContext *esContext, void ( ESCALLBACK *updateFunc ) ( ESContext *, float ) );
void ESUTIL_API esRegisterKeyFunc ( ESContext *esContext,void ( ESCALLBACK *drawFunc ) ( ESContext *, unsigned char, int, int ) );
void ESUTIL_API esLogMessage ( const char *formatStr, ... );

GLuint ESUTIL_API esLoadShader ( GLenum type, const char *shaderSrc );
GLuint ESUTIL_API esLoadProgram ( const char *vertShaderSrc, const char *fragShaderSrc );





int ESUTIL_API esGenSphere ( int numSlices, float radius, GLfloat **vertices, GLfloat **normals,GLfloat **texCoords, GLuint **indices );
int ESUTIL_API esGenCube ( float scale, GLfloat **vertices, GLfloat **normals,GLfloat **texCoords, GLuint **indices );
int ESUTIL_API esGenSquareGrid ( int size, GLfloat **vertices, GLuint **indices );

char *ESUTIL_API esLoadTGA ( void *ioContext, const char *fileName, int *width, int *height );

void ESUTIL_API esScale ( ESMatrix *result, GLfloat sx, GLfloat sy, GLfloat sz );
void ESUTIL_API esTranslate ( ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz );
void ESUTIL_API esRotate ( ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
void ESUTIL_API esFrustum ( ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ );
void ESUTIL_API esPerspective ( ESMatrix *result, float fovy, float aspect, float nearZ, float farZ );
void ESUTIL_API esOrtho ( ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ );
void ESUTIL_API esMatrixMultiply ( ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB );
void ESUTIL_API esMatrixLoadIdentity ( ESMatrix *result );

void ESUTIL_API
esMatrixLookAt ( ESMatrix *result,
                 float posX,    float posY,    float posZ,
                 float lookAtX, float lookAtY, float lookAtZ,
                 float upX,     float upY,     float upZ );

#ifdef __cplusplus
{}
#endif

#endif // ESUTIL_H
