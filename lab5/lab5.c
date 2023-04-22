// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "keyboard.h"
#include "video.h"

#define WAIT 5

video_mode_info mode_info;
int kbd_hook_id;
bool ih_error;
kbd_data data;

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
	int flag = video_start(mode);
	if (flag) return disable_video(flag);

	uint16_t painted_rows = mode_info.y_res - mode_info.y_res % no_rectangles;
	uint16_t painted_cols = mode_info.x_res - mode_info.x_res % no_rectangles;
	
	uint16_t width = mode_info.x_res / no_rectangles;
    uint16_t height = mode_info.y_res / no_rectangles;
	
	uint16_t x = 0, y = 0;

    // draw the rectangles
	for (int i = 0; i < no_rectangles * no_rectangles; ++i){
        uint32_t color = 0;
        uint8_t row = y / height, col = x / width;

        // compute the color
        if (mode_info.memory_model == VBE_DIRECT_MODE){
            uint32_t red = (RED(first) + col * step) % (1 << (mode_info.red_end - mode_info.red_begin));
            red <<= mode_info.red_begin;

            uint32_t green = (GREEN(first) + row * step) % (1 << (mode_info.green_end - mode_info.green_begin));
            green <<= mode_info.green_begin;

            uint32_t blue = (BLUE(first) + (col + row) * step) % (1 << (mode_info.blue_end - mode_info.blue_begin));
            blue <<= mode_info.blue_begin;

            color = red | green | blue;
        }
        else {
            color = (first + (row * no_rectangles + col) * step) % (1 << mode_info.bits_per_pixel);
        }
        
        flag = video_draw_rectangle(x, y, width, height, color);
		if (flag) return disable_video(flag);

		x += width;
		if (x < painted_cols) continue;
		
		x = 0;
		y += height;
	}

	// draw the black stripes
	for (int i = painted_rows; i < mode_info.y_res; ++i){
		flag = video_draw_row(0, i, mode_info.x_res, 0);
		if (flag) return disable_video(flag);
	}

	for (int i = painted_cols; i < mode_info.x_res; ++i){
		flag = video_draw_col(i, 0, mode_info.y_res, 0);
		if (flag) return disable_video(flag);
	}

	flag = kbd_int_loop();
	if (flag) return disable_video(flag);

	return vg_exit();
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    int flag = video_start(0x105);
    if (flag) return disable_video(flag);

    // draw the XPM
    flag = video_draw_xpm(xpm, x, y);
    if (flag) return disable_video(flag);

    flag = kbd_int_loop();
    if (flag) return disable_video(flag);

    return vg_exit();
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
