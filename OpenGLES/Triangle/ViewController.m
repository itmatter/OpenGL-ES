//
//  ViewController.m
//  OpenGLES
//
//  Created by 李礼光 on 2017/5/4.
//  Copyright © 2017年 李礼光. All rights reserved.
//

#import "ViewController.h"
#include "esUtil.h"

extern void esMain( ESContext *esContext );

@interface ViewController () {
    ESContext _esContext;
}
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    
    //创建指定版本的渲染上下文(ES3.0)
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if (!self.context){
        NSLog(@"Failed to create ES context");
    }
    //创建一个view指向self.view
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.frame = [UIScreen mainScreen].bounds;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;    //颜色缓冲区格式
    [self setupGL];
    
    
}

#pragma mark - 关闭OpenGLES
- (void)dealloc {
    [self tearDownGL];
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        [self tearDownGL];
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }
}


#pragma mark - 渲染
- (void)setupGL {
    //设置当前线程渲染上下文
    [EAGLContext setCurrentContext:self.context];
    //内存地址清零
    memset( &_esContext, 0, sizeof( _esContext ) );
    NSLog(@"sizeof(_esContext) : %lu",sizeof(_esContext));//56
    
    //传入渲染地址
    esMain( &_esContext );
}

- (void)tearDownGL {
    [EAGLContext setCurrentContext:self.context];
    if ( _esContext.shutdownFunc ){
        _esContext.shutdownFunc( &_esContext );
    }
}


- (void)update {
    if ( _esContext.updateFunc ) {
        _esContext.updateFunc( &_esContext, self.timeSinceLastUpdate );
    }
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    _esContext.width = (int)view.drawableWidth;
    _esContext.height = (int)view.drawableHeight;
    
    if ( _esContext.drawFunc ){
        _esContext.drawFunc( &_esContext );
    }
}

@end
