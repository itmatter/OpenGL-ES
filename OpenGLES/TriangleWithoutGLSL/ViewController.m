//
//  ViewController.m
//  TriangleWithoutGLSL
//
//  Created by 李礼光 on 2017/5/8.
//  Copyright © 2017年 李礼光. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()
@property (strong, nonatomic) EAGLContext *mContext;
@property (strong, nonatomic) GLKBaseEffect *mEffect;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];


    //新建OpenGLES 上下文
    self.mContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    
    GLKView* view = (GLKView *)self.view;
    view.context = self.mContext;
    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [EAGLContext setCurrentContext:self.mContext];
    glEnable(GL_DEPTH_TEST);
    
    //新的图形
    [self renderNew];





}


- (void)renderNew {
    //顶点数据，前三个是顶点坐标， 中间三个是顶点颜色，    最后两个是纹理坐标
    GLfloat attrArr[] =
    {
        -0.5f,  0.5f, 0.0f,       0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.0f,       0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,       1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f,       0.0f, 0.0f, 1.0f,
    };
    //顶点索引
    GLuint indices[] = {
        0, 1, 2,
        1, 2, 3,
    };
    
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(attrArr), attrArr, GL_STATIC_DRAW);
    
    GLuint index;
    glGenBuffers(1, &index);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLfloat *)NULL);
    //顶点颜色
    glEnableVertexAttribArray(GLKVertexAttribColor);
    glVertexAttribPointer(GLKVertexAttribColor, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLfloat *)NULL + 3);
    
    //着色器
    self.mEffect = [[GLKBaseEffect alloc] init];
    
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    glClearColor(0.3f, 0.6f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    [self.mEffect prepareToDraw];
    
}

@end
