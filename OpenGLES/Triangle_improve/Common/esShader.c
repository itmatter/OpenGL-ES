///
//  Includes
//
#include "esUtil.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//C语言不能直接返回一个数组,所以使用全局变量的方式.
char shaderStr[1024];

void loadShaderSrc(const char *fileName) {
    FILE *v_fp = fopen(GetBundleFileName(fileName), "r");
    memset( shaderStr, 0, sizeof( shaderStr ) );
    if(v_fp) {
        char line[1024];
        while(1) {
            int len = 0;
            line[0] = '\0';
            fgets(line, sizeof(line), v_fp);
            if(len == sizeof(line)-1) continue;
            strcat(shaderStr,line);
            if(feof(v_fp)) break;
        }
        fclose(v_fp);
    }
}



GLuint ESUTIL_API esLoadShader ( GLenum type, const char *shaderSrc ) {
    GLuint shader;       //着色器句柄
    GLint compiled;      //编译
    
    shader = glCreateShader ( type );
    if ( shader == 0 ) {
        return 0;
    }
    
    glShaderSource ( shader,1,&shaderSrc,NULL );
    glCompileShader ( shader );
    
    glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );
    if ( !compiled ) {
        GLint infoLen = 0;
        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
        
        if ( infoLen > 1 ){
            char *infoLog = malloc ( sizeof ( char ) * infoLen );
            glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
            esLogMessage ( "Error compiling shader:\n%s\n", infoLog );
            free ( infoLog );
        }
        glDeleteShader ( shader );
        return 0;
    }
    return shader;
}


GLuint ESUTIL_API esLoadProgram ( const char *vertShaderSrc, const char *fragShaderSrc ) {
   GLuint vShader;     //顶点着色器
   GLuint fShader;   //片段着色器
   GLuint programObject;    //程序对象
   GLint linked;            //链接

    
    if (strstr(vertShaderSrc, ".glsl")) {
        loadShaderSrc(vertShaderSrc);
        vShader = esLoadShader ( GL_VERTEX_SHADER, shaderStr);
    }else{
        vShader = esLoadShader ( GL_VERTEX_SHADER, vertShaderSrc );
    }
    if ( vShader == 0 ) {
        return 0;
    }
    
    
    if (strstr(fragShaderSrc, ".glsl")) {
        loadShaderSrc(fragShaderSrc);
        fShader = esLoadShader ( GL_FRAGMENT_SHADER, shaderStr);
    }else{
        fShader = esLoadShader ( GL_FRAGMENT_SHADER, fragShaderSrc );
    }
    if ( fShader == 0 ) {
        glDeleteShader ( vShader );
        return 0;
    }

   // 创建程序对象
   programObject = glCreateProgram ( );
   if ( programObject == 0 ) {
      return 0;
   }
   // 关联顶点/片段着色器
   glAttachShader ( programObject, vShader );
   glAttachShader ( programObject, fShader );
   // 链接(编译)程序
   glLinkProgram ( programObject );
   // 检查链接状态
   glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );
   // 检查状态
   if ( !linked ){
      GLint infoLen = 0;
      glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );
      if ( infoLen > 1 ){
         char *infoLog = malloc ( sizeof ( char ) * infoLen );

         glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
         esLogMessage ( "Error linking program:\n%s\n", infoLog );

         free ( infoLog );
      }

      glDeleteProgram ( programObject );
      return 0;
   }

   // 链接完成后删除着色器
   glDeleteShader ( vShader );
   glDeleteShader ( fShader );

   return programObject;
}

