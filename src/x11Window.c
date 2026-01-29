#include "window.h"
#include "scene.h"
#include "raster.h"
#ifdef IMEJ_DO_INPUT
#include "input.h"
#endif
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct displayHandler{
  Display*display;
  Window window;
  GC gc;
  XImage *im;
  Atom close;
  int width;
  int height;
}displayHandler;

static displayHandler* disp = NULL;

void closeWindow(){
  if(disp != NULL && disp->display != NULL){
    XCloseDisplay(disp->display);
    disp->display = NULL;
    disp = NULL; 
  }
}

void renderFrame(Scene* s){
  XPutImage(disp->display, disp->window, disp->gc, disp->im, 0, 0, 0, 0, disp->width,disp->height);
  XFlush(disp->display);
}

int openWindow(Scene* s){
  disp = malloc(sizeof(displayHandler));
  if(disp==NULL){
    return 0;
  }
  Rasterizer* r = getRasterizer(s);
  colType* framebuffer = getFramebuffer(r);

  Display *display = XOpenDisplay(NULL);
  if (!display) {
    fprintf(stderr, "Unable to open display.\n");
    free(disp);
    return 0;
  }

  int width = getFrameWidth(r);
  int height = getFrameHeight(r);
  disp->display=display;
  disp->height=height;
  disp->width=width;
  disp->im=XCreateImage(display, DefaultVisual(display,0),DefaultDepth(display,0),ZPixmap,0,(char*)framebuffer,width,height,32,width*sizeof(uint32_t));
  disp->window = XCreateSimpleWindow(display, XDefaultRootWindow(display), 0, 0, width, height, 0,0,0); 
  disp->gc = XCreateGC(display, disp->window, 0, NULL);
  XSelectInput(display, disp->window, KeyPressMask | ExposureMask | PointerMotionMask | ButtonReleaseMask | ButtonPressMask);
  XMapWindow(display, disp->window);
  disp->close = XInternAtom(display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(display, disp->window, &disp->close, 1);

  XPutImage(disp->display, disp->window, disp->gc, disp->im, 0, 0, 0, 0, width, height);
  return 1;
}

void startRenderingLoop(Scene* s){
  if(disp==NULL){
    return;
  }
  XEvent event;
  int quit =1;
  #ifdef IMEJ_DO_INPUT
  time_t lastFrame = clock();
  time_t currentFrame;
  struct InputHandler* input = getSceneInputHandler(s);
  #endif
  while(quit){
    while (XPending(disp->display) > 0) {
      XNextEvent(disp->display,&event);
      switch(event.type){
        case ClientMessage:
          if (event.xclient.data.l[0] == disp->close){
            quit=0;
            closeWindow();
            return;
          }
          break;
        case KeyPress:
          char c = XLookupKeysym(&event.xkey,0);
          if(c=='q'){
            quit=0;
          }
          #ifdef IMEJ_DO_INPUT
          if(input != NULL){
            input->handleKeypress(input,c);
          }
          #endif
        break;
          #ifdef IMEJ_DO_INPUT
        case MotionNotify:
          if(input!= NULL){
            input->handleMouseMove(input,event.xmotion.x,event.xmotion.y);
          }
        break;
        #endif
          #ifdef IMEJ_DO_INPUT
        case ButtonPress:
          if(event.xbutton.button==Button4){
            //scroll
          }
          if(event.xbutton.button==Button5){
            //scroll
          }
          if(event.xbutton.button==Button1){
            if(input!= NULL){
              input->handleMouseClick(input,event.xmotion.x,event.xmotion.y);
            }
          }
          break;
        case ButtonRelease:
          if(input!= NULL){
            input->handleMouseRelease(input,event.xmotion.x,event.xmotion.y);
          }
        break;
#endif
      }
    }
#ifdef IMEJ_DO_INPUT
    currentFrame = clock();
    if(input != NULL){
      input->tick(input,(float)(currentFrame-lastFrame)*1000/CLOCKS_PER_SEC);
    }
    lastFrame = currentFrame;
#endif
    renderFrame(s);
    renderScene(s);
  }
  closeWindow();
}
