#ifndef _LCOM_VIDEO_H_
#define _LCOM_VIDEO_H_

#include <stdint.h>

int (video_start)(uint16_t mode);

int (video_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (video_draw_row)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (video_draw_col)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (video_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

#endif // _LCOM_VIDEO_H_
