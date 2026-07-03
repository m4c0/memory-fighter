//
//  sc_loadmatch.c
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 12/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#include "sc_load.h"

#include "gl_main.h"
#include "mp_main.h"
#include "nt_main.h"
#include "sc_base.h"
#include "sc_menu.h"

static GLuint _vbo[3];

static gl_vertex _txt[] = {
    Q(64, 400, 0, 0,   512, 48, 1024, 1024,   1, 0)
};

static int _flag;
static int _optional;

static GLuint _texMsg;
static const char * _lastMsg;

static float _error_timer;

static int _lastFrame = -1;
static int _picked;
static float _ticks;

static sc_screen _next;

static void _prepare(float s) {
    if ((mp_status_flags() & _flag) == _flag) {
        sc_current_set(_next);
        return;
    }
    
    sc_base_prepare();
    
    if (_lastMsg != mp_status_message()) {
        glBindTexture(GL_TEXTURE_2D, _texMsg);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        _lastMsg = mp_status_message();
        nt_render_text(_lastMsg ?: "", 42, 1);
    }
    if (mp_status_flags() & MP_STATUS_ERROR) {
        _error_timer += s;
        if (_error_timer > 3.0) {
            sc_current_set(sc_menu_screen());
            return;
        }
    }

    int frame = (int)((_ticks += s) / 0.2);
    if (_lastFrame == frame) return;
    
    int u = frame % 2;
    int v = frame / 2;
    gl_vertex vtx[] = { Q(160, 60, 512 * u, 512 * v, 320, 320, 512, 512, 0.5, 0) };
    gl_buffer_vtx(_vbo[0], vtx, 0);
    
    gl_vertex cancel[] = { QQ(460, 0, 678, 474, 48, _picked ? 0.3 : 1.0, 0.2) };
    gl_buffer_vtx(_vbo[2], cancel, 0);
    
    _lastFrame = frame;
}

static void _render() {
    sc_base_render();

    gl_ryu_bind();
    gl_draw_elements(_vbo[0], 1);
    
    glBindTexture(GL_TEXTURE_2D, _texMsg);
    gl_draw_elements(_vbo[1], 1);
    
    if (_optional) {
        gl_atlas_bind();
        gl_draw_elements(_vbo[2], 1);
    }
}

static void _set_pick(int p) {
    _picked = p;
}

static void _tap() {
    if (_optional && _picked) {
        nt_play_sound("tap.wav");
        sc_current_set(sc_menu_screen());
    }
}

void sc_load_init() {
    glGenBuffers(3, _vbo);
    glGenTextures(1, &_texMsg);
    
    gl_buffer_vtx(_vbo[1], _txt, 0);
}

sc_screen sc_load_screen(int requiredFlag, int opt, sc_screen next) {
    mp_status_clear_error();
    
    _flag = requiredFlag;
    _next = next;
    _optional = opt;
    
    sc_screen s;
    s.prepare = _prepare;
    s.render = _render;
    s.set_pick = _set_pick;
    s.handle_tap = _tap;
    return s;
}
