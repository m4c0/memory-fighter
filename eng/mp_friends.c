//
//  mp_friends.c
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 16/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#include "mp_friends.h"

#include <stdlib.h>
#include <string.h>

#include "mp_status.h"
#include "nt_main.h"

static mp_player _list[100];
static int _count = -1;

int mp_friends_count() {
    return _count;
}

void mp_friends_error(const char * msg) {
    if (_count == -1) mp_status_error(msg);
}

const mp_player * mp_friends_get(int pos) {
    return _list + pos;
}

const mp_player * mp_friends_list(int * count) {
    if (count) *count = _count;
    return _list;
}

void mp_friends_load() {
    nt_load_player_friends();
}

void mp_friends_set(mp_player * list, int count) {
    for (int i = 0; i < _count; i++) {
        free((char *)_list[i].pid);
        free((char *)_list[i].name);
    }
    _count = count;
    for (int i = 0; i < count; i++) {
        _list[i].pid = strdup(list[i].pid);
        _list[i].name = strdup(list[i].name);
    }
}
