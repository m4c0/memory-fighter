//
//  shader.fsh
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 12/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

uniform lowp float uf_pick;
uniform sampler2D us_texture;

varying lowp vec2 vv2_tex;
varying lowp float vf_alpha;
varying lowp float vf_pick;

void main() {
    lowp vec4 lv4_color = texture2D(us_texture, vv2_tex) * vec4(1.0, 1.0, 1.0, vf_alpha);
    lowp vec4 lv4_pick = vec4(vf_pick, 0.0, 0.0, vf_pick / mix(1.0, 0.0, vf_pick));
    gl_FragColor = mix(lv4_color, lv4_pick, uf_pick);
}
