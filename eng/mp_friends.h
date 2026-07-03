//
//  mp_friends.h
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 16/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#ifndef __DDRMemory__mp_friends__
#define __DDRMemory__mp_friends__

#include "mp_player.h"

int mp_friends_count();
void mp_friends_error(const char * msg);
const mp_player * mp_friends_get(int pos);
const mp_player * mp_friends_list(int * count);
void mp_friends_load();
void mp_friends_set(mp_player * list, int count);

#endif /* defined(__DDRMemory__mp_friends__) */
