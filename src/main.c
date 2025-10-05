#define NOB_IMPLEMENTATION
#include "utils.h"
#include "player.h"
#include "renderer.h"

void* lock(void *opaque, void **planes) {
    renderer_data data = *((renderer_data*)opaque);
    *planes = data.video_pixels;
    return NULL;
}

void unlock(void *opaque, void *picture, void *const *planes) {
    // This function can be used for synchronization if needed
}

void display(void *opaque, void *picture) {
    renderer_data data = *((renderer_data*)opaque);

    SDL_UpdateTexture(data.video_texture, NULL, data.video_pixels, data.video_pitch);
    SDL_RenderClear(data.renderer);
    SDL_RenderCopy(data.renderer, data.video_texture, NULL, NULL);
    SDL_RenderPresent(data.renderer);
}

int main(int argc, char** argv){
    // TODO: remove after release
    const char *video_path = "D:\\muzyka\\minimal\\Deep⧸Minimal Drum and Bass Mix #1-(720p30).mp4";

    if(argc > 1){
        video_path = argv[1];
    }

    Player_ctx ctx = player_init();
    player_load(&ctx, video_path);

    renderer_data rend_data = renderer_init();

    rend_data.video_pitch = rend_data.width * 4; // * 4 - RGB32 RV32
    rend_data.video_texture = SDL_CreateTexture(rend_data.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, rend_data.width, rend_data.height);
    errorpz(rend_data.video_texture, "Cant create SDL texture!");

    rend_data.video_pixels = malloc(rend_data.video_pitch * rend_data.height);

    libvlc_video_set_callbacks(ctx.mp, lock, unlock, display, &rend_data);
    libvlc_video_set_format(ctx.mp, "RV32", rend_data.width, rend_data.height, rend_data.video_pitch);

    //TODO: Errors probably with RV32 "chroma" value format that is not supported with HW Accel

    player_play(&ctx);

    libvlc_state_t prev_state;
    double last_time = 0;
    double current_time = 0;
    SDL_Event event;
	int running = 1;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = 0;
        }
        
        libvlc_state_t state = libvlc_media_get_state(libvlc_media_player_get_media(ctx.mp));
        if(state != prev_state) {
            printf("STATE: %d\n", state);
        }
        prev_state = state;

        SDL_GetWindowSizeInPixels(rend_data.window,&rend_data.width,&rend_data.height);

		renderer_update(rend_data);
        
        current_time = (double)SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency();
		rend_data.delta_time = current_time - last_time;
		rend_data.fps = 1.0 / rend_data.delta_time;
		last_time = current_time;
		rend_data.time = current_time;
    }

	renderer_end(rend_data);

    player_exit(&ctx);
    free(rend_data.video_pixels);

    return 0;
}