//
//  mp_status.c
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 16/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#include "mp_status.h"

#include "mp_friends.h"
#include "mp_matches.h"
#include "mp_player.h"

#define RS(flag, cond) { if (cond) _flags |= flag; else _flags &= ~flag; }

static const char * _status;
static int _flags;
static long _matchTimestamp;

void mp_status_clear_error() {
    _flags &= ~MP_STATUS_ERROR;
    _matchTimestamp = mp_match_get()->timestamp;
#ifdef __APPLE__
    _status = "Waiting for Game Center";
#else
    _status = "Waiting for Play Services";
#endif
}

void mp_status_error(const char * err) {
    _status = err;
    RS(MP_STATUS_ERROR, err);
}

int mp_status_flags() {
    RS(MP_STATUS_LOGGED_IN, mp_player_get());
    RS(MP_STATUS_MATCH_UPDATED, _matchTimestamp < mp_match_get()->timestamp);
    return _flags;
}

const char * mp_status_message() {
    return _status;
}
