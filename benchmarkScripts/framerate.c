#include "../include/imej.h"
#include "../include/window.h"
#include "../include/extras/cube.h"
#include <stdio.h>
#include <time.h>
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
  Object* obj = loadObjectFromDisk("model.obj");
  #define DEPTH 2
  #define RANGE 6
  #define MAXOBJECTS 100
  int totalObjects =0;
  for(int z=0;z<DEPTH;z++){
    for(int x=-RANGE;x<RANGE;x++){
      for(int y=-RANGE;y<RANGE;y++){
        totalObjects++;
        Object* newObj = cloneObject(obj);
        scaleObject(newObj,0.1);
        translateObject(newObj,(vec3){x/3.0f,y/3.0f,10*z});
        addModelToScene(s,newObj);
#define FRAMES 10
        time_t start = clock();
        for(volatile int i=0;i< FRAMES ;i++){
          renderScene(s);
        }
        time_t end = clock();
        // printf("%d objects\n",totalObjects);
        // printf("%d frames in %fs\n",FRAMES, ((double)(end - start))/CLOCKS_PER_SEC);
        printf("%f\n",FRAMES/(((double)(end - start))/CLOCKS_PER_SEC));
        if(totalObjects == MAXOBJECTS){
          goto doneObjects;
        }
      }
    }
  }
doneObjects:
  deleteObject(obj);
#ifdef DO_DISPLAY
  startRenderingLoop(s);
  closeWindow();
  #else
// #define FRAMES 3
//   time_t start = clock();
//   for(volatile int i=0;i< FRAMES ;i++){
//     renderScene(s);
//   }
//   time_t end = clock();
//   printf("Rendered %d frames in %fs\n",FRAMES, ((double)(end - start))/CLOCKS_PER_SEC);
//   printf("Averaged %f fps\n",FRAMES/(((double)(end - start))/CLOCKS_PER_SEC));
#endif
  destroyScene(s);
}
