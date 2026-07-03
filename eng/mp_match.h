//
//  mp_match.h
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 16/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#ifndef __DDRMemory__mp_match__
#define __DDRMemory__mp_match__

#include <stdint.h>

typedef struct mp_match {
    char mid[1024];
    int my_turn;
    int i_am_p1;
    int ended;
    
    char p1_name[1024];
    char p2_name[1024];
    
    long last_update_timestamp;
    long timestamp;
    
    struct {
        uint8_t sequence[256];
        uint8_t count;
        struct {
            uint8_t p1, p2;
        } life;
    } data __attribute__((aligned(1)));
} mp_match;

void mp_match_start_alone();
void mp_match_start_random();
void mp_match_with_friend(const char * pid);
void mp_match_end_game(mp_match match, int winning);
void mp_match_end_turn(mp_match match);
const mp_match * mp_match_get();
void mp_match_set(mp_match match);
void mp_match_update(mp_match match);
void mp_match_error_set(mp_match match, const char * err);

#endif /* defined(__DDRMemory__mp_match__) */
