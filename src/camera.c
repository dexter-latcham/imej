#include "raster.h"
#include "camera.h"
#include "translation.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.1415
#endif
void setForwardsVec(Camera*c){
  vec3 newFacing;
  newFacing.x = cos(c->yaw)*cos(c->pitch);
  newFacing.y = sin(c->pitch);
  newFacing.z = sin(c->yaw)*cos(c->pitch);
  newFacing = vec3Normalised(newFacing);
  c->facing = newFacing;
}

void setCameraPositionDegrees(Camera* c,vec3 pos, float pitch, float yaw){
  float pitchRadians =(pitch*M_PI)/180;
  float yawRadians =(yaw*M_PI)/180;
  setCameraPosition(c,pos,pitchRadians,yawRadians);
}

void setCameraPosition(Camera* c,vec3 pos, float pitch, float yaw){
  c->pos = pos;
  c->pitch = pitch;
  c->yaw=yaw;
  setForwardsVec(c);
  c->cameraMoved=1;
}

mat4 createOrthogonalMatrix(float n, float f, float cameraWidth, float screenWidth, float screenHeight){
  float zMax=ZBUFFERLIMIT;
  float r = (cameraWidth/2);
  float t = (cameraWidth*screenHeight)/(screenWidth*2);
  mat4 ret = {{
    {-screenWidth/(2*r),0,0,screenWidth/2},
    {0,-screenHeight/(2*t),0,screenHeight/2},
    {0,0,zMax/(f-n),-(n*zMax)/(f-n)},
    {0,0,0,1}
  }};
  return ret;
}

Camera* createCamera(int width, int height){
  Camera * c =(Camera*) malloc(sizeof(Camera));
  if(c==NULL){
    return NULL;
  }
  c->pos = (vec3){0,0,0};
  c->pitch=0.0f;
  c->yaw = M_PI/2;
  c->frustumNear=0.1;
  c->frustumFar=500;
  c->projectedBoundsLeft=0;
  c->projectedBoundsRight=width;
  c->projectedBoundsBase=0;
  c->projectedBoundsTop=height;
  c->cameraMoved =1;
  setForwardsVec(c);
  getWorldToCameraMatrix(c);
  return c;
}

Camera* createCameraPerspective(int width, int height,float fovDegrees){
  Camera * c =createCamera(width,height);
  float fovRadians = fovDegrees * M_PI / 180.0f;
  float s = 1.0f / tanf(fovRadians/2);
  float zMax=ZBUFFERLIMIT;
  float near = c->frustumNear;
  float far = c->frustumFar;
  c->perspectiveMat = (mat4){{
    {-s *height/2.0f,0,-width/2.0f,0},
    {0,s*height/2.0f,-height/2.0f,0},
    {0,0,-(far*zMax)/(far-near),(near*far*zMax)/(far-near)},
    {0,0,-1,0}
  }};
  c->frustumLeftNormal = vec3Normalised((vec3){-cos(fovRadians/2),0,-sin(fovRadians/2)});
  c->frustumRightNormal= vec3Normalised((vec3){cos(fovRadians/2),0,-sin(fovRadians/2)});
  float verticalFov = atan((height*1.0/width)*tanf(fovRadians/2.0))*2;
  c->frustumTopNormal = vec3Normalised((vec3){0,cos(verticalFov/2),-sin(verticalFov/2)});
  c->frustumBaseNormal = vec3Normalised((vec3){0,-cos(verticalFov/2),-sin(verticalFov/2)});
  return c;
}

Camera* createCameraOrthogonal(int width, int height, int cameraWidth){
  Camera * c =createCamera(width,height);
  c->perspectiveMat = createOrthogonalMatrix(c->frustumNear, c->frustumFar, cameraWidth,width, height);
  return c;
}

mat4* getWorldToCameraMatrix(Camera *c){
  if(c->cameraMoved == 0){
    return &(c->worldToCamera);
  }
  setForwardsVec(c);
  vec3 forward = c->facing;
  vec3 right = vec3Normalised(crossProduct((vec3){0,1,0}, forward));
  vec3 up = crossProduct(forward, right);
  mat4 ret = {{
    {right.x, right.y, right.z, 0.0f},
    {up.x, up.y, up.z, 0.0f},
    {forward.x, forward.y, forward.z, 0.0f},
    {0.0f,0.0f,0.0f,1.0f}
  }};

  mat4 trans = {{
    {1,0,0,-c->pos.x},
    {0,1,0,-c->pos.y},
    {0,0,1,-c->pos.z},
    {0,0,0,1},
  }};
  ret = multMat4Mat4(ret,trans);
  c->worldToCamera = ret;
  c->cameraMoved =0;
  return &(c->worldToCamera);
}



void rotateCameraDegrees(Camera* c, float yaw, float pitch){
  float pitchRadians =(pitch*M_PI)/180;
  float yawRadians =(yaw*M_PI)/180;
  rotateCamera(c,yawRadians,pitchRadians);
}

#define EIGHTY_DEGREES 1.39622
void rotateCamera(Camera* c, float yaw, float pitch){
  c->yaw-=yaw;
  c->pitch+=pitch;
  if(c->pitch < -EIGHTY_DEGREES){
    c->pitch = -EIGHTY_DEGREES;
  }else if(c->pitch > EIGHTY_DEGREES){
    c->pitch=EIGHTY_DEGREES;
  }
  setForwardsVec(c);
  c->cameraMoved =1;
}


void moveCameraForwards(Camera* c, float xChange, float yChange, float zChange){
  if(zChange!=0){
    c->pos = vec3AddVec3(c->pos, vec3MultScalar(c->facing,zChange));
  }
  if(yChange!=0){
    c->pos = vec3AddVec3(c->pos, vec3MultScalar((vec3){0,1,0},yChange));
  }
  if(xChange!=0){
    vec3 axis = crossProduct((vec3){0,1,0}, c->facing);
    axis = vec3Normalised(axis);
    c->pos = vec3AddVec3(c->pos, vec3MultScalar(axis,-1.0f*xChange));
  }
  c->cameraMoved =1;
}


mat4 getCameraProjectionMatrix(Camera* c){
  return c->perspectiveMat;
}
