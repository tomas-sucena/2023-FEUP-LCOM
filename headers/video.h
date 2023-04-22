#ifndef _LCOM_VIDEO_H_
#define _LCOM_VIDEO_H_

#include <lcom/lcf.h>
#include <stdint.h>

#include "VBE.h"

#define RED(color) ((color & mode_info.red_mask) >> mode_info.red_begin)
#define GREEN(color) ((color & mode_info.green_mask) >> mode_info.green_begin)
#define BLUE(color) ((color & mode_info.blue_mask) >> mode_info.blue_begin)

typedef struct {
    uint8_t mode;
    uint8_t memory_model;
    phys_bytes physical_base;
    uint8_t bits_per_pixel, bytes_per_pixel;
    uint16_t x_res, y_res;
    uint32_t red_mask, green_mask, blue_mask;
    uint8_t red_begin, green_begin, blue_begin;
    uint8_t red_end, green_end, blue_end;
} video_mode_info;

int (video_get_mode_info)(uint16_t mode);
int (video_start)(uint16_t mode);

int (video_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (video_draw_row)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (video_draw_col)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (video_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (video_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

#endif // _LCOM_VIDEO_H_
