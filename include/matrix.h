#ifndef IMEJ_MATRIX
#define IMEJ_MATRIX

#include "vectors.h"

typedef struct{
  float m[4][4];
}mat4;


mat4 multMat4Mat4(mat4 a, mat4 b);
vec3 multMat4Vec3(mat4 a, vec3 b);
vec3 multMat4Vec3Normal(mat4 a, vec3 b);


void multmat4mat4Ptr(mat4* out, mat4 *a, mat4 *b);
void multMat4Vec3Ptr(vec3* out, mat4* a, vec3 *b);
#endif
