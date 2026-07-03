//
//  sc_main.h
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 09/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#ifndef __DDRMemory__sc_main__
#define __DDRMemory__sc_main__

typedef struct sc_screen {
    void (*prepare)(float s);
    void (*render)();
    void (*set_pick)(int p);
    void (*handle_tap)();
} sc_screen;

void sc_init();

sc_screen sc_current_get();
void sc_current_set(sc_screen s);

#endif /* defined(__DDRMemory__sc_main__) */
