#include "renderer.h"

void set_best_renderer_driver(SDL_Window* window) {
    //printf("Available renderers:\n");

    int renderers_count = SDL_GetNumRenderDrivers();

    dynamic_array(const char *) renderer_names_da = {0};

    for (int i = 0; i < renderers_count; i++) {
        SDL_RendererInfo info;
        SDL_GetRenderDriverInfo(i, &info);

        da_append(&renderer_names_da, info.name);

        //printf("%d: %s\n", i, info.name);
    }

    if(da_contains_cstr(&renderer_names_da, "direct3d11")){
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d11");
    }else if(da_contains_cstr(&renderer_names_da, "direct3d12")){
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d12");
    }else if(da_contains_cstr(&renderer_names_da, "direct3d")){
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d");
    }else if(da_contains_cstr(&renderer_names_da, "metal")){
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
    }else if(da_contains_cstr(&renderer_names_da, "opengl")){
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    }
}

renderer_data renderer_init() {
    renderer_data data = {0};
    data.width = 800;
    data.height = 600;

    SDL_Init(SDL_INIT_VIDEO);
    
    data.window = SDL_CreateWindow("BVLC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, data.width, data.height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    set_best_renderer_driver(data.window);

    data.renderer = SDL_CreateRenderer(data.window, -1, SDL_RENDERER_ACCELERATED);
    
    return data;
}

void renderer_update(renderer_data data){
    SDL_SetRenderDrawColor(data.renderer, 0, 0, 0, 255);
    SDL_RenderClear(data.renderer);
    
    

    SDL_RenderPresent(data.renderer);
}

void renderer_end(renderer_data data){
    SDL_DestroyRenderer(data.renderer);
    SDL_DestroyWindow(data.window);
    SDL_Quit();
}