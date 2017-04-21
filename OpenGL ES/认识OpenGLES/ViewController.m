//
//  ViewController.m
//  认识OpenGLES
//
//  Created by 李礼光 on 2017/4/21.
//  Copyright © 2017年 李礼光. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()
@property (nonatomic , strong) EAGLContext* mContext;   //渲染上下文
@property (nonatomic , strong) GLKBaseEffect* mEffect;  //效果器
@end

@implementation ViewController
{
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self setupConfig];         //设置配置
    [self uploadVertexArray];   //更新顶点数据
    [self uploadTexture];       //更新纹理
}

#pragma mark - 配置窗口显示
- (void)setupConfig {
    //新建OpenGLES 上下文
    self.mContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2]; //2.0，还有1.0和3.0
    GLKView* view = (GLKView *)self.view; //storyboard记得添加
    view.context = self.mContext;
    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;  //颜色缓冲区格式
    [EAGLContext setCurrentContext:self.mContext];
}

#pragma mark - 加载顶点数据
- (void)uploadVertexArray {
    //顶点数据，前三个是顶点坐标，后面两个是纹理坐标
    
    //每次画都是三角形的形式
    GLfloat squareVertexData[] =
    {
         0.5,  0.5,  0.0f,    1.0f, 1.0f, //右上 B
         0.5, -0.5,  0.0f,    1.0f, 0.0f, //右下 C
        -0.5, -0.5,  0.0f,    0.0f, 0.0f, //左下 D
        
        -0.5,  0.5,  0.0f,    0.0f, 1.0f, //左上 A
        -0.5, -0.5,  0.0f,    0.0f, 0.0f, //左下 D
         0.5,  0.5,  0.0f,    1.0f, 1.0f, //右上 C
    };
    
    
    
    
    //顶点数据缓存
    
    //创建buffero bject用于收集顶点数据
    GLuint buffer;
    
    //将buffer句柄存储在全局变量中
    glGenBuffers(1, &buffer);
    
    //绑定到GL_ARRAY_BUFFER上下文中
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    
    //glBufferData有两部操作
    //1.分配了当前绑定得到glBufferData的缓存空间.保存在RAM中
    //2.从我们的数组内存RAM中拷贝数据到buffero bject中.
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertexData), squareVertexData, GL_STATIC_DRAW);
    
    
    //开启顶点属性
    glEnableVertexAttribArray(GLKVertexAttribPosition); //顶点数据缓存
    
    //告诉OpenGL存储在OB中的顶点数组中的数据格式.也就是要告诉OpenGL如何解释Buffer Object中的数组
    glVertexAttribPointer(GLKVertexAttribPosition,    //顶点数据位置的配置
                          
                          3,                          // 指定每个顶点属性的组件数量。必须为1、2、3或者4。初始值为4。
                                                      // 即3个值组成一个点
                                                      //（如position是由3个（x,y,z）组成，而颜色是4个（r,g,b,a））
                          
                          GL_FLOAT,                   //指定数组中每个组件的数据类型。对应32位浮点数据
                                                      //可用的符号常量有GL_BYTE,
                                                      //             GL_UNSIGNED_BYTE,
                                                      //             GL_SHORT,
                                                      //             GL_UNSIGNED_SHORT,
                                                      //             GL_FIXED,
                                                      //             GL_FLOAT，
                                                      //初始值为GL_FLOAT。
                          
                          GL_FALSE,                   //指定当被访问时，
                                                      //固定点数据值是否应该被归一化（GL_TRUE）或者直接转换为固定点值（GL_FALSE）。
                          
                          
                          sizeof(GLfloat) * 5,        //指定连续顶点属性之间的偏移量。
                                                      //如果为0，那么顶点属性会被理解为：它们是紧密排列在一起的。初始值为0。
                          
                          (GLfloat *)NULL + 0);       //指定第一个组件在数组的第一个顶点属性中的偏移量。
                                                      //该数组与GL_ARRAY_BUFFER绑定，储存于缓冲区中。初始值为0；
    

    
    
    //开启纹理属性
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    //配置纹理属性
    glVertexAttribPointer(GLKVertexAttribTexCoord0,      //纹理内容
                          2,                             //2个值为一组
                          GL_FLOAT,                      //32位浮点数
                          GL_FALSE,                      //默认值
                          sizeof(GLfloat) * 5,           //总浮点长度5
                          (GLfloat *)NULL + 3);          //偏移量3
}

#pragma mark - 加载纹理内容
- (void)uploadTexture {
    //纹理贴图
    NSString* filePath = [[NSBundle mainBundle] pathForResource:@"for_test" ofType:@"jpg"];
    NSDictionary* options = [NSDictionary dictionaryWithObjectsAndKeys:@(1), GLKTextureLoaderOriginBottomLeft, nil];//GLKTextureLoaderOriginBottomLeft 纹理坐标系是相反的
    GLKTextureInfo* textureInfo = [GLKTextureLoader textureWithContentsOfFile:filePath options:options error:nil];
    //着色器
    self.mEffect = [[GLKBaseEffect alloc] init];
    self.mEffect.texture2d0.enabled = GL_TRUE;
    self.mEffect.texture2d0.name = textureInfo.name;
}

/**
 *  渲染场景代码
 */
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    
    glClearColor(0.3f, 0.6f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //启动着色器
    [self.mEffect prepareToDraw];
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}



@end
