
  #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <colour.h>
#include "raster.h"
#include <math.h>

colType IMEJ_COL_BACKGROUND = BLACKCOL;
colType IMEJ_COL_DEFAULT = WHITECOL;

struct Rasterizer{
  int width;
  int height;
  uint16_t * zbuffer;
  colType* framebuffer;
};


int getFrameWidth(Rasterizer* r){
  return r->width;
}
int getFrameHeight(Rasterizer* r){
  return r->height;
}
colType*  getFramebuffer(Rasterizer* r){
  return r->framebuffer;
}
Rasterizer * createRasterizer(int width, int height){
  Rasterizer *r = (Rasterizer*)malloc(sizeof(Rasterizer));
  if(r==NULL){
    return NULL;
  }
  r->zbuffer=(uint16_t*)malloc(width*height*sizeof(uint16_t));
  if(r->zbuffer == NULL){
    goto zbufferAllocFail;
  }
  r->framebuffer = (colType*)malloc(width*height*sizeof(colType));
  if(r->framebuffer == NULL){
    goto framebufferAllocFail;
  }
  r->width=width;
  r->height=height;
  wipeFrameBuffer(r);
  return r;
framebufferAllocFail:
  free(r->zbuffer);
zbufferAllocFail:
  free(r);
  return NULL;
}
void  wipeFrameBuffer(Rasterizer* r){
  memset(r->framebuffer,IMEJ_COL_BACKGROUND.col,r->width*r->height*sizeof(colType));
  memset(r->zbuffer,0,r->width*r->height*sizeof(uint16_t));
#ifdef IMEJ_DRAWBORDERS
  for(int x=0;x<r->width;x++){
    r->framebuffer[x]=IMEJ_COL_DEFAULT;
    int index = ((r->height-1)*(r->width-1))+x;
    r->framebuffer[index]=IMEJ_COL_DEFAULT;
  }
  for(int y=0;y<r->height-1;y++){
    r->framebuffer[((r->width)*y)]=IMEJ_COL_DEFAULT;
    r->framebuffer[((r->width)*y)+(r->width-1)]=IMEJ_COL_DEFAULT;
  }
#endif
}

void destroyRasterizer(Rasterizer* r){
  if(r != NULL){
    if(r->framebuffer!=NULL){
      free(r->framebuffer);
    }
    if(r->zbuffer != NULL){
      free(r->zbuffer);
    }
    free(r);
  }
}

void  drawPoint(Rasterizer * r, int x, int y, uint16_t z,colType col){
  int index = (y*r->width)+x;
  if(index < 0 || index > (r->width * r->height)){
    return;
  }
  if(r->zbuffer[index]==0 || (r->zbuffer[index] > z)){
    r->zbuffer[index]=z;
    r->framebuffer[index]=col;
  }
}


void ddaLine(Rasterizer* r,
               float x1, float y1, float z1,
               float x2, float y2, float z2,
               colType col){
  float x,y,z,dx,dy,dz,step;
  dx = (x2 - x1);
  dy = (y2 - y1);
  dz = (z2 - z1);
  if(fabs(dx) >= fabs(dy)){
    step = fabs(dx);
  }else{
    step = fabs(dy);
  }
  dx = dx/step;
  dy = dy/step;
  dz = dz/step;
  x=x1;
  y = y1;
  z = z1;
  int i=0;
  while(i<step){
    drawPoint(r,x,y,z,col);
    x+=dx;
    y+=dy;
    z+=dz;
    i+=1;
  }
  drawPoint(r,x,y,z,col);
}

void  drawLineNDC(Rasterizer * r, 
               float x1, float y1, float z1,
               float x2, float y2, float z2,
               colType col){
  x1=(x1+1)*(r->width/2.0);
  x2=(x2+1)*(r->width/2.0);
  y1=(y1+1)*(r->height/2.0);
  y2=(y2+1)*(r->height/2.0);
  z1=z1*10000;
  z2=z2*10000;
  ddaLine(r,x1,y1,z1,x2,y2,z2,col);
}

void  drawLine(Rasterizer * r,
               float x1, float y1, float z1,
               float x2, float y2, float z2,
               colType col){
  ddaLine(r,x1,y1,z1,x2,y2,z2,col);
}



void  drawTriangleNDC(Rasterizer* r,
               float x1, float y1, float z1,
               float x2, float y2, float z2,
               float x3, float y3, float z3,
               colType col){

  x1=(x1+1)*(r->width/2.0);
  x2=(x2+1)*(r->width/2.0);
  x3=(x3+1)*(r->width/2.0);
  y1=(y1+1)*(r->height/2.0);
  y2=(y2+1)*(r->height/2.0);
  y3=(y3+1)*(r->height/2.0);
  z1=z1*10000;
  z2=z2*10000;
  z3=z3*10000;
  drawTriangle(r,
               x1,y1,z1,
               x2,y2,z2,
               x3,y3,z3,col);
}

void  drawTriangle(Rasterizer* r,
               float x1, float y1, float z1,
               float x2, float y2, float z2,
               float x3, float y3, float z3,
               colType col){
  float tmp;

  if (y1> y2) {
    tmp = x1;
    x1=x2;
    x2=tmp;
    tmp = y1;
    y1 = y2;
    y2 = tmp;
    tmp = z1;
    z1 = z2;
    z2 = tmp;
  }
  if (y2 > y3) {
    tmp = x2;
    x2 = x3;
    x3 = tmp;
    tmp = y2;
    y2 = y3;
    y3 = tmp;
    tmp = z2;
    z2 = z3;
    z3 = tmp;
  }
  if (y1> y2) {
    tmp = x1;
    x1=x2;
    x2=tmp;
    tmp = y1;
    y1 = y2;
    y2 = tmp;
    tmp = z1;
    z1 = z2;
    z2 = tmp;
  }
  if(y3 == y1){
    return;
  }

  float dy1 = (y3 - y1);

  float dx2=(x3-x1) / dy1;
  float dz2=(z3-z1) / dy1;

  float xl=x1;
  float xr=x1;
  float zl=z1;
  float zr=z1;

  float dy2 = (y2 - y1);
  if(y2 != y1){
    float dx1=(x2-x1) / dy2;
    float dz1=(z2-z1) / dy2;
    int yStart = y1;
    int yEnd = y2;
    for(int y = yStart; y < yEnd; y++){
      ddaLine(r,xl,y,zl,xr,y,zr,col);
      xl+=dx1;
      xr+=dx2;
      zl+=dz1;
      zr+=dz2;
    }
  }
  if(y3 == y2){
    return;
  }

  float dy3 = (y3 - y2);
  float dx3=(x3-x2) / dy3;
  float dz3=(z3-z2) / dy3;
  int yStart = y2;
  int yEnd = y3;
  xl=x2;
  zl=z2;
  int y;
  for(y=yStart;y<yEnd;y++){
    ddaLine(r,xl,y,zl,xr,y,zr,col);
    xl+=dx3;
    xr+=dx2;
    zl+=dz3;
    zr+=dz2;
  }
}

// void sortPointsByY(vec3 *p1, vec3 *p2, vec3 *p3) {
//   if (p1->y > p2->y) {
//     vec3 temp = *p1;
//     *p1 = *p2;
//     *p2 = temp;
//   }
//   if (p2->y > p3->y) {
//     vec3 temp = *p2;
//     *p2 = *p3;
//     *p3 = temp;
//   }
//   if (p1->y > p2->y) {
//     vec3 temp = *p1;
//     *p1 = *p2;
//     *p2 = temp;
//   }
// }

// void  drawTriangleOld(Canvas * c, vec3 p1, vec3 p2, vec3 p3, colType col){
//   sortPointsByY(&p1,&p2,&p3);
//   if(p3.y == p1.y){
//     return;
//   }
//   p1.x=(p1.x+1)*(c->width/2.0);
//   p2.x=(p2.x+1)*(c->width/2.0);
//   p3.x=(p3.x+1)*(c->width/2.0);
//   p1.y=(p1.y+1)*(c->height/2.0);
//   p2.y=(p2.y+1)*(c->height/2.0);
//   p3.y=(p3.y+1)*(c->height/2.0);

//   p1.z=p1.z*1000;
//   p2.z=p2.z*1000;
//   p3.z=p3.z*1000;

//   float dy1 = (p3.y - p1.y);

//   float dx2=(p3.x-p1.x) / dy1;
//   float dz2=(p3.z-p1.z) / dy1;

//   float xl=p1.x;
//   float xr=p1.x;
//   float zl=p1.z;
//   float zr=p1.z;

//   float dy2 = (p2.y - p1.y);
//   if(p2.y != p1.y){
//     float dx1=(p2.x-p1.x) / dy2;
//     float dz1=(p2.z-p1.z) / dy2;
//     int yStart = p1.y;
//     int yEnd = p2.y;
//     for(int y = yStart; y < yEnd; y++){
//       ddaLine(c,xl,y,zl,xr,y,zr,col);
//       xl+=dx1;
//       xr+=dx2;
//       zl+=dz1;
//       zr+=dz2;
//     }
//   }
//   if(p3.y == p2.y){
//     return;
//   }

//   float dy3 = (p3.y - p2.y);
//   float dx3=(p3.x-p2.x) / dy3;
//   float dz3=(p3.z-p2.z) / dy3;
//   int yStart = p2.y;
//   int yEnd = p3.y;
//   xl=p2.x;
//   zl=p2.z;
//   for(int y=yStart;y<yEnd;y++){
//     ddaLine(c,xl,y,zl,xr,y,zr,col);
//     xl+=dx3;
//     xr+=dx2;
//     zl+=dz3;
//     zr+=dz2;
//   }
// }
// int edgeFunction(vec3 p1, vec3 p2, vec3 testPoint){
//   float edge =  (testPoint.x - p1.x)*(p2.y - p1.y) - (testPoint.y - p1.y)*(p2.x - p1.x);
//   return edge > 0 ? 1 : 0;
// }


// float maxF(float f1, float f2){
//   return (f1 > f2) ? f1 : f2;
// }

// float minF(float f1, float f2){
//   return (f1 < f2) ? f1 : f2;
// }

// void  drawTrianglePineda(Canvas * c, vec3 p1, vec3 p2, vec3 p3, colType col){
//   p1.x=(p1.x+1)*(c->width/2.0);
//   p2.x=(p2.x+1)*(c->width/2.0);
//   p3.x=(p3.x+1)*(c->width/2.0);
//   p1.y=(p1.y+1)*(c->height/2.0);
//   p2.y=(p2.y+1)*(c->height/2.0);
//   p3.y=(p3.y+1)*(c->height/2.0);
//   p1.z=p1.z*10000;
//   p2.z=p2.z*10000;
//   p3.z=p3.z*10000;
//   float xmax = maxF(maxF(p1.x,p2.x),p3.x);
//   float xmin = minF(minF(p1.x,p2.x),p3.x);
//   float ymax = maxF(maxF(p1.y,p2.y),p3.y);
//   float ymin = minF(minF(p1.y,p2.y),p3.y);
//   for(int y=ymin;y<ymax;y++){
//     for(int x=xmin;x<xmax;x++){
//       if(edgeFunction(p1,p2,(vec3){x,y,0}) &&
//          edgeFunction(p2,p3,(vec3){x,y,0}) &&
//          edgeFunction(p3,p1,(vec3){x,y,0})){
//         drawPoint(c,x,y,0,col);
//       }
//     }
//   }
// }

