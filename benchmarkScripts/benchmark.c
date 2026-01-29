#include "../include/window.h"
#include "../include/imej.h"
#include "../include/extras/cube.h"
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void update(Object* obj, int* frameNumber){
  rotateObject(obj,0.1,0.1,0.1);
  //   rotateObject(obj,0.1,0.7,-0.1);
  //   if(((*frameNumber)%2)==0){
  //     scaleObject(obj,2);
  //     translateObject(obj,(vec3){0,0,0.2});
  //   }else{
  //     translateObject(obj,(vec3){0,0,-0.2});
  //     scaleObject(obj,1.0f/2);
  //   }
  // }
}

// #define window
int main(){
  Scene *s = createScene(100,100);
  if(s==NULL){
    printf("COULDNE ALLOC SCENE\n");
    return 1;
  }
#ifdef window
  int windowSuccess = openWindow(s);
  if(windowSuccess == 0){
    printf("COULDNT OPEN WINDOW\n");
    destroyScene(s);
    return 1;
  }
#endif

  Object* obj = loadObjectFromDisk("model.obj");
  addModelToScene(s,obj);
  Object* obj2;
  colType col;
  srand(time(0));
  for(int y=-5;y<5;y++){
    for(int x=-5;x<5;x++){
      for(int z=-5;z<5;z++){
        if(x==0 && y==0 && z==0){
          continue;
        }
        col.r = rand() % 255;
        col.g = rand() % 255;
        col.b = rand() % 255;
        obj2 = cloneObject(obj);
        if(obj2 == NULL){
          goto cleanup;
        }
        obj->update=update;
        obj2->col = col;
        scaleObject(obj2,0.1);
        translateObject(obj2,(vec3){x/2,y/2,z/2});
        obj2->update=update;
        addModelToScene(s,obj2);
      }
    }
  }

#ifdef window
  startRenderingLoop(s);
#else
  #define FRAMES 100
  time_t start = clock();
  for(int i=0;i< FRAMES ;i++){
    // renderFrame(ctx);
    renderScene(s);
  }
  time_t end = clock();
  printf("Rendered %d frames in %fs\n",FRAMES, ((double)(end - start))/CLOCKS_PER_SEC);
#endif
cleanup:
#ifdef window
  closeWindow();
#endif
  destroyScene(s);
  return 1;
}

