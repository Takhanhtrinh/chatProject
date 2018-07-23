#ifndef CONFIG_H
#define CONFIG_H
#include <stdio.h>
#include <stdlib.h>
typedef uint8_t byte;
#define DEBUG 1
union Float {
  float floatValue;
  int intValue;
};
const int MAX_ROOM_NAME = 255;
#endif
