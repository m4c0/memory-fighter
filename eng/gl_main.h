//
//  gl_main.h
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 08/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#ifndef __DDRMemory__gl_main__
#define __DDRMemory__gl_main__

#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#else
#include <GLES2/gl2.h>
#endif

#define gl_buffer_vtx(vbo, vtx, sd) gl_buffer_data(vbo, vtx, sizeof(vtx) / sizeof(gl_vertex), sd)

typedef struct gl_vertex {
    GLfloat x, y, a;
    GLfloat p;
    GLushort u, v;
} gl_vertex __attribute__((aligned(4)));

void gl_atlas_bind();
void gl_ryu_bind();
void gl_buffer_data(GLuint vbo, gl_vertex * vtx, GLsizei vtxCount, int static_draw);
void gl_draw_elements(GLuint vbo, GLsizei quads);
void gl_init(float sh);
void gl_shader_enable_attribs();
void gl_shader_set_pick(float f);

#endif /* defined(__DDRMemory__gl_main__) */
