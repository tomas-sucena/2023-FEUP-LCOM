// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "keyboard.h"
#include "video.h"

#define WAIT 5

vbe_mode_info_t mode_info;
int kbd_hook_id;
bool ih_error;
struct kbd_data data;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int disable_video(int flag){
    vg_exit();
    return flag;
}

int kbd_int_loop(){
    // global variables
    kbd_hook_id = 0;

    // local variables
    int ipc_status;
    message msg;
    uint8_t bit_no = 0;
    uint32_t mask = BIT(bit_no);

    int flag = kbd_subscribe_int(&bit_no);
    if (flag) return flag;
    
    while (data.scancode != KBD_ESC_BREAKCODE){
        flag = driver_receive(ANY, &msg, &ipc_status);
        if (flag){
            printf("driver_receive failed with: %d", flag);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;

        switch(_ENDPOINT_P(msg.m_source)){
            case HARDWARE : {
                bool subscribedInt = msg.m_notify.interrupts & mask;
                if (!subscribedInt) break;

                kbd_get_scancode(&data, WAIT);

                if (ih_error) return ih_error;
                if (!data.valid) break;
            }
            default : break;
        }
    }

    return kbd_unsubscribe_int();
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  /* To be completed */
  int flag = video_start(mode);
  if (flag) return disable_video(flag);

  flag = sleep(delay);
  if (flag) return disable_video(flag);

  return vg_exit();
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  int flag = video_start(mode);
  if (flag) return disable_video(flag);

  // draw the rectangle
  flag = video_draw_rectangle(x, y, width, height, color);
  if (flag) return disable_video(flag);

  flag = kbd_int_loop();
  if (flag) return disable_video(flag);

  return vg_exit();
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  /* To be completed */
  printf("%s(0x%03x, %u, 0x%08x, %d): under construction\n", __func__,
         mode, no_rectangles, first, step);

  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* To be completed */
  printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
