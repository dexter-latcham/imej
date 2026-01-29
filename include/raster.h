#ifndef IMEJ_RASTER
#define IMEJ_RASTER
#include "colour.h"

#ifdef __cplusplus
extern "C" {
#endif
#define ZBUFFERLIMIT 65535
typedef struct Rasterizer Rasterizer;

int getFrameWidth(Rasterizer* r);
int getFrameHeight(Rasterizer* r);
colType* getFramebuffer(Rasterizer* r);
Rasterizer * createRasterizer(int width, int height);
void destroyRasterizer(Rasterizer* r);
void  wipeFrameBuffer(Rasterizer* r);
void  drawPoint(Rasterizer * r, int x, int y, uint16_t z,colType col);

void  drawLineNDC(Rasterizer * r, 
               float x1, float y1, float z1,
               float x2, float y2, float z2,
               colType col);
void  drawLine(Rasterizer * r, 
               float x1, float y1, float z1,
               float x2, float y2, float z2,
               colType col);

void  drawTriangleNDC(Rasterizer * r, 
               float x1, float y1, float z1,
               float x2, float y2, float z2,
               float x3, float y3, float z3,
               colType col);
void  drawTriangle(Rasterizer * r, 
               float x1, float y1, float z1,
               float x2, float y2, float z2,
               float x3, float y3, float z3,
               colType col);

#ifdef __cplusplus
}
#endif
#endif

