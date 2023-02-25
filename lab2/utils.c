#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  lsb = NULL;
  *lsb = (uint8_t) val;

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  msb = NULL;
  *msb = (uint8_t) (val >> 8);

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t* in = NULL; *in = 0;
  
  int flag = sys_inb(port, in);
  if (flag) return flag;
  if (in == NULL) return 1;
  
  value = NULL;
  *value = (uint8_t) *in;

  return 0;
}
