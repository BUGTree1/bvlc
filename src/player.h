#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "renderer.h"
#include "utils.h"
#include <vlc/vlc.h>

typedef struct {
    libvlc_instance_t* inst;
    libvlc_media_player_t* mp;

    SDL_Window* video_window;
    int video_width;
    int video_height;

    renderer_data* rend_data;
} Player_ctx;

Player_ctx player_init(renderer_data* rend_data);
void player_load  (Player_ctx* ctx, const char* media_path);
void player_play  (Player_ctx* ctx);
void player_pause (Player_ctx* ctx);
void player_stop  (Player_ctx* ctx);
void player_exit  (Player_ctx* ctx);

#endif // __PLAYER_H__