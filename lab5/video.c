#include <lcom/lcf.h>

#include "video.h"

uint8_t* video_mem;
extern vbe_mode_info_t mode_info;

int (video_start)(uint16_t mode){
    int flag = vbe_get_mode_info(mode, &mode_info);
    if (flag) return flag;

    uint8_t bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8; // rounding by excess
    unsigned int vram_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;

    struct minix_mem_range mr;
    mr.mr_base = (phys_bytes) mode_info.PhysBasePtr;	
    mr.mr_limit = mr.mr_base + vram_size;

    // allow memory mapping
    flag = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr);
    if (flag){
        panic("sys_privctl (ADD_MEM) failed: %d\n", flag);
        return flag;
    }
    
    // map the video memory to the address space of the process
    if ((video_mem = vm_map_phys(SELF, (void*) mr.mr_base, vram_size)) == NULL){
        perror("Couldn't map video memory!");
        return -1;
    }

    // change the video mode
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));

    reg86.intno = 0x10;
    reg86.ax = 0x4F02;
    reg86.bx = mode | BIT(14);

    flag = sys_int86(&reg86);
    if (flag)
        printf("Initializing graphics mode 0x%x failed!\n", mode);

    return flag;
}

int (video_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
    if (x > mode_info.XResolution || y > mode_info.YResolution)
        return 1;

    uint8_t bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    uint32_t pixel_index = (y * mode_info.XResolution + x) * bytes_per_pixel;

    memcpy(&video_mem[pixel_index], &color, bytes_per_pixel);
    return (video_mem == NULL);
}

int (video_draw_row)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    if (x > mode_info.XResolution || y > mode_info.YResolution)
        return 1;

    if (x + len > mode_info.XResolution)
        len = mode_info.XResolution - x;

    uint8_t bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    uint32_t pixel_index = (y * mode_info.XResolution + x) * bytes_per_pixel;

    while (len--){
        memcpy(&video_mem[pixel_index], &color, bytes_per_pixel);
        pixel_index += bytes_per_pixel;

        int flag = (video_mem == NULL);
        if (flag) return flag;
    }
    
    return 0;
}

int (video_draw_col)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    if (x > mode_info.XResolution || y > mode_info.YResolution)
        return 1;

    if (y + len > mode_info.YResolution)
        len = mode_info.YResolution - y;

    uint8_t bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    uint32_t pixel_index = (y * mode_info.XResolution + x) * bytes_per_pixel;

    while (len--){
        memcpy(&video_mem[pixel_index], &color, bytes_per_pixel);
        pixel_index += mode_info.XResolution * bytes_per_pixel;
        
        int flag = (video_mem == NULL);
        if (flag) return flag;
    }
    
    return 0;
}

int (video_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    if (y + height > mode_info.YResolution)
        height = mode_info.YResolution - y;
    
    for (uint16_t i = 0; i < height; ++i){
        int flag = video_draw_row(x, y + i, width, color);
        if (flag) return flag;
    }

    return 0;
}
