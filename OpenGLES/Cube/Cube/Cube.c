// The MIT License (MIT)
//
// Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//
// Book:      OpenGL(R) ES 3.0 Programming Guide, 2nd Edition
// Authors:   Dan Ginsburg, Budirijanto Purnomo, Dave Shreiner, Aaftab Munshi
// ISBN-10:   0-321-93388-5
// ISBN-13:   978-0-321-93388-1
// Publisher: Addison-Wesley Professional
// URLs:      http://www.opengles-book.com
//            http://my.safaribooksonline.com/book/animation-and-3d/9780133440133
//
// Simple_VertexShader.c
//
//    This is a simple example that draws a rotating cube in perspective
//    using a vertex shader to transform the object
//
#include <stdlib.h>
#include <stdio.h>
#include "esUtil.h"

/*
GLfloat vertices[] = {
    //底面
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f, -0.5f,
    
    //顶面
    -0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
    
    //背面
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
    
    //正面
    -0.5f, -0.5f, 0.5f,
    -0.5f,  0.5f, 0.5f,
     0.5f,  0.5f, 0.5f,
     0.5f, -0.5f, 0.5f,
    
    //左面
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    
    //右面
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
};


GLuint indices[] = {
    0, 2, 1,
    0, 3, 2,
    4, 5, 6,
    4, 6, 7,
    8, 9, 10,
    8, 10, 11,
    12, 15, 14,
    12, 14, 13,
    16, 17, 18,
    16, 18, 19,
    20, 23, 22,
    20, 22, 21
};

*/


GLfloat vertices[] = {
    //底部4个点
    -0.5f, -0.5f,  0.5f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f,    //下左下  0
    -0.5f, -0.5f, -0.5f,  1.0f,  0.0f, 0.0f, 1.0f, 1.0f,    //下左上  1
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f, 0.0f, 1.0f, 1.0f,    //下右上  2
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f, 0.0f, 1.0f, 1.0f,    //下右下  3
    
    //顶部四个点
    -0.5f,  0.5f,  0.5f,  1.0f,  0.0f, 0.0f, 1.0f, 1.0f,    //上左下  4
    -0.5f,  0.5f, -0.5f,  1.0f,  0.0f, 0.0f, 1.0f, 1.0f,    //上左上  5
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f,    //上右上  6
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f, 0.0f, 1.0f, 1.0f,    //上右下  7
};


GLuint indices[] = {
    //正面
    0, 3, 7,
    0, 7, 4,
    //背面
    1, 5, 6,
    1, 6, 2,
    //顶面
    4, 6, 7,
    4, 5, 6,
    //底面
    0, 1, 2,
    0, 2, 3,
    //右面
    3, 7, 6,
    3, 6, 2,
    //左面
    0, 5, 1,
    0, 4, 5,
};


typedef struct {
    GLuint programObject;
    GLint  mvpLoc;

    GLfloat  *vertices;
    GLuint   *indices;
    
    GLuint vbo[2];
    GLuint vao;

    GLfloat   angle;
    ESMatrix  mvpMatrix;
} UserData;


int Init ( ESContext *esContext ){
    UserData *userData = esContext->userData;
    userData->programObject = esLoadProgram ( "vShader.glsl", "fShader.glsl" );

    userData->mvpLoc = glGetUniformLocation ( userData->programObject, "u_mvpMatrix" );
    userData->angle = 45.0f;

    
    glGenBuffers(2, userData->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, userData->vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, userData->vbo[1] );//索引
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof ( indices ) , indices, GL_STATIC_DRAW );

    
    glBindBuffer(1, userData->vao);
    glBindVertexArray(userData->vao);
    
    glVertexAttribPointer ( 0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),  (GLfloat *)NULL + 0 );
    glEnableVertexAttribArray ( 0 );
    
    glVertexAttribPointer ( 1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),  (GLfloat *)NULL + 4 );
    glEnableVertexAttribArray ( 1 );
    
    glBindVertexArray(0);
    
    glEnable(GL_DEPTH_TEST);
    
    glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
    return GL_TRUE;
}



void Update ( ESContext *esContext, float deltaTime ) {
    
   UserData *userData = esContext->userData;
   ESMatrix perspective;        //正射投影矩阵
   ESMatrix modelview;          //模型矩阵
   float    aspect;

   userData->angle += ( deltaTime * 40.0f );

   if ( userData->angle >= 360.0f ) {
      userData->angle -= 360.0f;
   }

   aspect = ( GLfloat ) esContext->width / ( GLfloat ) esContext->height;

    //每次更新一个画面
   esMatrixLoadIdentity ( &perspective );
   esPerspective ( &perspective, 30.0f, aspect, 5.0f, 20.0f );
   esMatrixLoadIdentity ( &modelview );
   esTranslate ( &modelview, 0.0, 0.0, -15.0 );
   esRotate ( &modelview, userData->angle, 1.0, 1.0, 1.0 );
   esMatrixMultiply ( &userData->mvpMatrix, &modelview, &perspective );
    
}


void Draw ( ESContext *esContext ) {
    UserData *userData = esContext->userData;
    glViewport ( 0, 0, esContext->width, esContext->height );
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUseProgram ( userData->programObject );

    //userData->mvpMatrix 在update中更新,所以需要在这里重新设置矩阵,然后draw
    glUniformMatrix4fv ( userData->mvpLoc, 1, GL_FALSE, ( GLfloat * ) &userData->mvpMatrix.m[0][0] );

    glDrawElements ( GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT,  (GLfloat *)NULL + 0 );
}


void Shutdown ( ESContext *esContext ) {
   UserData *userData = esContext->userData;

    if ( userData->vertices != NULL ) {
      free ( userData->vertices );
   }

   if ( userData->indices != NULL ) {
      free ( userData->indices );
   }

   glDeleteProgram ( userData->programObject );
}


int esMain ( ESContext *esContext ) {
   esContext->userData = malloc ( sizeof ( UserData ) );
    
   esCreateWindow ( esContext, "Cube", 320, 240, ES_WINDOW_RGB | ES_WINDOW_DEPTH );
   if ( !Init ( esContext ) ){
      return GL_FALSE;
   }

   esRegisterShutdownFunc ( esContext, Shutdown );
   esRegisterUpdateFunc ( esContext, Update );
   esRegisterDrawFunc ( esContext, Draw );

   return GL_TRUE;
}

