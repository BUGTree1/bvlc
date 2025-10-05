#pragma once
#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "utils.h"

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Texture* video_texture;
    int video_pitch;
    uint8_t* video_pixels;
    int video_width;
    int video_height;

    int width;
    int height;

    double fps;
    double delta_time;
    double time;
} renderer_data;

renderer_data renderer_init();
void renderer_update(renderer_data data);
void renderer_end(renderer_data data);

#endif //__RENDERER_H__