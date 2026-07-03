//
//  sc_game.c
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 12/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#include "sc_game.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "gl_main.h"
#include "mp_main.h"
#include "nt_main.h"
#include "sc_base.h"
#include "sc_game_end.h"
#include "sc_game_repeat.h"
#include "sc_load.h"

#define R nt_random(4)

static int _init;
static float _timer;
static GLuint _vbo;

static void _prepare(float s) {
    mp_match m = *mp_match_get();
    if (strlen(m.mid) && !_init) {
        if (m.ended) {
            sc_current_set(sc_game_end_screen());
            return;
        } else if (m.data.count == 0) {
            m.data.sequence[0] = R;
            m.data.sequence[1] = R;
            m.data.sequence[2] = R;
            m.data.count = 3;
            m.data.life.p1 = m.data.life.p2 = 10;
            nt_play_sound("start.wav");
            mp_match_set(m);
        } else if ((m.data.count == 255) || !m.data.life.p1 || !m.data.life.p2) {
            sc_current_set(sc_game_end_screen());
            return;
        } else {
            m.data.sequence[m.data.count++] = R;
            nt_play_sound("start.wav");
            mp_match_set(m);
        }
        _init = 1;
    }
    _timer += s;
    
    float alpha = 1.0;
    if (_timer < 0.5) {
        alpha = _timer * 2.0;
    } else if (_timer > 3.0) {
        alpha = 0;
        sc_current_set(sc_game_repeat_screen());
    } else if (_timer > 2.5) {
        alpha = 1.0 - (_timer - 2.5) * 2.0;
    }
    gl_vertex vtx[] = {
        QQ(275, 0, 760, 300, 90, alpha, 0)
    };
    gl_buffer_vtx(_vbo, vtx, 0);
}

static void _render() {
    gl_atlas_bind();
    gl_draw_elements(_vbo, 1);
}

static void _set_pick(int p) {
}

static void _tap() {
}

void sc_game_init() {
    glGenBuffers(1, &_vbo);
}

sc_screen sc_game_screen() {
    _timer = 0;
    _init = 0;
    
    sc_screen s;
    s.prepare = _prepare;
    s.render = _render;
    s.set_pick = _set_pick;
    s.handle_tap = _tap;
    return s;
}
