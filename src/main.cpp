#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

#include <SDL3/SDL.h>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixdesc.h>
#include <libavutil/hwcontext.h>
#include <libavutil/opt.h>
#include <libavutil/avassert.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

static enum AVPixelFormat hw_pix_fmt = AV_PIX_FMT_NONE;

static int xpp = 0;

void error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}

enum AVPixelFormat get_hw_format(AVCodecContext* ctx, const enum AVPixelFormat* pix_fmts)
{
    for (const enum AVPixelFormat* p = pix_fmts; *p != -1; p++) {
        if (*p == hw_pix_fmt)
            return *p;
    }
    fprintf(stderr, "Failed to get HW surface format.\n");
    return AV_PIX_FMT_NONE;
}

void decode_write(AVCodecContext* decoder_ctx, FILE* output_file, AVPacket* packet) {
    AVFrame* frame = NULL;
    AVFrame* sw_frame = NULL;
    AVFrame* tmp_frame = NULL;
    uint8_t* buffer = NULL;
    int size = 0;
    int ret = 0;
    ret = avcodec_send_packet(decoder_ctx, packet);
    while (1) {
        frame = av_frame_alloc();
        sw_frame = av_frame_alloc();
        ret = avcodec_receive_frame(decoder_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            av_frame_free(&frame);
            av_frame_free(&sw_frame);
            break;
        }

        if (frame->format == hw_pix_fmt) {
            ret = av_hwframe_transfer_data(sw_frame, frame, 0);
            tmp_frame = sw_frame;
        } else {
            tmp_frame = frame;
        }

        size = av_image_get_buffer_size((enum AVPixelFormat)tmp_frame->format, tmp_frame->width, tmp_frame->height, 1);
        buffer = (uint8_t*)av_malloc(size);
        ret = av_image_copy_to_buffer(buffer, size, (const uint8_t * const *)tmp_frame->data, (const int *)tmp_frame->linesize, (enum AVPixelFormat)tmp_frame->format, tmp_frame->width, tmp_frame->height, 1);
        ret = fwrite(buffer, 1, size, output_file);
        if(xpp == 0) {
            xpp = 1;
            uint8_t* rgb_buffer = (uint8_t*)av_malloc(tmp_frame->height * tmp_frame->width * 3);
            struct SwsContext* sws_ctx = sws_getContext(tmp_frame->width, tmp_frame->height, (enum AVPixelFormat)tmp_frame->format, tmp_frame->width, tmp_frame->height, AV_PIX_FMT_RGB24, SWS_POINT, NULL, NULL, NULL);
            uint8_t* dst_data[4] = { rgb_buffer, NULL, NULL, NULL };
            int dst_linesize[4] = { tmp_frame->width * 3, 0, 0, 0 };
            sws_scale(sws_ctx, tmp_frame->data, tmp_frame->linesize, 0, tmp_frame->height, dst_data, dst_linesize);
            stbi_write_png("png.png", tmp_frame->width, tmp_frame->height, 3, rgb_buffer, tmp_frame->width * 3);
            // TODO: make this function return the rgb buffer for each frame
        }

        av_frame_free(&frame);
        av_frame_free(&sw_frame);
        av_freep(&buffer);
    }
}

int main(int argc, char** argv) {
    AVFormatContext* input_ctx = NULL;
    AVStream* video = NULL;
    AVCodecContext* decoder_ctx = NULL;
    const AVCodec* decoder = NULL;
    AVPacket* packet = NULL;
    AVBufferRef* hw_device_ctx = NULL;
    const AVCodecHWConfig* config = NULL;
    FILE* output_file = NULL;
    int video_stream = 0;
    int ret = 0;
 
    if (argc < 3) error("Usage: %s <input file> <output file>", argv[0]);
 
    packet = av_packet_alloc();
    if (avformat_open_input(&input_ctx, argv[1], NULL, NULL) != 0) error("Cannot open input file '%s'", argv[1]);
    ret = avformat_find_stream_info(input_ctx, NULL);

    video_stream = av_find_best_stream(input_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &decoder, 0);
 
    for (int i = 0; config == NULL; i++) {
        config = avcodec_get_hw_config(decoder, i);
        if (config == NULL) break;
        if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX) {
            hw_pix_fmt = config->pix_fmt;
            break;
        }
    }
    if(hw_pix_fmt == AV_PIX_FMT_NONE) {
        error("No hardware for codec '%s'!", decoder->name);
    }
 
    decoder_ctx = avcodec_alloc_context3(decoder);
 
    video = input_ctx->streams[video_stream];
    ret = avcodec_parameters_to_context(decoder_ctx, video->codecpar);
 
    decoder_ctx->get_format = get_hw_format;
 
    ret = av_hwdevice_ctx_create(&hw_device_ctx, config->device_type, NULL, NULL, 0);
    if(hw_device_ctx == NULL) error("Could not create the device context!");
    decoder_ctx->hw_device_ctx = av_buffer_ref(hw_device_ctx);
 
    ret = avcodec_open2(decoder_ctx, decoder, NULL);
 
    output_file = fopen(argv[2], "w+b");

    while (ret >= 0) {
        ret = av_read_frame(input_ctx, packet);
        if (ret < 0) break;
 
        if (video_stream == packet->stream_index) decode_write(decoder_ctx, output_file, packet);
 
        av_packet_unref(packet);
    }
 
    decode_write(decoder_ctx, output_file, NULL);
 
    if (output_file) fclose(output_file);
    av_packet_free(&packet);
    avcodec_free_context(&decoder_ctx);
    avformat_close_input(&input_ctx);
    av_buffer_unref(&hw_device_ctx);
    return 0;
}
