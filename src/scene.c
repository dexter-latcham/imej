#include "raster.h"
#include "projection.h"
#include "scene.h"
#include "input.h"
#include <stdlib.h>

struct Scene{
  Rasterizer* raster;
  Camera * camera;
  vec3 lightDirection;
  objectList* models;
#ifdef IMEJ_DO_INPUT
  struct InputHandler *input;
#endif
  int frameNumber;
};


Rasterizer* getRasterizer(Scene* s){
  return s->raster;
}
objectList* getSceneModels(Scene*s){
  return s->models;
}

void addModelToScene(Scene*s, Object* obj){
  addObjectToList(s->models,obj);
}
int* getSceneFrameCounter(Scene*s){
  return &(s->frameNumber);
}
vec3 getSceneLightSource(Scene*s){
  return s->lightDirection;
}
Camera* getSceneCamera(Scene* s){
  return s->camera;
}

Scene * createScene(int width, int height){
  Scene* scene = (Scene*)malloc(sizeof(Scene));
  if(scene == NULL){
    return NULL;
  }
  scene->raster = createRasterizer(width,height);
  if(scene->raster == NULL){
    free(scene);
    return NULL;
  }
#ifdef IMEJ_ORTHAGONAL
  scene->camera = createCameraOrthogonal(width,height,8);
#else
  scene->camera = createCameraPerspective(width, height,70);
#endif
  scene->models=createObjectList();
  scene->lightDirection=(vec3Normalised((vec3){1,1,1}));
  scene->frameNumber =0;
#ifdef IMEJ_DO_INPUT
  scene->input=createInputHandler(scene->camera);
#endif
  return scene;
}


#ifdef IMEJ_DO_INPUT
struct InputHandler* getSceneInputHandler(Scene*s){
  return s->input;
}
#endif
#include <stdio.h>
void destroyScene(Scene* s){
  if(s != NULL){
#ifdef IMEJ_DO_INPUT
    if(s->input != NULL){
      free(s->input);
    }
#endif
    if(s->camera != NULL){
      free(s->camera);
    }
    deleteObjectList(s->models);
    destroyRasterizer(s->raster);
  }
  free(s);
}





void renderScene(Scene *s){
  wipeFrameBuffer(s->raster);
  s->frameNumber++;
  Object * obj = objectListIterator(s->models);
  while(obj != NULL){
    if(obj->update != NULL){
      obj->update(obj,&(s->frameNumber));
    }
    projectObject(s->camera,s->raster,s->lightDirection,obj);
    obj = objectListIterator(NULL);
  }
}
