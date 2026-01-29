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


#define DIMS 500
#define DO_DISPLAY
int main(){
  Scene *s = createScene(DIMS,DIMS);
  if(s==NULL){
    return 1;
  }
  Camera*c = getSceneCamera(s);
  setCameraPositionDegrees(c, (vec3){.x=-0.2,.y=1.5,.z=-0.14},-60,32);
#define FRAMES 1
  for(int i=10;i<200;i+=5){
    createSinGraph(i);
    Object* graph = getGraph((uint16_t*)graphData,graphSize,graphSize);
    graph->update=graphUpdate;
    addModelToScene(s,graph);
    time_t start = clock();
    for(volatile int i=0;i< FRAMES ;i++){
      renderScene(s);
    }
    time_t end = clock();
    printf("%d\n",graph->faceCount);
    // printf("%d %f\n",i,FRAMES/(((double)(end - start))/CLOCKS_PER_SEC));
    deleteObjectFromList(getSceneModels(s),graph);
  }
  destroyScene(s);
  return 1;
}

