//
//  sc_newmatch.c
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 09/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#include "sc_newmatch.h"

#include <string.h>

#include "gl_main.h"
#include "mp_main.h"
#include "nt_main.h"
#include "sc_base.h"
#include "sc_game.h"
#include "sc_load.h"
#include "sc_menu.h"

static GLuint _friendsTex;
static int _friends;
static int _firstFriend;
static int _currentFriend;

static float _timer;
static float _timerRepeat;

static GLuint _vbo[2];

static int _picked;

static void _set_pick(int p) {
    _picked = p;
    
    int f = _currentFriend - _firstFriend;
    gl_vertex _btn[] = {
        QQ(-80, 474, 300, 460, 85, 1, 0),
        
        QQ( 40, 0, 462, 472, 54, (_picked != 1) ? 1.0 : 0.3, 0.2),
        QQ(150, 0, 516, 472, 54, (_friends && _picked != 2) ? 1.0 : 0.3, 0.4),
        QQ(550, 0, 570, 472, 54, (_picked != 3) ? 1.0 : 0.3, 0.6),
        
        Q(570, 340, 300, 170, 58, 56, 58, 56, (_currentFriend > 0 && _picked != 4) ? 1.0 : 0.3, 0.8),
        Q(570, 420, 300, 226, 58, 56, 58, 56, (_currentFriend < _friends - 1 && _picked != 5) ? 1.0 : 0.3, 1.0),
        
        Q(10, 255 + f * 44, 358, 170, 548, 56, 548, 56, _friends > 0, 0),
    };
    gl_buffer_vtx(_vbo[0], _btn, 0);
}

static void _update_friends() {
    int c;
    const mp_player * list = mp_friends_list(&c);
    if (c == _friends) return;
    
    char buf[1024] = { 0 };
    _friends = c;
    int i = 0;
    for (i = 0; (i < 6) && (_firstFriend + i) < _friends; i++) {
        strcat(buf, "\n");
        strcat(buf, list[i + _firstFriend].name);
    }
    while (i < 6) {
        strcat(buf, "\n");
        i++;
    }
    
    glBindTexture(GL_TEXTURE_2D, _friendsTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    nt_render_text(buf + 1, 44, 0);
    
    gl_vertex meh[] = {
        Q(20, 260, 0, 0,   512, 512, 1024, 1024,   1, 0)
    };
    gl_buffer_vtx(_vbo[1], meh, 0);
}

static void _render() {
    sc_base_render();
    
    gl_draw_elements(_vbo[0], 7);
    if (_friends > 0) {
        glBindTexture(GL_TEXTURE_2D, _friendsTex);
        gl_draw_elements(_vbo[1], 1);
    }
}

static void _tap() {
    if (_picked != 0) nt_play_sound("tap.wav");
    switch (_picked) {
        case 1:
            mp_match_start_random();
            sc_current_set(sc_load_screen(MP_STATUS_MATCH_UPDATED, 0, sc_game_screen()));
            break;
        case 2: {
            mp_match_with_friend(mp_friends_get(_currentFriend)->pid);
            sc_current_set(sc_load_screen(MP_STATUS_MATCH_UPDATED, 0, sc_game_screen()));
            break;
        }
        case 3: sc_current_set(sc_menu_screen()); break;
        case 4:
            if (_currentFriend == 0) break;
            
            _currentFriend--;
            if (_friends <= 6) break;
            
            if (_firstFriend && (_currentFriend - _firstFriend < 2)) {
                _firstFriend--;
                _friends = 0;
            }
            break;
        case 5:
            if (_currentFriend >= _friends - 1) break;
            
            _currentFriend++;
            if (_friends <= 6) break;
            
            if (_currentFriend - _firstFriend > 2) {
                _firstFriend++;
                _friends = 0;
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

void sc_newmatch_init() {
    glGenBuffers(2, _vbo);
    glGenTextures(1, &_friendsTex);
    
    _picked = 0;
}

sc_screen sc_newmatch_screen() {
    mp_friends_load();
    
    sc_screen s;
    s.prepare = _prepare;
    s.render = _render;
    s.set_pick = _set_pick;
    s.handle_tap = _tap;
    return s;
}
