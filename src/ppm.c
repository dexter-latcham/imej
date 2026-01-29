#include "window.h"
#include <stdio.h>
#include "scene.h"
#include "raster.h"
#include <string.h>
#include <stdlib.h>

typedef struct displayHandler{
  char filename[30];
  Scene* scene;
  int width;
  int height;
}displayHandler;

static displayHandler* disp = NULL;

void saveP6(char* filename, Rasterizer* r){
  FILE *fp = fopen(filename, "wb"); 
  if(fp == NULL){
    printf("Error saving to file\n");
    return;
  }
  colType* framebuffer = getFramebuffer(r);
  int width = getFrameWidth(r);
  int height = getFrameHeight(r);
  fprintf(fp, "P6\n%d %d\n255\n", width, height);
  for (int j = 0; j < height; j++){
    for (int i = 0; i < width;i++){
      fputc(framebuffer[j*width+i].r,fp);
      fputc(framebuffer[j*width+i].g,fp);
      fputc(framebuffer[j*width+i].b,fp);
    }
  }
  fclose(fp);
}

void saveP3(char* filename, Rasterizer* r){
  FILE *fp = fopen(filename, "wb"); 
  if(fp == NULL){
    printf("Error saving to file\n");
    return;
  }
  colType* framebuffer = getFramebuffer(r);
  int width = getFrameWidth(r);
  int height = getFrameHeight(r);
  fprintf(fp, "P3\n%d %d\n255\n", width, height);
  for (int j = 0; j < height; j++){
    for (int i = 0; i < width;i++){
      colType pixel = framebuffer[j*width+i];
      fprintf(fp, "%d %d %d\n",pixel.r,pixel.g,pixel.b);
    }
  }
  fclose(fp);
}

void startRenderingLoop(Scene* s){
  int frame;
  for(int i=0; i<100;i++){
    frame = *(getSceneFrameCounter(s));
    sprintf(disp->filename,"frames/frame%03d.ppm",frame);
    renderScene(s);
    renderFrame(s);
  }
}

void renderFrame(Scene* s){
  Rasterizer* r = getRasterizer(s);
  saveP6(disp->filename,r);
}

void closeWindow(){
  if(disp!=NULL){
    free(disp);
  }
}

int openWindow(Scene* s){
  if(disp!=NULL){
    return 1;
  }
  disp = malloc(sizeof(displayHandler));
  if(disp==NULL){
    return 0;
  }
  Rasterizer* r = getRasterizer(s);
  strcpy(disp->filename,"frames/frame000.ppm");
  disp->scene=s;
  int width = getFrameWidth(r);
  int height = getFrameHeight(r);
  disp->width=width;
  disp->height=height;
  return 1;
}
