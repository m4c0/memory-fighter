//
//  gl_pick.c
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 09/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#include "gl_pick.h"

#include <stdlib.h>

#include "gl_main.h"
#include "lg_main.h"

static GLuint _fb, _rb;
static int _sw, _sh;

void gl_pick_init(int sw, int sh) {
    glGenFramebuffers(1, &_fb);
    glBindFramebuffer(GL_FRAMEBUFFER, _fb);
    
    glGenRenderbuffers(1, &_rb);
    glBindRenderbuffer(GL_RENDERBUFFER, _rb);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB5_A1, sw, sh);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _rb);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        lg_error("%x\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        abort();
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    _sw = sw;
    _sh = sh;
}

uint8_t gl_pick_red(int x, int y, void (*render)()) {
    glBindFramebuffer(GL_FRAMEBUFFER, _fb);

#ifdef DEBUG
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        abort();
    }
#endif

    glViewport(0, 0, _sw, _sh);
    glClear(GL_COLOR_BUFFER_BIT);

    gl_shader_set_pick(1);
    render();
    gl_shader_set_pick(0);
    
    GLubyte pix[4];
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pix);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return pix[0];
}
