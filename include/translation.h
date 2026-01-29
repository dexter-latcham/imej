#ifndef IMEJ_TRANSLATIONS
#define IMEJ_TRANSLATIONS
#include "vectors.h"
#include "matrix.h"

typedef struct transform transform;
typedef struct Object Object;

transform* createNoTransform();
void destroyTransform(transform* t);
mat4* getObjectTransformationMatrix(Object* obj);

void translateObject(Object* obj, vec3 translation);
void scaleObject(Object* obj, float scaleFactor);
void scaleObjectAboutPoint(Object* obj, vec3 point, float scaleFactor);
void rotateObject(Object* obj, float pitch, float roll, float yaw);
void rotateObjectAbout(Object* obj, vec3 about, float pitch, float roll, float yaw);

#endif
