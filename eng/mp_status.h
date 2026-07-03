//
//  mp_status.h
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 16/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#ifndef __DDRMemory__mp_status__
#define __DDRMemory__mp_status__

#define MP_STATUS_ERROR          0x01
#define MP_STATUS_LOGGED_IN      0x02
#define MP_STATUS_MATCH_UPDATED  0x08

void mp_status_clear_error();
void mp_status_error(const char * err);
int mp_status_flags();
const char * mp_status_message();

#endif /* defined(__DDRMemory__mp_status__) */
