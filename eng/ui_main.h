//
//  ui_main.h
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 08/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#ifndef __DDRMemory__ui_main__
#define __DDRMemory__ui_main__

void ui_drawFrame();
void ui_prepareFrame(float s);
void ui_init(int sw, int sh);
void ui_touch_begin(int x, int y);
void ui_touch_moved(int x, int y);
void ui_touch_ended(int x, int y);

#endif /* defined(__DDRMemory__ui_main__) */
