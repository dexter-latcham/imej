#ifndef IMEJ_CAMERA
#define IMEJ_CAMERA
#include "vectors.h"
#include "matrix.h"

typedef struct{
  vec3 pos;
  vec3 facing;
  float pitch;
  float yaw;
  int cameraMoved;

  mat4 perspectiveMat;
  mat4 worldToCamera;
  struct{
    vec3 frustumLeftNormal;
    vec3 frustumRightNormal;
    vec3 frustumTopNormal;
    vec3 frustumBaseNormal;
  };
  float frustumNear;
  float frustumFar;
  struct{
    float projectedBoundsLeft;
    float projectedBoundsRight;
    float projectedBoundsBase;
    float projectedBoundsTop;
  };
}Camera;

void setCameraPosition(Camera* c,vec3 pos, float pitch, float yaw);
void setCameraPositionDegrees(Camera* c,vec3 pos, float pitch, float yaw);

Camera* createCamera(int width, int height);

mat4 *getWorldToCameraMatrix(Camera* c);
mat4 getCameraProjectionMatrix(Camera* c);

void moveCameraForwards(Camera* c, float xChange, float yChange, float zChange);
void rotateCameraDegrees(Camera* c, float yaw, float pitch);
void rotateCamera(Camera* c, float yaw, float pitch);

Camera* createCameraOrthogonal(int width,int height, int cameraWidth);
Camera* createCameraPerspective(int width, int height, float fovDegrees);
#endif
