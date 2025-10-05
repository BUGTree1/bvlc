#include "player.h"


Player_ctx player_init(){
    Player_ctx ctx = {};
    ctx.inst = (libvlc_instance_t*)errorz(libvlc_new(0, NULL),"Failed to initialize libVLC\n");
    return ctx;
}

void player_play(Player_ctx* ctx, filesystem::path media_path){
    libvlc_media_t* media = (libvlc_media_t*)errorz(libvlc_media_new_path(ctx->inst, media_path.generic_u8string().c_str()),"Failed to create media\n");
    ctx->mp = libvlc_media_player_new_from_media(media);
    libvlc_media_release(media);
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