//
//  mp_matches.c
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 16/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#include "mp_matches.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "mp_status.h"
#include "nt_main.h"

static mp_match _list[1024];
static int _count = -1;
static int _version = -1;

int mp_matches_count() {
    return _count;
}

const mp_match * mp_matches_get(int pos) {
    return _list + pos;
}

const mp_match * mp_matches_list(int * count) {
    if (count) *count = _count;
    return _list;
}

int mp_matches_list_version() {
    return _version;
}

void mp_matches_load() {
    _count = -1;
    nt_load_player_matches();
}

void _update_match(mp_match * p, mp_match * m) {
    *p = *m;
    p->timestamp = time(0);
    if (!strlen(p->p1_name)) strcpy(p->p1_name, "<Opponent>");
    if (!strlen(p->p2_name)) strcpy(p->p2_name, "<Opponent>");
    _version++;
}

#define SWAP mp_match tmp = list[i]; list[i] = list[j]; list[j] = tmp
void mp_matches_set(mp_match * list, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (list[i].ended && !list[j].ended) {
                SWAP;
            } else if (!list[i].ended && list[j].ended) {
            } else if (list[i].ended && list[j].ended) {
                if (list[i].last_update_timestamp < list[j].last_update_timestamp) {
                    SWAP;
                }
            } else if (!list[i].my_turn && list[j].my_turn) {
                SWAP;
            } else if (list[i].my_turn && !list[j].my_turn) {
            } else if (list[i].last_update_timestamp < list[j].last_update_timestamp) {
                SWAP;
            }
        }
    }
    
    _count = count < 1024 ? count : 1024;
    int endedCount = 0;
    for (int i = 0, j = 0; i < _count; i++) {
        if (_list[j].ended && endedCount++ > 128) {
            nt_match_delete(list[i].mid);
        } else {
            _update_match(_list + j, list + i);
            j++;
        }
    }
}

void mp_matches_update(mp_match m) {
    for (int i = 0; i < _count; i++) {
        if (!strcmp(_list[i].mid, m.mid)) {
            _update_match(_list + i, &m);
            break;
        }
    }
    mp_match_update(m);
}

void mp_matches_error(const char * err) {
    if (_count == -1) mp_status_error(err);
}
