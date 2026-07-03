//
//  lg_main.h
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 11/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#ifndef DDRMemory_lg_main_h
#define DDRMemory_lg_main_h

#ifdef __APPLE__
#include <stdio.h>
#define lg_error(fmt...) printf(fmt); printf("\n");
#else
#include <android/log.h>
#define lg_error(fmt...) __android_log_print(ANDROID_LOG_ERROR, "DDRMemory", fmt)
#endif

#define lg_gl(x) x; lg_check_gl()
#define lg_check_gl() lg_check_gl_raw(__FILE__, __LINE__)

void lg_check_gl_raw(const char * f, int l);

#endif
