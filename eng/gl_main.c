//
//  gl_main.c
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 08/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#include "gl_main.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "nt_main.h"

static GLuint _atlas, _ryu;
static GLuint _shader;

void gl_atlas_bind() {
    glBindTexture(GL_TEXTURE_2D, _atlas);
}

void gl_ryu_bind() {
    glBindTexture(GL_TEXTURE_2D, _ryu);
}

void gl_atlas_load() {
    glGenTextures(1, &_atlas);
    glBindTexture(GL_TEXTURE_2D, _atlas);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    nt_load_texture("atlas.png");
}

void gl_ryu_load() {
    glGenTextures(1, &_ryu);
    glBindTexture(GL_TEXTURE_2D, _ryu);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    nt_load_texture("ryu.png");
}

void gl_buffer_data(GLuint vbo, gl_vertex * vtx, GLsizei vtxCount, int static_draw) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vtxCount * sizeof(gl_vertex), vtx, static_draw ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
}

void gl_draw_elements(GLuint vbo, GLsizei quads) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    gl_shader_enable_attribs();
    glDrawElements(GL_TRIANGLES, quads * 6, GL_UNSIGNED_SHORT, 0);
}

GLuint gl_shader_compile(const GLchar * filename, GLenum type) {
    GLuint shader = glCreateShader(type);
    if (!shader) abort();
    nt_load_shader_source(shader, filename);
    glCompileShader(shader);

#ifdef DEBUG
    GLint logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(shader, logLength, &logLength, log);
        fprintf(stderr, "Warning for shader:\n%s", log);
        free(log);
    }
#endif
    
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        glDeleteShader(shader);
        abort();
    }

    return shader;
}

void gl_shader_load(float sh) {
    GLuint vsh = gl_shader_compile("shader.vsh", GL_VERTEX_SHADER);
    GLuint fsh = gl_shader_compile("shader.fsh", GL_FRAGMENT_SHADER);
    
    _shader = glCreateProgram();
    glAttachShader(_shader, vsh);
    glAttachShader(_shader, fsh);
    
    glBindAttribLocation(_shader, 0, "av2_pos");
    glBindAttribLocation(_shader, 1, "af_alpha");
    glBindAttribLocation(_shader, 2, "av2_tex");
    glBindAttribLocation(_shader, 3, "af_pick");
    
    glLinkProgram(_shader);

#ifdef DEBUG
    GLint logLength;
    glGetProgramiv(_shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(_shader, logLength, &logLength, log);
        fprintf(stderr, "Warning for shader:\n%s", log);
        free(log);
    }
#endif

    GLint status;
    glGetProgramiv(_shader, GL_LINK_STATUS, &status);
    if (status == 0) abort();

    glDetachShader(_shader, vsh);
    glDetachShader(_shader, fsh);
    
    glDeleteShader(vsh);
    glDeleteShader(fsh);
    
    glUseProgram(_shader);
    glUniform1f(glGetUniformLocation(_shader, "uf_h"), sh);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
}

#define o(x) (void *)offsetof(gl_vertex, x)
void gl_shader_enable_attribs() {
    GLsizei s = sizeof(gl_vertex);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, 0, s, o(x));
    glVertexAttribPointer(1, 1, GL_FLOAT, 0, s, o(a));
    glVertexAttribPointer(2, 2, GL_UNSIGNED_SHORT, 0, s, o(u));
    glVertexAttribPointer(3, 1, GL_FLOAT, 0, s, o(p));
}

void gl_shader_set_pick(float f) {
    glUniform1f(glGetUniformLocation(_shader, "uf_pick"), f);
}

void gl_init(float sh) {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDisable(GL_DEPTH_TEST);
    
    gl_atlas_load();
    gl_ryu_load();
    gl_shader_load(sh);
}