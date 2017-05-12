//
//  Cube1.c
//  OpenGLES
//
//  Created by 李礼光 on 2017/5/12.
//  Copyright © 2017年 李礼光. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include "esUtil1.h"

//这里面纹理怎么覆盖六个面还需要重新考虑,或者其他方式设计,这里就暂时不浪费更多时间
GLfloat vertices[] = {
    //底部4个点
    -0.5f, -0.5f,  0.5f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f,    0.0, 0.0,//下左下  0
    -0.5f, -0.5f, -0.5f,  1.0f,  0.0f, 0.0f, 1.0f, 1.0f,    0.0, 1.0,//下左上  1
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f, 0.0f, 1.0f, 1.0f,    1.0, 1.0,//下右上  2
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f, 0.0f, 1.0f, 1.0f,    1.0, 0.0,//下右下  3
    
    //顶部四个点
    -0.5f,  0.5f,  0.5f,  1.0f,  0.0f, 0.0f, 1.0f, 1.0f,    0.0, 0.0,//上左下  4
    -0.5f,  0.5f, -0.5f,  1.0f,  0.0f, 0.0f, 1.0f, 1.0f,    0.0, 1.0,//上左上  5
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f,    1.0, 1.0,//上右上  6
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f, 0.0f, 1.0f, 1.0f,    1.0, 0.0,//上右下  7
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
    
    glVertexAttribPointer ( 0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat),  (GLfloat *)NULL + 0 );
    glEnableVertexAttribArray ( 0 );
    
    glVertexAttribPointer ( 1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat),  (GLfloat *)NULL + 4 );
    glEnableVertexAttribArray ( 1 );
    
    glVertexAttribPointer ( 2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat),  (GLfloat *)NULL + 8 );
    glEnableVertexAttribArray ( 2 );

    
    SetupTexture("texture0.jpg");
    
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
   esPerspective ( &perspective, 20.0f, aspect, 5.0f, 20.0f );
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

