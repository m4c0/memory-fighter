//
//  sc_curmatch.c
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 13/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#include "sc_curmatch.h"

#include <string.h>

#include "gl_main.h"
#include "mp_main.h"
#include "nt_main.h"
#include "sc_base.h"
#include "sc_game.h"
#include "sc_load.h"
#include "sc_menu.h"

static int _open;

static mp_match _list[1024];

static int _listVersion = -1;
static GLuint _listTex[2];
static int _count;
static int _firstFriend;
static int _currentFriend;

static float _timer;
static float _timerRepeat;

static GLuint _vbo[3];

static int _picked;

static void _set_pick(int p) {
    _picked = p;
    
    int mine = 0;
    if (_count) mine = _list[_currentFriend].my_turn;
    
    int f = _currentFriend - _firstFriend;
    gl_vertex _btn[] = {
        QQ(-80, 0, !_open ? 934 : 850, 640, 85, 1, 0),
        
        Q(570, 260, 300, 170, 58, 56, 58, 56, (_currentFriend > 0 && _picked != 4) ? 1.0 : 0.3, 0.8),
        Q(570, 340, 300, 226, 58, 56, 58, 56, (_currentFriend < _count - 1 && _picked != 5) ? 1.0 : 0.3, 1.0),
        
        Q(10, 155 + f * 44, 358, 170, 548, 56, 548, 56, _count > 0, 0),
        
        QQ(550, 0, 570, 472, 54, (_picked != 3) ? 1.0 : 0.3, 0.6),
        QQ( 40, 0, 624, 472, 54, _open ? 0.0 : (_count > 0 && mine && _picked != 1) ? 1.0 : 0.3, 0.2),
        QQ( 40, 732, 226, 172, 56, !_open ? 0.0 : (_count > 0 && _picked != 2) ? 1.0 : 0.3, 0.4),
    };
    gl_buffer_vtx(_vbo[0], _btn, 0);
}

static void _update_list() {
    while (_currentFriend >= _count) {
        _currentFriend = _count--;
        if (_firstFriend && (_currentFriend - _firstFriend < 4)) {
            _firstFriend--;
        }
    }
    
    gl_vertex meh[8 * 4];
    
    char buf[1024] = { 0 };
    int i = 0;
    for (i = 0; (i < 8) && (_firstFriend + i) < _count; i++) {
        strcat(buf, "\n");
        
        const mp_match * m = _list + i + _firstFriend;
        strcat(buf, m->i_am_p1 ? m->p2_name : m->p1_name);
        
        int y;
        if (!_open) {
            y = m->my_turn;
        } else if (m->i_am_p1) {
            y = m->data.life.p1;
        } else {
            y = m->data.life.p2;
        }
        gl_vertex uh[4] = {
            Q(20, 160 + i * 44, 0, i * 88,   512, 44, 1024, 88,  y ? 1.0 : 0.5, 0)
        };
        meh[i * 4 + 0] = uh[0];
        meh[i * 4 + 1] = uh[1];
        meh[i * 4 + 2] = uh[2];
        meh[i * 4 + 3] = uh[3];
    }
    while (i < 8) {
        gl_vertex uh[4] = {
            Q(20, 160 + i * 44, 0, i * 44,   512, 44, 1024, 1024,  0, 0)
        };
        meh[i * 4 + 0] = uh[0];
        meh[i * 4 + 1] = uh[1];
        meh[i * 4 + 2] = uh[2];
        meh[i * 4 + 3] = uh[3];
        
        strcat(buf, "\n");
        i++;
    }

    glBindTexture(GL_TEXTURE_2D, _listTex[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    nt_render_text(buf + 1, 44, 0);
    
    gl_buffer_vtx(_vbo[1], meh, 0);
}
static void _update_friends() {
    if (_listVersion == mp_matches_list_version()) return;
    
    const mp_match * m = mp_matches_list(0);
    
    _count = 0;
    for (int i = 0; i < mp_matches_count(); i++, m++) {
        if (_open ^ !m->ended) {
            _list[_count++] = *m;
        }
    }
    
    _listVersion = mp_matches_list_version();
    
    _update_list();
}

static void _render() {
    sc_base_render();
    
    gl_draw_elements(_vbo[0], 7);
    if (_count > 0) {
        glBindTexture(GL_TEXTURE_2D, _listTex[0]);
        gl_draw_elements(_vbo[1], 8);
    }
}

static void _tap() {
    if (_picked != 0) nt_play_sound("tap.wav");
    switch (_picked) {
        case 1:
        case 2:
            mp_match_set(_list[_currentFriend]);
            sc_current_set(sc_game_screen());
            break;
        case 3: sc_current_set(sc_menu_screen()); break;
        case 4:
            if (_currentFriend == 0) break;
            
            _currentFriend--;
            if (_count <= 8) break;
            
            if (_firstFriend && (_currentFriend - _firstFriend < 4)) {
                _firstFriend--;
                _update_list();
            }
            break;
        case 5:
            if (_currentFriend >= _count - 1) break;
            
            _currentFriend++;
            if (_count <= 8) break;
            
            if (_currentFriend - _firstFriend > 4) {
                _firstFriend++;
                _update_list();
            }
            break;
        default: break;
    }
}

static void _prepare(float s) {
    sc_base_prepare();
    _set_pick(_picked);
    if ((_picked == 4 || _picked == 5)) {
        _timer += s;
        if (_timer > _timerRepeat) {
            _tap();
            _timer -= _timerRepeat;
            _timerRepeat *= 0.8;
        }
    } else {
        _timer = 0;
        _timerRepeat = 0.6;
    }
    _update_friends();
}

void sc_curmatch_init() {
    glGenBuffers(3, _vbo);
    glGenTextures(2, _listTex);
    
    _picked = 0;
}

sc_screen sc_curmatch_screen(int open) {
    _open = open;
    _count = 0;
    _currentFriend = 0;
    _firstFriend = 0;

    mp_matches_load();
    
    sc_screen s;
    s.prepare = _prepare;
    s.render = _render;
    s.set_pick = _set_pick;
    s.handle_tap = _tap;
    return s;
}
