#include "../include/imej.h"
#include "../include/window.h"
#include "../include/extras/cube.h"
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
// #define DO_DISPLAY
#define DIMS 500
int main(){
  Scene *s = createScene(DIMS,DIMS);
  if(s==NULL){
    return 1;
  }
#ifdef DO_DISPLAY
  int windowSuccess = openWindow(s);
  if(windowSuccess == 0){
    destroyScene(s);
    return 1;
  }
  IMEJ_COL_BACKGROUND.col = 0x00FFFFFF;
  IMEJ_COL_DEFAULT = (colType){.r=0,.g=230,.b=230};
#endif
  // Object* obj = getCube();
  Object* obj = getCube();
  translateObject(obj,(vec3){0,0,3});
  addModelToScene(s,obj);
  // Object* obj = loadObjectFromDisk("model.obj");
  // #define DEPTH 2
  // #define RANGE 1
  // int totalObjects =0;
  // for(int z=0;z<DEPTH;z++){
  //   for(int x=-RANGE;x<RANGE;x++){
  //     for(int y=-RANGE;y<RANGE;y++){
  //       totalObjects++;
  //       Object* newObj = cloneObject(obj);
  //       scaleObject(newObj,0.2);
  //       translateObject(newObj,(vec3){x/3.0f,y/3.0f,2*z+2});
  //       addModelToScene(s,newObj);
  //     }
  //   }
  // }
  // printf("%d\n",totalObjects);
  // deleteObject(obj);
// #define FRAMES 8000 
#define FRAMES 10000
  time_t start = clock();
#ifndef DO_DISPLAY
  for(int i=0;i< FRAMES ;i++){
    renderScene(s);
  }
#endif
  time_t end = clock();
  printf("Rendered %d frames in %fs\n",FRAMES, ((double)(end - start))/CLOCKS_PER_SEC);
  printf("Averaged %f fps\n",FRAMES/(((double)(end - start))/CLOCKS_PER_SEC));
  Camera* c = getSceneCamera(s);
  float rotateValue = (M_PI*0.01)/180.0f;
  start = clock();
  for(int i=0;i< FRAMES/10;i++){
    for(int j=0;j<5;j++){
#ifdef DO_DISPLAY
      renderFrame(s);
#endif
      renderScene(s);
      moveCameraForwards(c,0,0,0.001);
      rotateCamera(c,rotateValue,0);
    }
    for(int j=0;j<5;j++){
#ifdef DO_DISPLAY
      renderFrame(s);
#endif
      renderScene(s);
      rotateCamera(c,-rotateValue,0);
      moveCameraForwards(c,0,0,-0.001);
    }
  }
  end = clock();
  printf("Rendered %d frames in %fs\n",FRAMES, ((double)(end - start))/CLOCKS_PER_SEC);
  printf("Averaged %f fps\n",FRAMES/(((double)(end - start))/CLOCKS_PER_SEC));
  destroyScene(s);
  #ifdef DO_DISPLAY
  closeWindow();
  #endif
}
