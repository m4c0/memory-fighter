//
//  sc_menu.c
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 08/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#include "sc_menu.h"

#include "gl_main.h"
#include "mp_main.h"
#include "nt_main.h"
#include "sc_base.h"
#include "sc_curmatch.h"
#include "sc_game.h"
#include "sc_load.h"
#include "sc_newmatch.h"

static GLuint _vbo[2];
static gl_vertex _vtx[] = {
    QQ(-60, 300, 0, 390, 170, 1, 0),
};

static int _picked;

static void _set_pick(int p) {
    _picked = p;
    
    gl_vertex _btn[] = {
        QQ(160, 472, 774, 472, 54, (_picked != 4) ? 1.0 : 0.3, 0.8),
        QQ(270, 0, 300, 472, 54, (_picked != 1) ? 1.0 : 0.3, 0.2),
        QQ(380, 0, 354, 472, 54, (_picked != 2) ? 1.0 : 0.3, 0.4),
        QQ(490, 0, 408, 472, 54, (_picked != 3) ? 1.0 : 0.3, 0.6),
    };
    gl_buffer_vtx(_vbo[1], _btn, 0);
}

static void _prepare(float s) {
    sc_base_prepare();
    _set_pick(_picked);
}

void sc_menu_init() {
    glGenBuffers(3, _vbo);
    
    gl_buffer_vtx(_vbo[0], _vtx, 1);

    _picked = 0;
}

static void _render() {
    sc_base_render();
    
    gl_draw_elements(_vbo[0], 1);
    gl_draw_elements(_vbo[1], 4);
}

static void _tap() {
    if (_picked != 0) nt_play_sound("tap.wav");
    switch (_picked) {
        case 1: sc_current_set(sc_newmatch_screen()); break;
        case 2: sc_current_set(sc_curmatch_screen(0)); break;
        case 3: sc_current_set(sc_curmatch_screen(1)); break;
        case 4:
            mp_match_start_alone();
            sc_current_set(sc_game_screen());
            break;
        default: break;
    }
}

sc_screen sc_menu_screen() {
    sc_screen s;
    s.prepare = _prepare;
    s.render = _render;
    s.set_pick = _set_pick;
    s.handle_tap = _tap;
    return sc_load_screen(MP_STATUS_LOGGED_IN, 0, s);
}
