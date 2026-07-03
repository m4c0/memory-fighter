//
//  ui_main.c
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 08/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#include "ui_main.h"

#include "gl_main.h"
#include "gl_pick.h"
#include "mp_main.h"
#include "sc_main.h"

void ui_init(int sw, int sh) {
    glClearColor(0, 0, 0, 1);
    gl_init((float)sh / (float)sw);
    gl_pick_init(sw, sh);
    sc_init();
}

void ui_prepareFrame(float s) {
    sc_current_get().prepare(s);
}

void ui_drawFrame() {
    glClear(GL_COLOR_BUFFER_BIT);
    sc_current_get().render();
}

static int _pick(int x, int y) {
    uint8_t pix = gl_pick_red(x, y, sc_current_get().render);
    if (pix ==  0) return 0;
    if (pix <  50) return 1;
    if (pix < 100) return 2;
    if (pix < 180) return 3;
    if (pix < 225) return 4;
    return 5;
}

void ui_touch_begin(int x, int y) {
    sc_current_get().set_pick(_pick(x, y));
}
void ui_touch_moved(int x, int y) {
    sc_current_get().set_pick(_pick(x, y));
}
void ui_touch_ended(int x, int y) {
    sc_screen current = sc_current_get();
    current.handle_tap();
    current.set_pick(0);
}
