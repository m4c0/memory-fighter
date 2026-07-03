//
//  sc_main.c
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 09/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#include "sc_main.h"

#include "mp_main.h"
#include "sc_base.h"
#include "sc_curmatch.h"
#include "sc_game.h"
#include "sc_game_end.h"
#include "sc_game_repeat.h"
#include "sc_load.h"
#include "sc_menu.h"
#include "sc_newmatch.h"

sc_screen _current;

void sc_init() {
    sc_base_init();
    sc_curmatch_init();
    sc_game_init();
    sc_game_end_init();
    sc_game_repeat_init();
    sc_load_init();
    sc_menu_init();
    sc_newmatch_init();
    
    sc_current_set(sc_menu_screen());
}

sc_screen sc_current_get() {
    return _current;
}

void sc_current_set(sc_screen s) {
    _current = s;
    s.prepare(0);
}
