//
//  sc_loadmatch.h
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 12/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#ifndef __DDRMemory__sc_loadmatch__
#define __DDRMemory__sc_loadmatch__

#include "sc_main.h"

void sc_load_init();
sc_screen sc_load_screen(int requiredFlag, int optional, sc_screen next);

#endif /* defined(__DDRMemory__sc_loadmatch__) */
