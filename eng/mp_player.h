//
//  mp_player.h
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 16/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#ifndef __DDRMemory__mp_player__
#define __DDRMemory__mp_player__

typedef struct mp_player {
    const char * pid;
    const char * name;
} mp_player;

const mp_player * mp_player_get();
void mp_player_set(const mp_player * p);

#endif /* defined(__DDRMemory__mp_player__) */
