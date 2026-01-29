#include "../include/imej.h"
#include "../include/window.h"
#include "../include/extras/frustum.h"
#include "../include/extras/cube.h"

#include <string.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef WHITECOL
#undef WHITECOL
#define WHITECOL (colType){0x00000000}
#endif
#ifdef BLACKCOL
#undef BLACKCOL
#define BLACKCOL (colType){0x00FFFFFF}
#endif
#ifdef BGCOL
#undef BGCOL
#define BGCOL 0x00FFFFFF
#endif

void update(Object* obj, int* frameNumber){
  rotateObject(obj,1,2,3);
}

// #define DISPLAY
int main(){
  Scene *s = createScene(500,500);
  if(s==NULL){
    return 1;
  }

#ifdef DISPLAY
  int windowSuccess = openWindow(s);
  if(windowSuccess == 0){
    destroyScene(s);
    return 1;
  }
#endif
  IMEJ_COL_BACKGROUND.col = 0x00FFFFFF;
  IMEJ_COL_DEFAULT = (colType){.r=0,.g=230,.b=230};
  Object* obj = loadObjectFromDisk("model.obj");
  #define RANGE 3
  for(int x=-RANGE;x<RANGE;x++){
    for(int y=-RANGE;y<RANGE;y++){
      for(int z=-RANGE;z<RANGE;z++){
        Object* obj2 = cloneObject(obj);
        translateObject(obj2,(vec3){x*3,y*3,z*3});
        addModelToScene(s,obj2);
      }
    }
  }
  deleteObject(obj);
  // scaleObject(obj,0.1);

#ifndef DISPLAY
#define FRAMES 100
  time_t start = clock();
  for(int i=0;i< FRAMES ;i++){
    renderScene(s);
  }
  time_t end = clock();
  printf("Rendered %d frames in %fs\n",FRAMES, ((double)(end - start))/CLOCKS_PER_SEC);
#else
  startRenderingLoop(s);
  closeWindow();
#endif
  destroyScene(s);
  return 1;
}
