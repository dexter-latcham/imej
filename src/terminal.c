#include "window.h"
#include "scene.h"
#include "raster.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#ifdef __linux__
#include <sys/ioctl.h>
#include <stdio.h>
#define IMEJ_PRINT printf
#elif defined(MICROBIT_DAL_VERSION)
#include "MicroBit.h"
MicroBit uBit;
'define IMEJ_PRINT uBit.serial.printf'
#endif

typedef struct displayHandler{
  int termWidth;
  int termHeight;
  int scaleFactor;
  Scene* scene;
}displayHandler;

static displayHandler* disp = NULL;

void renderFrame(Scene* s){
  Rasterizer* r = getRasterizer(s);
  colType* framebuffer = getFramebuffer(r);
  int frameWidth = getFrameWidth(r);
  int frameHeight = getFrameHeight(r);
  if(disp->scaleFactor == 1){
    for(int y=0;y<disp->termHeight;y++){
      for(int x=0;x<disp->termWidth;x++){
        colType col = framebuffer[(y*frameWidth)+x];
        IMEJ_PRINT("\033[48;2;%03u;%03u;%03um ",col.r,col.g,col.b);
      }
      IMEJ_PRINT("\033[0m\n");;
    }
  }else{
    for(int y=0;y<disp->termHeight;y++){
      for(int x=0;x<disp->termWidth;x++){
        float r =0;
        float g =0;
        float b =0;
        for(int y1=y*disp->scaleFactor;y1<(y+1)*disp->scaleFactor;y1++){
          for(int x1=x*disp->scaleFactor;x1<(x+1)*disp->scaleFactor;x1++){
            colType col = framebuffer[(y1*frameWidth)+x1];
            r+=(col.r*col.r);
            g+=(col.g*col.g);
            b+=(col.b*col.b);
          }
        }
        r=sqrt(r/(disp->scaleFactor*disp->scaleFactor));//averaging sample
        g=sqrt(g/(disp->scaleFactor*disp->scaleFactor));
        b=sqrt(b/(disp->scaleFactor*disp->scaleFactor));
        IMEJ_PRINT("\033[48;2;%03u;%03u;%03um ",(int)r,(int)g,(int)b);
      }
      IMEJ_PRINT("\033[0m\n");;
    }
  }
  IMEJ_PRINT("\033[%dA", disp->termHeight); //goto 0x0
  IMEJ_PRINT("\033[%dD", disp->termWidth); //goto 0x0
}


int openWindow(Scene* s){
#if defined(MICROBIT_DAL_VERSION)
  extern MicroBit uBit;
  uBit.init();
  uBit.sleep(200);
#endif
  if(disp!=NULL){
    return 1;
  }
  disp = malloc(sizeof(displayHandler));
  if(disp==NULL){
    return 0;
  }
  Rasterizer* r = getRasterizer(s);
  int frameWidth = getFrameWidth(r);
  int frameHeight = getFrameHeight(r);

#ifdef __linux__
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);
  disp->termWidth = w.ws_col;
  disp->termHeight = w.ws_row ;

#elif defined(MICROBIT_DAL_VERSION)
  disp->termWidth = frameWidth;
  disp->termHeight = frameHeight;
#endif
  int yScaleFactor = ceil((float)frameHeight/disp->termHeight);
  int xScaleFactor = ceil((float)frameWidth/disp->termWidth);
  if(yScaleFactor > xScaleFactor){
    disp->termWidth = frameWidth/yScaleFactor;
    disp->termHeight = frameHeight/yScaleFactor;
    disp->scaleFactor = yScaleFactor;
  }else{
    disp->termWidth = frameWidth/xScaleFactor;
    disp->termHeight = frameHeight/xScaleFactor;
    disp->scaleFactor = xScaleFactor;
  }
  return 1;
}

void closeWindow(){
  if(disp!=NULL){
    free(disp);
  }
  return;
}

void startRenderingLoop(Scene* s){
  while(1){
    renderFrame(s);
    renderScene(s);
  }
}
