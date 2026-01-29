#ifndef IMEJ_TYPES
#define IMEJ_TYPES
#include <stdint.h>
#ifdef IMEJ_32_BIT_COLOUR
#define BLACKCOL (colType){0x00000000}
#define WHITECOL (colType){0x00FFFFFF}

typedef union{
  uint32_t col;
  struct{
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
  };
}colType;

static inline colType scaleColour(colType col,float percent){
  return (colType){
    .r = (uint8_t)(col.r * percent),
    .g = (uint8_t)(col.g * percent),
    .b = (uint8_t)(col.b * percent)
  };
}

#else
#define BLACKCOL (colType){0x0000}
#define WHITECOL (colType){0x1CE7}
#define BGCOL 0
typedef union{
  uint16_t col;
  struct{
    uint16_t g: 5;
    uint16_t b: 5;
    uint16_t r: 6;
  };
}colType;

static inline colType scaleColour(colType colour, float percent){
  uint16_t r = ((colour.col&0xFC00)>>10)*percent;
  uint16_t b = ((colour.col&0x03E0)>>5)*percent;
  uint16_t g = (colour.col&0x001F)*percent; \
  colType beep;
  beep.col = ((r&0x003F)<<10) | ((b&0x001F)<<5) | (g&0x001F);
  return beep;
}

#endif
extern colType IMEJ_COL_BACKGROUND;
extern colType IMEJ_COL_DEFAULT;
#endif

