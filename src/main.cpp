#include "player.h"
#include <raylib.h>
#include <raygui.h>

// TODO:
// Add Raylib/sdl2 (android) GUI https://www.raylib.com/
// https://chatgpt.com/c/68a9bfcc-d9c4-8325-ad7b-64c16fae8368

typedef struct {
    int width;
    int height;
} Window;

int main(int argc, char** argv) {
    const char *video_path = "D:\\muzyka\\minimal\\Deep⧸Minimal Drum and Bass Mix #1-(720p30).mp4";

    if(argc > 1){
        video_path = argv[1];
    }

    Player_ctx player_ctx = player_init();

    Window win = {800,480};
    InitWindow(win.width,win.height,"BVLC");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    filesystem::path path_to_exe = filesystem::weakly_canonical(filesystem::path(argv[0]));
    const char* full_font_path = (path_to_exe / "font.ttf").u8string().c_str();
    GuiSetFont(LoadFont(full_font_path));

    while(!WindowShouldClose()){
        win.width = GetScreenWidth(); win.height = GetScreenHeight();

        BeginDrawing();

        ClearBackground((Color){0,0,0,255});
        GuiButton((Rectangle){0,win.height - 80.0f,80.0f,80.0f},"XD");

        EndDrawing();
    }

    /*
    player_play(&player_ctx,video_path);
    player_stop(&player_ctx);
    player_exit(&player_ctx);*/
    return 0;
}
