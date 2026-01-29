#ifndef IMEJ_SCENE
#define IMEJ_SCENE
#include "vectors.h"
#include "camera.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Scene Scene;
typedef struct Rasterizer Rasterizer;
typedef struct Object Object;
typedef struct objectList objectList;

Scene * createScene(int width, int height);
void destroyScene(Scene* s);
Camera* getSceneCamera(Scene* s);
Rasterizer* getRasterizer(Scene* s);
void addModelToScene(Scene*s, Object* obj);
objectList* getSceneModels(Scene*s);
int* getSceneFrameCounter(Scene*s);
vec3 getSceneLightSource(Scene*s);

void renderScene(Scene *scene);
#ifdef IMEJ_DO_INPUT
struct InputHandler* getSceneInputHandler(Scene*s);
#endif

#ifdef __cplusplus
}
#endif
#endif
