//
//  FileWrapper.m
//  OpenGLES
//
//  Created by 李礼光 on 2017/5/12.
//  Copyright © 2017年 李礼光. All rights reserved.
//

#import "esFileWrapper.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <OpenGLES/ES3/gl.h>
///
/// \brief Given a fileName, convert into a path that can be used to open from
/// the mainBundle
/// \param fileName Name of file to convert to mainBundle path
/// \return Path that can be used to fopen() from the mainBundle
///
const char *GetBundleFileName( const char *fileName )
{
    NSString* fileNameNS = [NSString stringWithUTF8String:fileName];
    NSString* baseName = [fileNameNS stringByDeletingPathExtension];
    NSString* extension = [fileNameNS pathExtension];
    NSString *path = [[NSBundle mainBundle] pathForResource: baseName ofType: extension ];
    fileName = [path cStringUsingEncoding:1];
    
    return fileName;
}


void SetupTexture(const char *texSource) {
    NSString *fileName = [NSString stringWithUTF8String:texSource];
    // 1获取图片的CGImageRef
    CGImageRef spriteImage = [UIImage imageNamed:fileName].CGImage;
    if (!spriteImage) {
        NSLog(@"Failed to load image %@", fileName);
        exit(1);
    }
    // 2 读取图片的大小
    size_t width = CGImageGetWidth(spriteImage);
    size_t height = CGImageGetHeight(spriteImage);
    GLubyte * spriteData = (GLubyte *) calloc(width * height * 4, sizeof(GLubyte)); //rgba共4个byte
    CGContextRef spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width*4,
                                                       CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
    
    CGContextDrawImage(spriteContext, CGRectMake(0, 0, width, height), spriteImage);
    
    CGContextRelease(spriteContext);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    float fw = width, fh = height;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fw, fh, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    free(spriteData);
}







