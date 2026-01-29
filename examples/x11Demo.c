#include "../include/imej.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
  IMEJ_COL_BACKGROUND.col = 0x00FFFFFF;
  IMEJ_COL_DEFAULT = (colType){.r=0,.g=230,.b=230};
  Scene *s = createScene(1000,600);
  if(s==NULL){
    printf("Error opening display\n");
    return 1;
  }
  int windowSuccess = openWindow(s);
  if(windowSuccess == 0){
    printf("COULDNT OPEN WINDOW\n");
    destroyScene(s);
    return 1;
  }
  Object * obj = loadObjectFromDisk("model.obj");
  if(obj == NULL){
    printf("Error reading model, ensure file exists\n");
    goto cleanup;
  }
  scaleObject(obj,2);
  translateObject(obj,(vec3){0,-1,3});
  addModelToScene(s,obj);
  startRenderingLoop(s);
cleanup:
  closeWindow();
  destroyScene(s);
  return 0;
}

