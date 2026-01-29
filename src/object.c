#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include "object.h"
#include "translation.h"

Object * createObject(int vertexCount, int faceCount){
  Object * obj = (Object*)malloc(sizeof(Object));
  if(obj == NULL){
    return NULL;
  }
  obj->vertexCount=vertexCount;
  obj->faceCount=faceCount;
  obj->vertices=(vec3Int*)malloc(sizeof(vec3Int)*vertexCount);
  if(obj->vertices == NULL){
    goto objFailed;
  }
  obj->faces=(Face*)malloc(sizeof(Face)*faceCount);
  if(obj->faces == NULL){
    goto vertFailed;
  }
  obj->transformation=createNoTransform();
  if(obj->transformation == NULL){
    goto faceFailed;
  }
  obj->refCount=(int*)malloc(sizeof(int));
  if(obj->refCount == NULL){
    goto transformFailed;
  }
  obj->update = NULL;
  *obj->refCount = 1;
  return obj;
transformFailed:
  destroyTransform(obj->transformation);
faceFailed:
  free(obj->faces);
vertFailed:
  free(obj->vertices);
objFailed:
  free(obj);
  return NULL;
}

Object* cloneObject(Object* objIn){
  Object * newObj = (Object*)malloc(sizeof(Object));
  if(newObj == NULL){
    return NULL;
  }
  newObj->transformation=createNoTransform();
  if(newObj->transformation == NULL){
    free(newObj);
    return NULL;
  }
  newObj->vertexCount=objIn->vertexCount;
  newObj->faceCount=objIn->faceCount;
  newObj->update = NULL;
  newObj->vertices=objIn->vertices;
  newObj->faces = objIn->faces;
  newObj->refCount=objIn->refCount;
  (*newObj->refCount) = *(newObj->refCount)+1;
  newObj->col = objIn->col;
  return newObj;
}

void deleteObject(Object* obj){
  if(obj!=NULL){
    if(*(obj->refCount)>1){
      *(obj->refCount) = *(obj->refCount)-1;
    }else{
      if(obj->refCount != NULL){
        free(obj->refCount);
      }
      if(obj->vertices!=NULL){
        free(obj->vertices);
      }
      if(obj->faces!=NULL){
        free(obj->faces);
      }
    }
    destroyTransform(obj->transformation);
    free(obj);
  }
}


Object * loadObjectFromDisk(char* filename){
  return loadObjectWithColour(filename, IMEJ_COL_DEFAULT);
}

//if fopen has been implemented
#ifdef __linux__
Object * loadObjectWithColour(char* filename, colType colour){
  FILE* f = fopen(filename,"r");
  if(f==NULL){
    goto fopenFail;
  }
  char buffer[200];
  int vertexCount=0;
  int faceCount=0;
  float min = FLT_MAX;
  float max = -FLT_MIN;
  while(fgets(buffer,sizeof(buffer),f)!=NULL){
    if(buffer[0]=='v'){
      vertexCount++;
      float x,y,z;
      sscanf(buffer, "v %f %f %f",&x, &y, &z);
      if(x<min) min=x;
      if(y<min) min=y;
      if(z<min) min=z;
      if(x>max) max=x;
      if(y>max) max=y;
      if(z>max) max=z;
    }else if(buffer[0]=='f'){
      faceCount++;
    }
  }
  Object * ret = createObject(vertexCount,faceCount);
  if(ret == NULL){
    goto objectFail;
  }
  fseek(f,0,SEEK_SET);
  int vertexI=0, faceI=0;
  float x,y,z;
  while(fgets(buffer,sizeof(buffer),f)!=NULL){
    if(buffer[0]=='v'){
      sscanf(buffer, "v %f %f %f", &x,&y,&z);
      ret->vertices[vertexI].x = ((x-min)/(max-min))*(VERTMAX-VERTMIN)+VERTMIN;
      ret->vertices[vertexI].y = ((y-min)/(max-min))*(VERTMAX-VERTMIN)+VERTMIN;
      ret->vertices[vertexI].z = ((z-min)/(max-min))*(VERTMAX-VERTMIN)+VERTMIN;
      vertexI++;
    }else if(buffer[0]=='f'){
      sscanf(buffer, "f %hd %hd %hd", &(ret->faces[faceI].a), &(ret->faces[faceI].b), &(ret->faces[faceI].c));
      ret->faces[faceI].a-=1;
      ret->faces[faceI].b-=1;
      ret->faces[faceI].c-=1;
      faceI++;
    }
  }
  fclose(f);
  ret->col = colour;
  return ret;
objectFail:
  fclose(f);
fopenFail:
  return NULL;
}
#else
Object * loadObjectWithColour(char* filename, colType col){
  return NULL;
}
#endif
