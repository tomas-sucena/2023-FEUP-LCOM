#include <lcom/lcf.h>

#include "video.h"

void* video_mem;
extern vbe_mode_info_t mode_info;

int video_set_mode(uint16_t mode){
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));

    reg86.intno = 0x10;
    reg86.ax = 0x4F02;
    reg86.bx = mode | BIT(14);

    int flag = sys_int86(&reg86);
    if (flag){
        printf("Initializing graphics mode 0x%x failed!\n", mode);
        return flag;
    }
    
    flag = vbe_get_mode_info(mode, &mode_info);
    return flag;
}

int (video_start)(uint16_t mode){
    int flag = video_set_mode(mode);
    if (flag) return flag;
    
    // allow memory mapping
    struct minix_mem_range mr;
    unsigned int vram_base = mode_info.PhysBasePtr;
    unsigned int vram_limit = mode_info.XResolution * mode_info.YResolution * mode_info.BitsPerPixel;

    mr.mr_base = (phys_bytes) vram_base;	
    mr.mr_limit = mr.mr_base + vram_limit;

    flag = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr);
    if (flag){
        panic("sys_privctl (ADD_MEM) failed: %d\n", flag);
        return flag;
    }
    
    // map memory
    if ((video_mem = vm_map_phys(SELF, (void*) mr.mr_base, vram_limit)) == MAP_FAILED){
        perror("Couldn't map video memory!");
        return -1;
    }

    return 0;
}
