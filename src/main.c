#define NOB_IMPLEMENTATION
#include "utils.h"
#include "player.h"
#include "renderer.h"

int main(int argc, char** argv){
    // TODO: remove after release
    const char *video_path = "D:\\muzyka\\minimal\\Deep⧸Minimal Drum and Bass Mix #1-(720p30).mp4";

    if(argc > 1){
        video_path = argv[1];
    }

    renderer_data rend_data = renderer_init();

    Player_ctx ctx = player_init(&rend_data);
    player_load(&ctx, video_path);

    player_play(&ctx);

    double last_time = 0;
    double current_time = 0;
	bool running = true;
    
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type != SDL_MOUSEMOTION) printf("event: %d\n", event.type);
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = false;
            if (event.type == SDL_WINDOWEVENT) {
                printf("WINDOW T: %d\n", event.window.event);
                printf("WINDOW ID: %d\n", event.window.windowID);
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) running = false;
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    rend_data.width  = event.window.data1;
                    rend_data.height = event.window.data2;

                    ctx.video_width  = rend_data.width  / 2;
                    ctx.video_height = rend_data.height / 2;

                    SDL_SetWindowSize(ctx.video_window, ctx.video_width, ctx.video_height);
                }
                if (event.window.event == SDL_WINDOWEVENT_MOVED) {
                    SDL_SetWindowPosition(ctx.video_window, event.window.data1, event.window.data2);
                }
            }
        }

		renderer_update(rend_data);
        
        current_time = (double)SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency();
		rend_data.delta_time = current_time - last_time;
		rend_data.fps = 1.0 / rend_data.delta_time;
		last_time = current_time;
		rend_data.time = current_time;
    }

	renderer_end(rend_data);

    player_exit(&ctx);

    return 0;
}