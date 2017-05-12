//
//  Triangle.c
//  OpenGLES
//
//  Created by 李礼光 on 2017/5/5.
//  Copyright © 2017年 李礼光. All rights reserved.
//

#include "esUtil.h"

#define VERTEX_POS_INDX       0
#define VERTEX_COLOR_INDX     1

#define VERTEX_POS_SIZE       3 // x, y and z
#define VERTEX_COLOR_SIZE     4 // r, g, b, and a



#define VERTEX_STRIDE       ( sizeof(GLfloat)*( VERTEX_POS_SIZE + VERTEX_COLOR_SIZE ) )

typedef struct {
    GLuint programObject;
    GLuint vboIds[2];       //坐标,索引
    GLuint vaoId;
} UserData;


int Init ( ESContext *esContext ) {
    UserData *userData = esContext->userData;//用来保存程序对象
    GLuint programObject;    //程序对象
    //创建一个对象
    programObject = esLoadProgram("Triangle_V_Shader.glsl", "Triangle_F_Shader.glsl");
    //存储程序对象
    userData->programObject = programObject;
    
    
    
    GLfloat vertices[4 * ( VERTEX_POS_SIZE + VERTEX_COLOR_SIZE )] = {
         0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,    // 右上角
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,    // 右下角
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,    // 左下角
        -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,    // 左上角
    };
    GLushort indices[6] = {
        0, 1, 2,
        0, 2, 3,
    };
    
    
    //坐标颜色,索引
    glGenBuffers( 2, userData->vboIds );
    glBindBuffer( GL_ARRAY_BUFFER, userData->vboIds[0] );//坐标颜色
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices) , vertices, GL_STATIC_DRAW );
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, userData->vboIds[1] );//索引
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof ( indices ) , indices, GL_STATIC_DRAW );
    
    //VAO
    glGenBuffers(1, &userData->vaoId);
    glBindVertexArray(userData->vaoId);
    
    // 位置属性
    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, VERTEX_STRIDE, (GLfloat *)NULL + 0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(VERTEX_COLOR_INDX, VERTEX_COLOR_SIZE, GL_FLOAT, GL_FALSE, VERTEX_STRIDE, (GLfloat *)NULL + 3);
    glEnableVertexAttribArray(1);
    
    
    //注意,这里配置属性的思考是,坐标加上颜色为一个整体, 所以配置位置和颜色的时候,步长STRIDE都是一样的
    
    glLineWidth(10);
    // 重置VAO
    glBindVertexArray(0);
    glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
    return TRUE;
}



//使用VAO的方式
void DrawTriangleWithVAO ( ESContext *esContext ) {
    
    UserData *userData = esContext->userData;
    glUseProgram ( userData->programObject );
    glViewport ( 0, 0, esContext->width, esContext->height );
    glClear ( GL_COLOR_BUFFER_BIT );
    
    glBindVertexArray ( userData->vaoId );
    glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0 );
    glBindVertexArray ( 0 );
    
}




void Draw ( ESContext *esContext ) {
    DrawTriangleWithVAO(esContext);
}




void Shutdown ( ESContext *esContext ) {
    UserData *userData = esContext->userData;
    glDeleteProgram ( userData->programObject );
}



int esMain ( ESContext *esContext ) {
    
    esContext->userData = malloc ( sizeof ( UserData ) );
    //针对于非APPLE
    esCreateWindow ( esContext, "Hello Triangle", 320, 240, ES_WINDOW_RGB );
    if ( !Init ( esContext ) ){
        return GL_FALSE;
    }
    esRegisterShutdownFunc ( esContext, Shutdown );
    esRegisterDrawFunc ( esContext, Draw );
    return GL_TRUE;
}
