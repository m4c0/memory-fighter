//
//  nt_main.h
//  DDRMemory
//
//  Created by Eduardo Mauricio da Costa on 08/01/15.
//  Copyright (c) 2015 Eduardo Mauricio da Costa. All rights reserved.
//

#ifndef DDRMemory_nt_main_h
#define DDRMemory_nt_main_h

void nt_load_player_friends();
void nt_load_player_matches();
void nt_load_shader_source(unsigned int shader, const char * filename);
void nt_load_texture(const char * name);
void nt_render_text(const char * text, int fontSize, int centered);
void nt_match_start_random();
void nt_match_with_friend(const char * pid);
void nt_match_delete(const char * mid);
void nt_match_end_turn();
void nt_match_end_game(int winning);
void nt_play_sound(const char * name);
void nt_share_screen(const char * msg);
int nt_random(int max);

#endif
