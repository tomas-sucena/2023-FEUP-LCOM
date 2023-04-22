#ifndef _LCOM_VIDEO_H_
#define _LCOM_VIDEO_H_

#include <lcom/lcf.h>
#include <stdint.h>

#include "VBE.h"

struct video_mode_info {
    uint8_t mode;
    uint8_t memory_model;
    phys_bytes physical_base;
    uint8_t bits_per_pixel, bytes_per_pixel;
    uint16_t x_res, y_res;
    uint32_t red, green, blue;
};

int (video_get_mode_info)(uint16_t mode);
int (video_start)(uint16_t mode);

int (video_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (video_draw_row)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (video_draw_col)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (video_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

#endif // _LCOM_VIDEO_H_
