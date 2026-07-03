//
//  mp_player.c
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 16/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#include "mp_player.h"

#include <stdlib.h>
#include <string.h>

#include "mp_status.h"

static mp_player _player;

const mp_player * mp_player_get() {
    return _player.pid ? &_player : 0;
}
void mp_player_set(const mp_player * p) {
    if (_player.pid) free((char *)_player.pid);
    if (_player.name) free((char *)_player.name);
    
    if (p) {
        _player.pid = p->pid ? strdup(p->pid) : 0;
        _player.name = p->name ? strdup(p->name) : 0;
    } else {
        mp_status_error("Error logging in");
    }
}
