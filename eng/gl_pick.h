//
//  gl_pick.h
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 09/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#ifndef __DDRMemory__gl_pick__
#define __DDRMemory__gl_pick__

#include "gl_main.h"

void gl_pick_init(int sw, int sh);
void gl_pick_bind_fb();
uint8_t gl_pick_red(int x, int y, void (*render)());

#endif /* defined(__DDRMemory__gl_pick__) */
