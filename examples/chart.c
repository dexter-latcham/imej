#include "../include/imej.h"
#include "../include/extras/graph.h"
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
static int16_t* graphData;
static int graphSize;

void createSinGraph(int dimensions){
  if(graphData != NULL){
    free(graphData);
  }
  if(dimensions%2 != 0){
    dimensions++;
  }
  graphSize = dimensions;
  graphData = malloc(sizeof(int16_t)*dimensions*dimensions);
  for(int i=0; i<dimensions; i++){
    for(int j=0;j<dimensions;j++){
      float x = (i-(dimensions/2))*(i-dimensions/2);
      float y = (j-(dimensions/2))*(j-dimensions/2);
      graphData[i*dimensions+j]=(1+sin(sqrt(x+y)))*(VERTMAX>>4);
    }
  }
}

void generateWave(int* frameNumber){
  for(int i=0; i<graphSize; i++){
    for(int j=0;j<graphSize;j++){
      float x = (i-(graphSize/2))*(i-(graphSize/2));
      float y = (j-(graphSize/2))*(j-(graphSize/2));
      graphData[i*graphSize+j]=(1+sin(sqrt(x+y)+*frameNumber/40.0f))*(VERTMAX>>4);
    }
  }
}

void graphUpdate(Object* obj, int *frameNumber){
  generateWave(frameNumber);
  updateGraph(obj,(int16_t*)graphData,graphSize,graphSize);
}

int main(){
  Scene *s = createScene(1000,1000);
  int windowSuccess = openWindow(s);
  if(windowSuccess == 0){
    printf("COULDNT OPEN WINDOW\n");
    destroyScene(s);
    return 1;
  }
  createSinGraph(10);
  Object* graph = getGraph((uint16_t*)graphData,graphSize,graphSize);
  graph->update=graphUpdate;

  addModelToScene(s,graph);
  Camera*c = getSceneCamera(s);
  setCameraPositionDegrees(c, (vec3){.x=-0.2,.y=1.5,.z=-0.14},-60,32);
  startRenderingLoop(s);
cleanup:
  closeWindow();
  destroyScene(s);
  return 1;
}

