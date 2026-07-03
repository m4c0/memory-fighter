//
//  lg_main.c
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 11/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#include "lg_main.h"

#include "gl_main.h"

void lg_check_gl_raw(const char * f, int l) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) lg_error("GL Error %x at %s:%d", err, f, l);
}
