//
//  sc_game_repeat.c
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 14/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#include "sc_game_repeat.h"

#include <stdio.h>
#include <string.h>

#include "gl_main.h"
#include "mp_main.h"
#include "nt_main.h"
#include "sc_base.h"
#include "sc_game.h"
#include "sc_load.h"
#include "sc_menu.h"

static enum {
    _STATE_SHOWING,
    _STATE_GO,
    _STATE_PLAY,
    _STATE_BLINKING,
    _STATE_LOSING,
} _state;
static int _current_item;
static int _picking, _tapped, _wrong;
static GLuint _nameTex;
static float _timer;
static GLuint _vbo[3];
static mp_match _match;
static int _last_show_sound;

static void _play_right(int i) {
    char buf[128];
    sprintf(buf, "right_%d.wav", i);
    nt_play_sound(buf);
}

static void _prepare(float s) {
    _timer += s;
    
    float a[4] = { 0, 0, 0, 0 };
    float ae[4] = { 0, 0, 0, 0 };
    if (_state == _STATE_SHOWING) {
        int item = (int)(_timer / 0.5);
        if (item < _match.data.count) {
            if (item != _last_show_sound) {
                _play_right(_match.data.sequence[item]);
                _last_show_sound = item;
            }
            
            float r = (_timer / 0.5) - (float)item;
            float * b = a + _match.data.sequence[item];
            if (r < 0.5) {
                *b = r * 2.0;
            } else if (r < 1.0) {
                *b = 1.0;
            } else if (r < 1.5) {
                *b = 1.0 - (r - 1.0) * 2.0;
            }
        } else {
            _state = _STATE_GO;
            _timer = 0;
        }
    } else if (_state == _STATE_BLINKING) {
        float * who = _wrong ? ae + _tapped : a + _tapped;
        if (_timer < 0.125) {
            *who = _timer * 8.0;
        } else if (_timer < 0.25) {
            *who = 1.0 - (_timer - 0.125) * 8.0;
        } else if (!_match.data.life.p1 || !_match.data.life.p2) {
            nt_play_sound("lost.wav");
            _state = _STATE_LOSING;
            _timer = 0;
        } else if (_current_item < _match.data.count) {
            _state = _STATE_PLAY;
        } else {
            mp_match_end_turn(_match);
            if (_match.mid[0] == 1) {
                sc_current_set(sc_game_screen());
            } else {
                sc_current_set(sc_load_screen(MP_STATUS_MATCH_UPDATED, 1, sc_game_screen()));
            }
            return;
        }
    }
    
    float p1x = -320.0 * (1.0 - (float)_match.data.life.p1 / 10.0);
    float p2x = +300.0 * (1.0 - (float)_match.data.life.p2 / 10.0);
    
    float watchAlpha = 0.0;
    float lostAlpha = 0.0;
    if (_state == _STATE_GO) {
        if (_timer < 0.5) {
            watchAlpha = _timer * 2.0;
        } else if (_timer < 1.0) {
            watchAlpha = 1.0;
        } else if (_timer < 1.5) {
            watchAlpha = 1.0 - (_timer - 1.0) * 2.0;
        } else {
            watchAlpha = 0;
            _state = _STATE_PLAY;
            _timer = 0;
        }
    } else if (_state == _STATE_LOSING) {
        if (_timer < 0.5) {
            lostAlpha = _timer * 2.0;
        } else if (_timer < 2.5) {
            lostAlpha = 1.0;
        } else if (_timer < 3.0) {
            lostAlpha = 1.0 - (_timer - 1.5) * 2.0;
        } else {
            mp_match_end_game(_match, 0);
            sc_current_set(sc_menu_screen());
        }
    }
    gl_vertex vtx[] = {
        // Blocks
        
        Q( 31,  15, 690, 462, 264, 264, 108, 108, a[0], 0),
        Q(345,  75, 798, 462, 264, 264, 108, 108, a[1], 0),
        Q( 31, 335, 690, 570, 264, 264, 108, 108, a[2], 0),
        Q(345, 395, 798, 570, 264, 264, 108, 108, a[3], 0),
        
        // Blocks
        
        Q( 31,  15, 474, 462, 264, 264, 108, 108, ae[0], 0),
        Q(345,  75, 582, 462, 264, 264, 108, 108, ae[1], 0),
        Q( 31, 335, 474, 570, 264, 264, 108, 108, ae[2], 0),
        Q(345, 395, 582, 570, 264, 264, 108, 108, ae[3], 0),
        
        // Borders
        
        Q(16,   0, 474, 385, 294,  32, 294, 32, 1, 0.2),
        Q(16,  32, 474, 417, 294, 230, 294,  4, 1, 0.2),
        Q(16, 262, 474, 430, 294,  32, 294, 32, 1, 0.2),
        
        Q(330,  60, 474, 385, 294,  32, 294, 32, 1, 0.4),
        Q(330,  92, 474, 417, 294, 230, 294,  4, 1, 0.4),
        Q(330, 322, 474, 430, 294,  32, 294, 32, 1, 0.4),
        
        Q(16, 320, 474, 385, 294,  32, 294, 32, 1, 0.6),
        Q(16, 352, 474, 417, 294, 230, 294,  4, 1, 0.6),
        Q(16, 582, 474, 430, 294,  32, 294, 32, 1, 0.6),
        
        Q(330, 380, 474, 385, 294,  32, 294, 32, 1, 0.8),
        Q(330, 412, 474, 417, 294, 230, 294,  4, 1, 0.8),
        Q(330, 642, 474, 430, 294,  32, 294, 32, 1, 0.8),
        
        // Messages

        QQ(275, 300, 760, 174, 90, watchAlpha, 0),
        QQ(275, 640, 850, 385, 90, lostAlpha, 0),
        
        // P1 Bar
        
        Q(-95,       -80, 474, 679, 450, 48, 450, 48, 1, 0),
        Q(-95 + p1x, -80, 474, 726, 450, 48, 450, 48, 1, 0),
        
        // P2 Bar
        
        Q(305,       -50, 474 + 450, 679 + 48, 450, 48, -450, -48, 1, 0),
        Q(305 + p2x, -50, 474 + 450, 726 + 48, 450, 48, -450, -48, 1, 0),
    };
    gl_buffer_vtx(_vbo[1], vtx, 0);
}

static void _render() {
    gl_atlas_bind();
    gl_draw_elements(_vbo[1], _match.mid[0] == 1 ? 24 : 26);
    
    glBindTexture(GL_TEXTURE_2D, _nameTex);
    gl_draw_elements(_vbo[2], _match.mid[0] == 1 ? 1 : 2);
}

static void _set_pick(int p) {
    _picking = p;
}

static void _tap() {
    if (_state != _STATE_PLAY) return;
    _tapped = _picking - 1;
    _state = _STATE_BLINKING;
    _timer = 0;
    
    if (_match.data.sequence[_current_item] == _tapped) {
        _play_right(_tapped);
        _wrong = 0;
        _current_item++;
    } else {
        nt_play_sound("wrong.wav");
        _wrong = 1;
        if (_match.i_am_p1) {
            _match.data.life.p1--;
        } else {
            _match.data.life.p2--;
        }
    }
}

void sc_game_repeat_init() {
    glGenBuffers(3, _vbo);
    glGenTextures(1, &_nameTex);
    
    static gl_vertex nvtx[] = {
        Q(0, -35, 0, 0, 320, 30, 1024, 192, 1, 0),
        Q(340, -75, 0, 192, 320, 30, 1024, 192, 1, 0),
    };
    gl_buffer_vtx(_vbo[2], nvtx, 1);
}

sc_screen sc_game_repeat_screen() {
    _timer = 0;
    _current_item = 0;
    _state = _STATE_SHOWING;
    _last_show_sound = -1;
    
    _match = *(mp_match_get());
    
    char buf[1024] = "";
    strcat(buf, _match.p1_name);
    strcat(buf, "\n");
    strcat(buf, _match.p2_name);
    strcat(buf, "\n\n"); // Para ficar na mesma proporcao do game_end
    
    glBindTexture(GL_TEXTURE_2D, _nameTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    nt_render_text(buf, 48, 1);
    
    sc_screen s;
    s.prepare = _prepare;
    s.render = _render;
    s.set_pick = _set_pick;
    s.handle_tap = _tap;
    return s;
}
