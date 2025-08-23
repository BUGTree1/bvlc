#include <stdio.h>
#include <stdlib.h>
#include <vlc/vlc.h>

int main(int argc, char **argv) {
    printf("pozdro\n");
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path-to-video>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *video_path = argv[1];

    // 1. Create a new libVLC instance
    libvlc_instance_t *inst = libvlc_new(0, NULL);
    if (!inst) {
        fprintf(stderr, "Failed to initialize libVLC\n");
        return EXIT_FAILURE;
    }

    // 2. Create a media descriptor for the given video file
    libvlc_media_t *media = libvlc_media_new_path(inst, video_path);
    if (!media) {
        fprintf(stderr, "Failed to create media for %s\n", video_path);
        libvlc_release(inst);
        return EXIT_FAILURE;
    }

    // 3. Create a media player using the media descriptor
    libvlc_media_player_t *mp = libvlc_media_player_new_from_media(media);

    // We no longer need the media descriptor separately
    libvlc_media_release(media);

    // 4. Start playing the video
    if (libvlc_media_player_play(mp) != 0) {
        fprintf(stderr, "Failed to play media\n");
        libvlc_media_player_release(mp);
        libvlc_release(inst);
        return EXIT_FAILURE;
    }

    // 5. Let the video play for a bit
    printf("Playing… Press Enter to exit.\n");
    getchar();

    // 6. Cleanup
    libvlc_media_player_stop(mp);
    libvlc_media_player_release(mp);
    libvlc_release(inst);

    return EXIT_SUCCESS;
}
