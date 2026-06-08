#include "games.h"
#include <Arduino.h>

static const char *cur_banner = NULL;

void banner_begin(const char *buf) {
  cur_banner = buf;
}

void banner_step() {
  if (cur_banner == NULL) {
    return;
  }
  char c = pgm_read_byte(cur_banner);
  if (c == 0) {
    cur_banner = NULL;
    return;
  }
  cur_banner++;
  Serial.write(c);
}