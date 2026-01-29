#ifndef IMEJ_VECTORS
#define IMEJ_VECTORS

typedef struct{
  float x,y,z;
}vec3;

float vec3MagRecip(vec3 vec);
float vec3Mag(vec3 vec);

float vec3DotProduct(vec3 a, vec3 b);
vec3 vec3Normalised(vec3 vec);
vec3 crossProduct(vec3 a, vec3 b);
vec3 vec3SubVec3(vec3 a, vec3 b);
vec3 lineIntersectsPlane(vec3 line1, vec3 line2, vec3 plane, vec3 planeNormal);
vec3 paneNormal(vec3 a, vec3 b, vec3 c);
vec3 vec3AddVec3(vec3 a, vec3 b);
vec3 vec3MultScalar(vec3 v, float scalar);
vec3 vec3Negate(vec3 v);

float distanceToPlane(vec3 point, vec3 plane, vec3 planeNormal);
#endif
