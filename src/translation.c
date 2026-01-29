#include "object.h"
#include "translation.h"
#include <math.h>
#include <stdlib.h>
#ifndef M_PI
#define M_PI 3.1415
#endif
struct transform{
  mat4 matrix;
};

mat4 noTransform(){
  mat4 ret = {{
    {1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {0,0,0,1},
  }};
  return ret;
}

transform* createNoTransform(){
  transform* t = (transform*)malloc(sizeof(transform));
  if(t == NULL){
    return NULL;
  }
  t->matrix = noTransform();
  return t;
}


mat4* getObjectTransformationMatrix(Object* obj){
  if(obj->transformation==NULL){
    obj->transformation = createNoTransform();
  }
  return &(obj->transformation->matrix);
}

void combineTransform(transform* out, transform* a, transform* b){
  multmat4mat4Ptr(&(out->matrix),&(a->matrix),&(b->matrix));
}

void recordTransformation(Object* obj, mat4 transformation){
  if(obj->transformation==NULL){
    obj->transformation=createNoTransform();
  }
  mat4 ret;
  multmat4mat4Ptr(&(ret),&(transformation),&(obj->transformation->matrix));
  obj->transformation->matrix = ret;
}

mat4 getScaleMatrix(float scaleFactor){
  mat4 scaleMatrix={{
    {scaleFactor,0,0,0},
    {0,scaleFactor,0,0},
    {0,0,scaleFactor,0},
    {0,0,0,1}}};
  return scaleMatrix;
}

void destroyTransform(transform* t){
  if(t!=NULL){
    free(t);
  }
}

mat4 getTranslationMatrix(vec3 change){
  mat4 translation = {{
    {1,0,0,change.x},
    {0,1,0,change.y},
    {0,0,1,change.z},
    {0,0,0,1}}};
  return translation;
}

mat4 getZRotation(float angleInDegrees){ //roll
  float angleInRadians = angleInDegrees*M_PI / 180.0f; 
  mat4 ret = {0};
  ret.m[0][0]=cosf(angleInRadians);
  ret.m[0][2]=-sinf(angleInRadians);
  ret.m[2][0]=sinf(angleInRadians);
  ret.m[1][1]=1.0f;
  ret.m[2][2]=cosf(angleInRadians);
  ret.m[3][3]=1.0f;
  return ret;
}

mat4 getYRotation(float angleInDegrees){ //yaw
  float angleInRadians = angleInDegrees*M_PI / 180.0f; 
  mat4 ret = {0};
  ret.m[0][0]=cosf(angleInRadians);
  ret.m[0][1]=-sinf(angleInRadians);
  ret.m[1][0]=sinf(angleInRadians);
  ret.m[1][1]=cosf(angleInRadians);
  ret.m[2][2]=1.0f;
  ret.m[3][3]=1.0f;
  return ret;
}

mat4 getXRotation(float angleInDegrees){ //pitch
  float angleInRadians = angleInDegrees*M_PI / 180.0f; 
  mat4 ret = {0};
  ret.m[0][0]=1.0f;
  ret.m[1][1]=cosf(angleInRadians);
  ret.m[1][2]=-sinf(angleInRadians);
  ret.m[2][1]=sinf(angleInRadians);
  ret.m[2][2]=cosf(angleInRadians);
  ret.m[3][3]=1.0f;
  return ret;
}

void rotateObject(Object* obj, float pitch, float roll, float yaw){
  mat4* current = getObjectTransformationMatrix(obj);
  vec3 cent = multMat4Vec3(*current, (vec3){0,0,0});
  rotateObjectAbout(obj,cent,pitch,roll,yaw);
}


void rotateObjectAbout(Object* obj, vec3 about, float pitch, float roll, float yaw){
  mat4 rotationMatrix = {{
    {1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {0,0,0,1},
  }};
  if(pitch != 0){
    mat4 pitchMat = getXRotation(pitch);
    rotationMatrix = multMat4Mat4(rotationMatrix, pitchMat);
  }

  if(roll != 0){
    mat4 rollMat = getZRotation(roll);
    rotationMatrix = multMat4Mat4(rotationMatrix, rollMat);
  }

  if(yaw != 0){
    mat4 yawMat = getYRotation(yaw);
    rotationMatrix = multMat4Mat4(rotationMatrix, yawMat);
  }

  mat4 trans1 = getTranslationMatrix(about);
  mat4 trans2 = getTranslationMatrix(vec3MultScalar(about, -1));
  mat4 combined = multMat4Mat4(multMat4Mat4(trans1,rotationMatrix),trans2);

  recordTransformation(obj,combined);
}



mat4 getMatAboutPoint(mat4 trans, vec3 about){
  mat4 a = getTranslationMatrix(about);
  mat4 b = getTranslationMatrix(vec3Negate(about));
  return multMat4Mat4(multMat4Mat4(a,trans),b);
}




void scaleObject(Object* obj, float scaleFactor){
  mat4* current = getObjectTransformationMatrix(obj);
  vec3 cent = multMat4Vec3(*current,(vec3){0,0,0});
  scaleObjectAboutPoint(obj,cent,scaleFactor);
}

void scaleObjectAboutPoint(Object* obj, vec3 point, float scaleFactor){
  mat4 trans1 = getTranslationMatrix(point);
  mat4 scale = getScaleMatrix(scaleFactor);
  mat4 trans2 = getTranslationMatrix(vec3MultScalar(point, -1));
  mat4 combined = multMat4Mat4(multMat4Mat4(trans1,scale),trans2);
  recordTransformation(obj,combined);
}

void translateObject(Object* obj, vec3 translation){
  mat4 translationMat = getTranslationMatrix(translation);
  recordTransformation(obj,translationMat);
}
