//
//  mp_match.c
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 16/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#include "mp_match.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "mp_main.h"
#include "nt_main.h"

static mp_match _match;

void mp_match_start_alone() {
    mp_match m;
    memset(&m, 0, sizeof(m));
    m.mid[0] = 1;
    m.i_am_p1 = 1;
    strcpy(m.p1_name, "You");
    strcpy(m.p2_name, "Nobody");
    mp_match_set(m);
}
void mp_match_start_random() {
    nt_match_start_random();
}
void mp_match_with_friend(const char * pid) {
    nt_match_with_friend(pid);
}

const mp_match * mp_match_get() {
    return &_match;
}

void mp_match_set(mp_match match) {
    _match = match;
    _match.timestamp = time(0);
    if (!strlen(_match.p1_name)) strcpy(_match.p1_name, "<Opponent>");
    if (!strlen(_match.p2_name)) strcpy(_match.p2_name, "<Opponent>");
}

void mp_match_update(mp_match match) {
    if (!strlen(_match.mid) || strcmp(match.mid, _match.mid)) return;
    mp_match_set(match);
}

void mp_match_end_game(mp_match match, int winning) {
    mp_match_set(match);
    nt_match_end_game(winning);
}

void mp_match_end_turn(mp_match match) {
    mp_match_set(match);
    nt_match_end_turn();
}

void mp_match_error_set(mp_match match, const char * err) {
    if (strcmp(match.mid, _match.mid)) return;
    
    mp_status_error(err);
}