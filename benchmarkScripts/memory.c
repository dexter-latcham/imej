#include "../include/imej.h"
#include "../include/window.h"
#include "../include/extras/cube.h"
#include <stdio.h>
#include <time.h>
#define DIMS 500
int main(){
  Scene *s = createScene(DIMS,DIMS);
  if(s==NULL){
    return 1;
  }
  Object* obj = loadObjectFromDisk("model.obj");
  #define DEPTH 2
  #define RANGE 6
  #define MAXOBJECTS 20
  int totalObjects =0;
  for(int z=0;z<DEPTH;z++){
    for(int x=-RANGE;x<RANGE;x++){
      for(int y=-RANGE;y<RANGE;y++){
        totalObjects++;
        Object* newObj = loadObjectFromDisk("model.obj");
        // Object* newObj = cloneObject(obj);
        scaleObject(newObj,0.1);
        translateObject(newObj,(vec3){x/3.0f,y/3.0f,10*z});
        addModelToScene(s,newObj);
#define FRAMES 10
        if(totalObjects == MAXOBJECTS){
          goto doneObjects;
        }
      }
    }
  }
doneObjects:
  deleteObject(obj);
  destroyScene(s);
}
