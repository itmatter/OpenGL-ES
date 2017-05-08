///
//  Includes
//
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "esUtil.h"

#ifdef ANDROID
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/asset_manager.h>
typedef AAsset esFile;
#else
typedef FILE esFile;
#endif

#ifdef __APPLE__
#include "FileWrapper.h"
#endif

///
//  Macros
//
#define INVERTED_BIT            (1 << 5)

///
//  Types
//
#ifndef __APPLE__
#pragma pack(push,x1)                            // Byte alignment (8-bit)
#pragma pack(1)
#endif

typedef struct
#ifdef __APPLE__
__attribute__ ( ( packed ) )
#endif
{
   unsigned char  IdSize,               MapType,        ImageType;
   unsigned short PaletteStart,         PaletteSize;
   unsigned char  PaletteEntryDepth;
   unsigned short X,Y,Width,Height;
   unsigned char  ColorDepth,           Descriptor;

} TGA_HEADER;

#ifndef __APPLE__
#pragma pack(pop,x1)
#endif

#ifndef __APPLE__


EGLint GetContextRenderableType ( EGLDisplay eglDisplay )
{
#ifdef EGL_KHR_create_context
    const char *extensions = eglQueryString ( eglDisplay, EGL_EXTENSIONS );
    if ( extensions != NULL && strstr( extensions, "EGL_KHR_create_context" ) ) {
       return EGL_OPENGL_ES3_BIT_KHR;
    }
#endif
    return EGL_OPENGL_ES2_BIT;
}
#endif


GLboolean ESUTIL_API esCreateWindow ( ESContext *esContext, const char *title, GLint width, GLint height, GLuint flags ) {
#ifndef __APPLE__
    EGLConfig config;        //配置
    EGLint majorVersion;     //主版本
    EGLint minorVersion;     //次版本
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE }; //上下文属性

    if ( esContext == NULL ) {
        return GL_FALSE;
    }

#ifdef ANDROID
    esContext->width = ANativeWindow_getWidth ( esContext->eglNativeWindow );
    esContext->height = ANativeWindow_getHeight ( esContext->eglNativeWindow );
#else
    esContext->width = width;
    esContext->height = height;
#endif

    // 创建Win
    if ( !WinCreate ( esContext, title ) ) {
        return GL_FALSE;
    }

    // 打开与EGL显示服务器连接
    esContext->eglDisplay = eglGetDisplay( esContext->eglNativeDisplay );
    if ( esContext->eglDisplay == EGL_NO_DISPLAY ) {
       return GL_FALSE;
    }

    // 初始化 EGL
    if ( !eglInitialize ( esContext->eglDisplay, &majorVersion, &minorVersion ) ) {
        return GL_FALSE;
    }

    {
        EGLint numConfigs = 0;
        EGLint attribList[] =
        {
            EGL_RED_SIZE,       5,
            EGL_GREEN_SIZE,     6,
            EGL_BLUE_SIZE,      5,
            EGL_ALPHA_SIZE,     ( flags & ES_WINDOW_ALPHA ) ? 8 : EGL_DONT_CARE,
            EGL_DEPTH_SIZE,     ( flags & ES_WINDOW_DEPTH ) ? 8 : EGL_DONT_CARE,
            EGL_STENCIL_SIZE,   ( flags & ES_WINDOW_STENCIL ) ? 8 : EGL_DONT_CARE,
            EGL_SAMPLE_BUFFERS, ( flags & ES_WINDOW_MULTISAMPLE ) ? 1 : 0,
            // if EGL_KHR_create_context extension is supported, then we will use
            // EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT in the attribute list
            EGL_RENDERABLE_TYPE, GetContextRenderableType ( esContext->eglDisplay ),
            EGL_NONE
        };

        // EGL选择配置
        if ( !eglChooseConfig ( esContext->eglDisplay, attribList, &config, 1, &numConfigs ) ) {
            return GL_FALSE;
        }

        if ( numConfigs < 1 ) {
            return GL_FALSE;
        }
    }


#ifdef ANDROID
    // For Android, need to get the EGL_NATIVE_VISUAL_ID and set it using ANativeWindow_setBuffersGeometry
    {
        EGLint format = 0;
        eglGetConfigAttrib ( esContext->eglDisplay, config, EGL_NATIVE_VISUAL_ID, &format );
        ANativeWindow_setBuffersGeometry ( esContext->eglNativeWindow, 0, 0, format );
    }
#endif // ANDROID

    // 创建屏幕上的渲染区域 : EGL窗口
    esContext->eglSurface = eglCreateWindowSurface ( esContext->eglDisplay, config, esContext->eglNativeWindow, NULL );
    
    if ( esContext->eglSurface == EGL_NO_SURFACE ){
        return GL_FALSE;
    }

    esContext->eglContext = eglCreateContext ( esContext->eglDisplay, config, EGL_NO_CONTEXT, contextAttribs );
    if ( esContext->eglContext == EGL_NO_CONTEXT ) {
        return GL_FALSE;
    }

    if ( !eglMakeCurrent ( esContext->eglDisplay, esContext->eglSurface, esContext->eglSurface, esContext->eglContext ) ) {
        return GL_FALSE;
    }

#endif

    return GL_TRUE;
}


void ESUTIL_API esRegisterDrawFunc ( ESContext *esContext, void ( ESCALLBACK *drawFunc ) ( ESContext * ) ){
   esContext->drawFunc = drawFunc;
}

void ESUTIL_API esRegisterShutdownFunc ( ESContext *esContext, void ( ESCALLBACK *shutdownFunc ) ( ESContext * ) ){
   esContext->shutdownFunc = shutdownFunc;
}

void ESUTIL_API esRegisterUpdateFunc ( ESContext *esContext, void ( ESCALLBACK *updateFunc ) ( ESContext *, float ) ){
   esContext->updateFunc = updateFunc;
}








void ESUTIL_API esRegisterKeyFunc ( ESContext *esContext,void ( ESCALLBACK *keyFunc ) ( ESContext *, unsigned char, int, int ) ){
   esContext->keyFunc = keyFunc;
}

void ESUTIL_API esLogMessage ( const char *formatStr, ... )
{
   va_list params;
   char buf[BUFSIZ];

   va_start ( params, formatStr );
   vsprintf ( buf, formatStr, params );

#ifdef ANDROID
   __android_log_print ( ANDROID_LOG_INFO, "esUtil" , "%s", buf );
#else
   printf ( "%s", buf );
#endif

   va_end ( params );
}

static esFile *esFileOpen ( void *ioContext, const char *fileName ){
   esFile *pFile = NULL;

#ifdef ANDROID
   if ( ioContext != NULL )
   {
      AAssetManager *assetManager = ( AAssetManager * ) ioContext;
      pFile = AAssetManager_open ( assetManager, fileName, AASSET_MODE_BUFFER );
   }

#else
#ifdef __APPLE__
   // iOS: Remap the filename to a path that can be opened from the bundle.
   fileName = GetBundleFileName ( fileName );
#endif

   pFile = fopen ( fileName, "rb" );
#endif

   return pFile;
}

static void esFileClose ( esFile *pFile ) {
   if ( pFile != NULL ){
#ifdef ANDROID
      AAsset_close ( pFile );
#else
      fclose ( pFile );
      pFile = NULL;
#endif
   }
}

static int esFileRead ( esFile *pFile, int bytesToRead, void *buffer )
{
   int bytesRead = 0;

   if ( pFile == NULL )
   {
      return bytesRead;
   }

#ifdef ANDROID
   bytesRead = AAsset_read ( pFile, buffer, bytesToRead );
#else
   bytesRead = fread ( buffer, bytesToRead, 1, pFile );
#endif

   return bytesRead;
}

char *ESUTIL_API esLoadTGA ( void *ioContext, const char *fileName, int *width, int *height )
{
   char        *buffer;
   esFile      *fp;
   TGA_HEADER   Header;
   int          bytesRead;

   // Open the file for reading
   fp = esFileOpen ( ioContext, fileName );

   if ( fp == NULL )
   {
      // Log error as 'error in opening the input file from apk'
      esLogMessage ( "esLoadTGA FAILED to load : { %s }\n", fileName );
      return NULL;
   }

   bytesRead = esFileRead ( fp, sizeof ( TGA_HEADER ), &Header );

   *width = Header.Width;
   *height = Header.Height;

   if ( Header.ColorDepth == 8 ||
         Header.ColorDepth == 24 || Header.ColorDepth == 32 )
   {
      int bytesToRead = sizeof ( char ) * ( *width ) * ( *height ) * Header.ColorDepth / 8;

      // Allocate the image data buffer
      buffer = ( char * ) malloc ( bytesToRead );

      if ( buffer )
      {
         bytesRead = esFileRead ( fp, bytesToRead, buffer );
         esFileClose ( fp );

         return ( buffer );
      }
   }

   return ( NULL );
}
