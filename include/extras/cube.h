#ifndef MAKECUBE
#define MAKECUBE
#include "../include/colour.h"
#include "../include/object.h"
#include <stdlib.h>
Object * getCubeColoured(colType col){
  Object* cube = createObject(8,12);
  cube->col=col;

  cube->vertices[0]=(vec3Int){VERTMIN,VERTMIN,VERTMIN};
  cube->vertices[1]=(vec3Int){VERTMIN,VERTMIN,VERTMAX};
  cube->vertices[2]=(vec3Int){VERTMIN,VERTMAX,VERTMIN};
  cube->vertices[3]=(vec3Int){VERTMIN,VERTMAX,VERTMAX};
  cube->vertices[4]=(vec3Int){VERTMAX,VERTMIN,VERTMIN};
  cube->vertices[5]=(vec3Int){VERTMAX,VERTMIN,VERTMAX};
  cube->vertices[6]=(vec3Int){VERTMAX,VERTMAX,VERTMIN};
  cube->vertices[7]=(vec3Int){VERTMAX,VERTMAX,VERTMAX};

  // cube->vertices[0]=(vec3){-1,-1,-1};
  // cube->vertices[1]=(vec3){-1,-1,1};
  // cube->vertices[2]=(vec3){-1,1,-1};
  // cube->vertices[3]=(vec3){-1,1,1};
  // cube->vertices[4]=(vec3){1,-1,-1};
  // cube->vertices[5]=(vec3){1,-1,1};
  // cube->vertices[6]=(vec3){1,1,-1};
  // cube->vertices[7]=(vec3){1,1,1};
  cube->faces[0 ]=(Face){0,1,2};
  cube->faces[1 ]=(Face){3,2,1};
  cube->faces[2 ]=(Face){0,2,4};
  cube->faces[3 ]=(Face){6,4,2};
  cube->faces[4 ]=(Face){7,3,1};
  cube->faces[5 ]=(Face){5,7,1};
  cube->faces[6 ]=(Face){4,1,0};
  cube->faces[7 ]=(Face){1,4,5};
  cube->faces[8 ]=(Face){2,3,7};
  cube->faces[9 ]=(Face){2,7,6};
  cube->faces[10]=(Face){7,5,4};
  cube->faces[11]=(Face){6,7,4};
  return cube;
}
Object * getCube(){
  return getCubeColoured(IMEJ_COL_DEFAULT);
}
#endif
