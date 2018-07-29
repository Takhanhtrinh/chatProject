#ifndef CONFIG_H
#define CONFIG_H
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
typedef uint8_t byte;
#define DEBUG 1
union Float {
  float floatValue;
  int intValue;
};
const int MAX_ROOM_NAME = 255;
const int MAX_PIXEL_SIZE = 128;
#endif
