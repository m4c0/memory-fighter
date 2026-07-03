//
//  shader.vsh
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 12/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

uniform float uf_h;

attribute vec2 av2_pos;
attribute float af_alpha;
attribute float af_pick;
attribute vec2 av2_tex;

varying lowp vec2 vv2_tex;
varying lowp float vf_alpha;
varying lowp float vf_pick;

void main() {
    vec2 lv2_pos;
    lv2_pos.x = av2_pos.x / 640.0 * 2.0 - 1.0;
    lv2_pos.y = (1.0 - av2_pos.y / 640.0 * 2.0) / uf_h;
    
    gl_Position = vec4(lv2_pos, 0, 1);
    vv2_tex = av2_tex / vec2(1024.0, 1024.0);
    vf_alpha = af_alpha;
    vf_pick = af_pick;
}
