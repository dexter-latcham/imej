#include "imej.h"
#include "cube.h"

// Define the dimensions of the display.
#define LCD_WIDTH       128
#define LCD_HEIGHT      160 //160
int main(){
  Scene* s = createScene(LCD_WIDTH, LCD_HEIGHT);
  int windowSuccess = openWindow(s);
  if(windowSuccess == 0){
    destroyScene(s);
    return 1;
  }
  // uBit.seedRandom();
  colType colours[8] = {
    (colType){.r=4,.g=0,.b=0}
  };

  Object* model = getCube();
  translateObject(model,(vec3){0,0,4});
  addModelToScene(s,model);
  Object* newObject;
  for(int x=-2;x<3;x++){
    for(int y=-2;y<3;y++){
      // colType col;
      // col.r = uBit.random(8);
      // col.g = uBit.random(8);
      // col.b = uBit.random(8);
      newObject = cloneObject(model);
      newObject->col = colours[0];
      translateObject(newObject,(vec3){x*4,y*4,4});
      addModelToScene(s,model);
    }
  }
  startRenderingLoop(s);
  closeWindow();
  destroyScene(s);
}

