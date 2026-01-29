#include "../include/imej.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void update(Object* model, int* frameNumber){
  rotateObjectAbout(model, (vec3){0,0,4}, 3,2,1);
}


int main(){
  Scene *s = createScene(1000,1000);
  int windowSuccess = openWindow(s);
  if(windowSuccess == 0){
    printf("COULDNT OPEN WINDOW\n");
    destroyScene(s);
    return 1;
  }
  Object * model = loadObjectWithColour("model.obj",WHITECOL);
  if(model == NULL){
    printf("Error reading model, ensure file exists\n");
    goto cleanup;
  }
  model->update = update;
  translateObject(model,(vec3){0,0,4});
  addModelToScene(s,model);
  startRenderingLoop(s);
cleanup:
  closeWindow();
  destroyScene(s);
}
