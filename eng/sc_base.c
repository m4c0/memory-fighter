//
//  sc_base.c
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 09/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#include "sc_base.h"

#include "gl_main.h"

static GLuint _vbo, _ibo;
static gl_vertex _vtx[] = {
    Q(  0, -320, 0, 0,   320, 320, 300, 300,   1, 0),
    Q(320, -320, 0, 0,   320, 320, 300, 300,   1, 0),
    Q(  0,    0, 0, 0,   320, 320, 300, 300,   1, 0),
    Q(320,    0, 0, 0,   320, 320, 300, 300,   1, 0),
    Q(  0,  320, 0, 0,   320, 320, 300, 300,   1, 0),
    Q(320,  320, 0, 0,   320, 320, 300, 300,   1, 0),
    Q(  0,  640, 0, 0,   320, 320, 300, 300,   1, 0),
    Q(320,  640, 0, 0,   320, 320, 300, 300,   1, 0),
};

void sc_base_init() {
    GLushort idx[600];
    for (int i = 0; i < 600; i += 6) {
        int x = 4 * i / 6;
        idx[i + 0] = 0 + x;
        idx[i + 1] = 1 + x;
        idx[i + 2] = 2 + x;
        idx[i + 3] = 1 + x;
        idx[i + 4] = 2 + x;
        idx[i + 5] = 3 + x;
    }
    
    glGenBuffers(1, &_vbo);
    gl_buffer_vtx(_vbo, _vtx, 1);

    glGenBuffers(1, &_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
}

void sc_base_prepare() {
}

void sc_base_render() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

    gl_atlas_bind();
    gl_draw_elements(_vbo, 8);
}
