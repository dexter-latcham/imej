#include "vectors.h"
#include <math.h>

float vec3DotProduct(vec3 a, vec3 b){
  return a.x*b.x+a.y*b.y+a.z*b.z;
}

float vec3Mag(vec3 vec){
  return sqrt(vec3DotProduct(vec,vec));
}

float vec3MagRecip(vec3 vec){
  return 1.0f/sqrt(vec3DotProduct(vec,vec));
  // return invSqRt(vec3DotProduct(vec,vec));
}

vec3 vec3Normalised(vec3 a){
  vec3 ret = a;
  float lenRecip = vec3MagRecip(a);
  if(lenRecip!=0){
    ret.x*=lenRecip;
    ret.y*=lenRecip;
    ret.z*=lenRecip;
  }
  return ret;
}


vec3 vec3AddVec3(vec3 a, vec3 b){
  return (vec3){a.x+b.x, a.y+b.y, a.z+b.z};
}


vec3 vec3MultScalar(vec3 v, float scalar){
  return (vec3){v.x*scalar, v.y*scalar, v.z * scalar};
}

vec3 crossProduct(vec3 a, vec3 b){
  vec3 ret;
  ret.x = a.y*b.z-a.z*b.y;
  ret.y=a.z*b.x-a.x*b.z;
  ret.z=a.x*b.y-a.y*b.x;
  return ret;
}


vec3 vec3Negate(vec3 v){
  return (vec3){-v.x,-v.y,-v.z};
}

vec3 vec3SubVec3(vec3 a, vec3 b){
  return (vec3){a.x -b.x, a.y - b.y, a.z - b.z};
}
vec3 paneNormal(vec3 a, vec3 b, vec3 c){
  vec3 side1 = vec3SubVec3(b,a);
  vec3 side2 = vec3SubVec3(c,a);
  return vec3Normalised(crossProduct(side1,side2));
}

float distanceToPlane(vec3 point, vec3 plane, vec3 planeNormal){
  return vec3DotProduct(vec3SubVec3(point, plane),planeNormal);
}


vec3 lineIntersectsPlane(vec3 line1, vec3 line2, vec3 plane, vec3 planeNormal){
  vec3 line = vec3SubVec3(line2, line1);
  float dot = vec3DotProduct(planeNormal, line);
  vec3 planeToLine = vec3SubVec3(plane, line1);
  float fac = vec3DotProduct(planeToLine, planeNormal)/dot;
  line =  vec3MultScalar(line,fac);
  return vec3AddVec3(line1,line);
}
