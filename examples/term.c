#include "../include/imej.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ioctl.h>

void update(Object* obj,int *frameNum){
  rotateObjectAbout(obj, (vec3){0,0,4}, 5,4,3);
  usleep(1000000/40); // approx 40fps
}

int main(){
  Scene *s = createScene(800,200);
  int windowSuccess = openWindow(s);
  if(windowSuccess == 0){
    printf("COULDNT OPEN WINDOW\n");
    destroyScene(s);
    return 1;
  }
  Object * model = loadObjectFromDisk("model.obj");
  if(model == NULL){
    printf("Error reading model, ensure file exists\n");
    goto cleanup;
  }
  scaleObjectAboutPoint(model,(vec3){0,0,0},2);
  translateObject(model,(vec3){0,0,4});
  addModelToScene(s,model);
  model->update = update;
  startRenderingLoop(s);
cleanup:
  closeWindow();
  destroyScene(s);
  return 0;
}

