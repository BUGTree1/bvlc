#include "player.h"


Player_ctx player_init(renderer_data* rend_data){
    Player_ctx ctx = {};
    const char *vlc_args[] = {
        #ifdef UTILS_OS_WINDOWS
        "--no-xlib",  // Avoid X11 usage on Windows
        #endif

        "--quiet"
    };
    ctx.inst = libvlc_new(ARRAY_LEN(vlc_args), vlc_args);
    errorpz(ctx.inst, "Failed to initialize libVLC\n");
    ctx.rend_data = rend_data;

    ctx.video_width  = ctx.rend_data->width  / 2;
    ctx.video_height = ctx.rend_data->height / 2;
    ctx.video_window = SDL_CreateWindow("libvlc video window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ctx.video_width, ctx.video_height, SDL_WINDOW_RESIZABLE);
    return ctx;
}

void player_load(Player_ctx* ctx, const char* media_path){
    libvlc_media_t* media = libvlc_media_new_path(ctx->inst, media_path);
    errorpz(media, "Failed to create media\n");

    ctx->mp = libvlc_media_player_new_from_media(media);
    libvlc_media_release(media);

    SDL_SysWMinfo wm_info;
    SDL_VERSION(&wm_info.version);
    SDL_GetWindowWMInfo(ctx->video_window, &wm_info);
    
    #ifdef UTILS_OS_WINDOWS
    libvlc_media_player_set_hwnd(ctx->mp, wm_info.info.win.window);
    #endif

    #ifdef UTILS_OS_ANDROID
    libvlc_media_player_set_android_context(ctx->mp, wm_info.info.android.window);
    #endif

    #ifdef UTILS_OS_POSIX
    #ifdef SDL_VIDEO_DRIVER_X11
    libvlc_media_player_set_xwindow(ctx->mp, wm_info.info.x11.window);
    #endif
    #ifdef SDL_VIDEO_DRIVER_WAYLAND
    error("libvlc does not support wayland!");
    #endif
    #endif

    #ifdef UTILS_OS_DARWIN
    #ifdef SDL_VIDEO_DRIVER_COCOA
    libvlc_media_player_set_nsobject(ctx->mp, wm_info.info.cocoa.window);
    #endif
    #ifdef SDL_VIDEO_DRIVER_UIKIT
    warning("uikit window set as nsobject!");
    libvlc_media_player_set_nsobject(ctx->mp, wm_info.info.uikit.window);
    #endif
    #endif
}
void player_play(Player_ctx* ctx){
    errornz(libvlc_media_player_play(ctx->mp),"Failed to play media\n");
}

void player_pause(Player_ctx* ctx){
    libvlc_media_player_pause(ctx->mp);
}

void player_stop(Player_ctx* ctx){
    libvlc_media_player_stop(ctx->mp);
}

void player_exit(Player_ctx* ctx){
    libvlc_media_player_stop(ctx->mp);
    libvlc_media_player_release(ctx->mp);
    libvlc_release(ctx->inst);
}