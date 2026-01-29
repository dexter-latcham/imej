#ifndef IMEJ_PROJECT
#define IMEJ_PROJECT
#include "raster.h"
#include "object.h"
#include "camera.h"
#include "vectors.h"

void projectObject(Camera* cam,Rasterizer* r,vec3 lightSource,Object* obj);
#endif
