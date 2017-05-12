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


GLuint ESUTIL_API esLoadShader ( GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;

   // Create the shader object
   shader = glCreateShader ( type );

   if ( shader == 0 )
   {
      return 0;
   }

   // 
   glShaderSource ( shader, 1, &shaderSrc, NULL );

   // Compile the shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled )
   {
      GLint infoLen = 0;

      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

      if ( infoLen > 1 )
      {
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


//
///
/// \brief Load a vertex and fragment shader, create a program object, link program.
//         Errors output to log.
/// \param vertShaderSrc Vertex shader source code
/// \param fragShaderSrc Fragment shader source code
/// \return A new program object linked with the vertex/fragment shader pair, 0 on failure
//
GLuint ESUTIL_API esLoadProgram ( const char *vertShaderSrc, const char *fragShaderSrc )
{
   GLuint vertexShader;     //顶点着色器
   GLuint fragmentShader;   //片段着色器
   GLuint programObject;    //程序对象
   GLint linked;            //链接

    
    
    if (strstr(vertShaderSrc, ".glsl")) {
        loadShaderSrc(vertShaderSrc);
        vertexShader = esLoadShader ( GL_VERTEX_SHADER, shaderStr);
    }else{
        vertexShader = esLoadShader ( GL_VERTEX_SHADER, vertShaderSrc );
    }
    if ( vertexShader == 0 ) {
        return 0;
    }
    
    
    if (strstr(fragShaderSrc, ".glsl")) {
        loadShaderSrc(fragShaderSrc);
        fragmentShader = esLoadShader ( GL_FRAGMENT_SHADER, shaderStr);
    }else{
        fragmentShader = esLoadShader ( GL_FRAGMENT_SHADER, fragShaderSrc );
    }
    if ( fragmentShader == 0 ) {
        glDeleteShader ( vertexShader );
        return 0;
    }

   // 创建程序对象
   programObject = glCreateProgram ( );
   if ( programObject == 0 ) {
      return 0;
   }

   // 关联顶点/片段着色器
   glAttachShader ( programObject, vertexShader );
   glAttachShader ( programObject, fragmentShader );

   // 链接(编译)程序
   glLinkProgram ( programObject );

   // 检查链接状态
   glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

   // 检查状态
   if ( !linked )
   {
      GLint infoLen = 0;
      glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

      if ( infoLen > 1 )
      {
         char *infoLog = malloc ( sizeof ( char ) * infoLen );

         glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
         esLogMessage ( "Error linking program:\n%s\n", infoLog );

         free ( infoLog );
      }

      glDeleteProgram ( programObject );
      return 0;
   }

   // 链接完成后删除着色器
   glDeleteShader ( vertexShader );
   glDeleteShader ( fragmentShader );

   return programObject;
}
