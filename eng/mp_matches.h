//
//  mp_matches.h
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 16/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#ifndef __DDRMemory__mp_matches__
#define __DDRMemory__mp_matches__

#include "mp_match.h"

int mp_matches_count();
void mp_matches_error(const char * err);
const mp_match * mp_matches_get(int pos);
const mp_match * mp_matches_list(int * count);
int mp_matches_list_version();
void mp_matches_load();
void mp_matches_set(mp_match * list, int count);
void mp_matches_update(mp_match who);

#endif /* defined(__DDRMemory__mp_matches__) */
