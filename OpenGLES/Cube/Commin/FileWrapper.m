//
//  FileWrapper.m
//  OpenGLES
//
//  Created by 李礼光 on 2017/5/12.
//  Copyright © 2017年 李礼光. All rights reserved.
//
#import "FileWrapper.h"
#import <Foundation/Foundation.h>

const char *GetBundleFileName( const char *fileName )
{
    NSString* fileNameNS = [NSString stringWithUTF8String:fileName];
    NSString* baseName = [fileNameNS stringByDeletingPathExtension];
    NSString* extension = [fileNameNS pathExtension];
    NSString *path = [[NSBundle mainBundle] pathForResource: baseName ofType: extension ];
    fileName = [path cStringUsingEncoding:1];
    
    return fileName;
}
