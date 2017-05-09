//
//  ViewController.m
//  Texture
//
//  Created by 李礼光 on 2017/5/9.
//  Copyright © 2017年 李礼光. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()
@property (strong, nonatomic) EAGLContext *mContext;            //创建上下文
@property (strong, nonatomic) GLKBaseEffect *mEffect;           //效果器
@end

@implementation ViewController

- (EAGLContext *)mContext {
    if (_mContext == nil) {
        _mContext = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES3];
        if (_mContext == nil) {
            NSLog(@"can't not creater ES 3.0 Context.");
            _mContext = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES2];
        }
    }
    return _mContext;
}

- (GLKBaseEffect *)mEffect {
    if (_mEffect == nil) {
        _mEffect = [[GLKBaseEffect alloc]init];
        //这里面可以设置效果器的一些默认效果
    }
    return _mEffect;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    //新建OpenGLES 上下文
    GLKView* view = (GLKView *)self.view;
    view.context = self.mContext;
    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    [EAGLContext setCurrentContext:self.mContext];
    glEnable(GL_DEPTH_TEST);

    //新的图形
    [self renderNew];

}

#pragma mark - 纹理属性存在于顶点属性当中
- (void)renderNew {
    //顶点数据，前三个是顶点坐标， 中间三个是顶点颜色，    最后两个是纹理坐标
    GLfloat attrArr[] = {
        //  x,     y,    z,     r,    g,    b,    a,       s,    t,
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f,
         0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,    1.0f, 0.0f,
    };
    //顶点索引
    GLuint indices[] = {
        0, 1, 2,
        1, 2, 3,
    };
    
    //VBO
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(attrArr), attrArr, GL_STATIC_DRAW);
    
    //IBO
    GLuint index;
    glGenBuffers(1, &index);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    //坐标
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, (GLfloat *)NULL);
    //顶点颜色
    glEnableVertexAttribArray(GLKVertexAttribColor);
    glVertexAttribPointer(GLKVertexAttribColor, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, (GLfloat *)NULL + 3);
   
    //纹理属性0
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, (GLfloat *)NULL + 3 + 4);
    
    //纹理属性1
    glEnableVertexAttribArray(GLKVertexAttribTexCoord1);
    glVertexAttribPointer(GLKVertexAttribTexCoord1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, (GLfloat *)NULL + 3 + 4);
    
    //加载纹理0
    NSString* filePath0 = [[NSBundle mainBundle] pathForResource:@"texture0.jpg" ofType:@""];
    NSDictionary* options0 = [NSDictionary dictionaryWithObjectsAndKeys:@(1), GLKTextureLoaderGenerateMipmaps, nil];
    GLKTextureInfo* textureInfo0 = [GLKTextureLoader textureWithContentsOfFile:filePath0 options:options0 error:nil];
    self.mEffect.texture2d0.enabled = GL_TRUE;
    self.mEffect.texture2d0.name = textureInfo0.name;
    
    //加载纹理1
    NSString* filePath1 = [[NSBundle mainBundle] pathForResource:@"texture1.png" ofType:@""];
    NSDictionary* options1 = [NSDictionary dictionaryWithObjectsAndKeys:@(1), GLKTextureLoaderOriginBottomLeft, nil];
    GLKTextureInfo* textureInfo1 = [GLKTextureLoader textureWithContentsOfFile:filePath1 options:options1 error:nil];
    self.mEffect.texture2d1.enabled = GL_TRUE;
    self.mEffect.texture2d1.name = textureInfo1.name;


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
