#include <lcom/lcf.h>

#include "video.h"

int video_set_graphics_mode(uint16_t mode){
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));

    reg86.intno = 0x10;
    reg86.ax = 0x4F02;
    reg86.bx = mode | BIT(14);

    int flag = sys_int86(&reg86);
    if (flag)
        perror("Setting the graphic mode failed!\n");

    return flag;
}
