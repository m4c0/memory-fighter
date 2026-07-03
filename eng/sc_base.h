//
//  sc_base.h
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 09/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#ifndef __DDRMemory__sc_base__
#define __DDRMemory__sc_base__

#define Q(sx, sy, su, sv, ex, ey, eu, ev, a, p) \
{ sx, sy, a, p, su, sv }, { sx+ex, sy, a, p, su+eu, sv }, { sx, sy+ey, a, p, su, sv+ev }, { sx+ex, sy+ey, a, p, su+eu, sv+ev }
#define QQ(y, u, v, w, h, a, p) Q((640 - w) / 2, (y), (u), (v), (w), (h), (w), (h), (a), (p))

#define CNT(x) (sizeof(x) * 6 / (sizeof(gl_vertex) * 4))

void sc_base_init();
void sc_base_prepare();
void sc_base_render();

#endif /* defined(__DDRMemory__sc_base__) */
