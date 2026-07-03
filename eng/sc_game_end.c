//
//  sc_game_end.c
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 15/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#include "sc_game_end.h"

#include <stdio.h>
#include <string.h>

#include "gl_main.h"
#include "mp_main.h"
#include "nt_main.h"
#include "sc_base.h"
#include "sc_menu.h"

static int _picked;

static GLuint _txt;
static GLuint _vbo[2];

static void _prepare(float s) {
    const mp_match * m = mp_match_get();
    
    float p1x = -320.0 * (1.0 - (float)m->data.life.p1 / 10.0);
    float p2x = +300.0 * (1.0 - (float)m->data.life.p2 / 10.0);
    gl_vertex vtx[] = {
        // P1 Bar
        Q(-95,       70, 474, 679, 450, 48, 450, 48, 1, 0),
        Q(-95 + p1x, 70, 474, 726, 450, 48, 450, 48, 1, 0),
        
        // P2 Bar
        Q(305,       225, 474 + 450, 679 + 48, 450, 48, -450, -48, 1, 0),
        Q(305 + p2x, 225, 474 + 450, 726 + 48, 450, 48, -450, -48, 1, 0),
        
        // Buttons
        QQ(500, 358, 226, 186, 56, _picked == 1 ? 0.3 : 1.0, 0.2),
        QQ(600, 552, 226, 172, 56, _picked == 2 ? 0.3 : 1.0, 0.4),
        
        // Won
        Q(360, 50, 640, 936, 250, 88, 250, 88, m->data.life.p1 ? 1 : 0, 0),
        Q(40, 205, 640, 936, 250, 88, 250, 88, m->data.life.p2 ? 1 : 0, 0),
    };
    gl_buffer_vtx(_vbo[0], vtx, 1);
}

static void _render() {
    gl_atlas_bind();
    gl_draw_elements(_vbo[0], 8);
    
    glBindTexture(GL_TEXTURE_2D, _txt);
    gl_draw_elements(_vbo[1], 3);
}

static void _set_pick(int p) {
    _picked = p;
}

static void _tap() {
    if (_picked != 0) nt_play_sound("tap.wav");
    switch (_picked) {
        case 1: {
            const char * fmt;
            const mp_match * m = mp_match_get();
            const char * opp = m->i_am_p1 ? m->p2_name : m->p1_name;
            if ((m->data.life.p1 && m->i_am_p1) || (m->data.life.p2 && !m->i_am_p1)) {
                fmt = "I beat %s after %d rounds. Dare to take the challenge?";
            } else {
                fmt = "I lost to %s after %d rounds. Dare to take a challenge?";
            }
            
            char msg[1024];
            sprintf(msg, fmt, opp, m->data.count - 2);
            nt_share_screen(msg);
            break;
        }
        case 2: sc_current_set(sc_menu_screen()); break;
        default: break;
    }
}

void sc_game_end_init() {
    glGenBuffers(2, _vbo);
    glGenTextures(1, &_txt);

    static gl_vertex nvtx[] = {
        Q(0, 115, 0, 0, 320, 30, 1024, 192, 1, 0),
        Q(340, 200, 0, 192, 320, 30, 1024, 192, 1, 0),
        Q(80, 350, 0, 384, 480, 45, 1024, 192, 1, 0),
    };
    gl_buffer_vtx(_vbo[1], nvtx, 1);
}
sc_screen sc_game_end_screen() {
    const mp_match * m = mp_match_get();
    
    char buf[1024] = "";
    strcat(buf, m->p1_name);
    strcat(buf, "\n");
    strcat(buf, m->p2_name);
    sprintf(buf + strlen(buf), "\nafter %d rounds\n", m->data.count - 2);
    
    glBindTexture(GL_TEXTURE_2D, _txt);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    nt_render_text(buf, 48, 1);
    nt_play_sound("end.wav");
    
    sc_screen s;
    s.prepare = _prepare;
    s.render = _render;
    s.set_pick = _set_pick;
    s.handle_tap = _tap;
    return s;
}
